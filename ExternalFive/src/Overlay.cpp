#include "pch.h"
#include "Overlay.h"
#include "Cheats.h"
#include "ui/Menu.h"
#include "ui/Theme.h"
#include "features/Visuals.h"
#include "features/Aimbot.h"
#include "features/LuaExec.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include <MinHook.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// ---------------------------------------------------------------- direcciones
static constexpr uintptr_t FUNC_CPad_UpdatePads       = 0x541DD0;  // CPad::UpdatePads
static constexpr uintptr_t FUNC_CPad_ClearMouseHistory = 0x53F1E0;
static constexpr uintptr_t CPad_Instance              = 0xB73458;  // CGameSA.h: CLASS_CPad
static constexpr uintptr_t CPad_NewMouse              = 0xB73418;
static constexpr uintptr_t CPad_OldMouse              = 0xB73404;

using EndScene_t   = HRESULT(__stdcall*)(IDirect3DDevice9*);
using Reset_t      = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using UpdatePads_t = void(__stdcall*)();
using ClearMouse_t = void(__thiscall*)(void*);

// ---------------------------------------------------------------- estado
static EndScene_t   oEndScene    = nullptr;
static Reset_t      oReset       = nullptr;
static UpdatePads_t oUpdatePads  = nullptr;
static HWND         g_hWnd       = nullptr;
static WNDPROC      g_oWndProc   = nullptr;
static IDirect3DDevice9* g_device = nullptr;
static bool  g_ready   = false;
static bool  g_running = false;
static bool  g_cursor  = false;

// ---------------------------------------------------------------- utilidades
static void SetCursorState(bool show)
{
    if (show == g_cursor) return;
    g_cursor = show;
    if (show) { ShowCursor(TRUE); ClipCursor(nullptr); }
    else      { ShowCursor(FALSE); }
}

static void ToggleMenu()
{
    g_menu.open = !g_menu.open;
    SetCursorState(g_menu.open);
}

// ---------------------------------------------------------------- wndproc
static LRESULT CALLBACK hkWndProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    if (m == WM_KEYDOWN && w == VK_INSERT) { ToggleMenu(); return 0; }

    if (g_menu.open)
    {
        if (m == WM_KEYDOWN && w == VK_F5) { Lua_Run(g_cfg.luaBuf); return 0; }

        if (ImGui_ImplWin32_WndProcHandler(h, m, w, l)) return 0;

        // el juego no debe ver teclado mientras el menú está abierto
        switch (m)
        {
        case WM_KEYDOWN: case WM_KEYUP:
        case WM_SYSKEYDOWN: case WM_SYSKEYUP:
        case WM_CHAR: case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
            return 0;
        }
    }

    if (m == WM_SETCURSOR && g_menu.open)
    {
        SetCursor(LoadCursorA(nullptr, IDC_ARROW));
        return TRUE;
    }

    return CallWindowProcA(g_oWndProc, h, m, w, l);
}

// ---------------------------------------------------------------- cpad
static void __stdcall hkUpdatePads()
{
    oUpdatePads();

    if (!g_menu.open) return;

    // congela el mouse del juego: sin esto la cámara gira al mover el menú
    memset(reinterpret_cast<void*>(CPad_NewMouse), 0, 0x14);
    memset(reinterpret_cast<void*>(CPad_OldMouse), 0, 0x14);

    if (mem::Valid(CPad_Instance))
        reinterpret_cast<ClearMouse_t>(FUNC_CPad_ClearMouseHistory)(reinterpret_cast<void*>(CPad_Instance));
}

// ---------------------------------------------------------------- d3d9
static bool InitImGui(IDirect3DDevice9* dev)
{
    D3DDEVICE_CREATION_PARAMETERS cp{};
    if (FAILED(dev->GetCreationParameters(&cp)) || !cp.hFocusWindow) return false;

    g_hWnd = cp.hFocusWindow;
    g_oWndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrA(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
    if (!g_oWndProc) return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename   = nullptr;
    io.LogFilename   = nullptr;
    io.ConfigFlags  |= ImGuiConfigFlags_NoMouseCursorChange;

    th::Load();
    th::Apply();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(dev);

    g_device = dev;
    g_ready  = true;
    SetCursorState(g_menu.open);
    return true;
}

static HRESULT __stdcall hkReset(IDirect3DDevice9* dev, D3DPRESENT_PARAMETERS* pp)
{
    if (g_ready) ImGui_ImplDX9_InvalidateDeviceObjects();

    HRESULT hr = oReset(dev, pp);

    if (SUCCEEDED(hr))
    {
        g_device = dev;
        if (g_ready) ImGui_ImplDX9_CreateDeviceObjects();
    }
    return hr;
}

static HRESULT __stdcall hkEndScene(IDirect3DDevice9* dev)
{
    if (!g_ready && !InitImGui(dev))
        return oEndScene(dev);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Cheats_Tick();

    Menu_Render();          // incluye Menu_Preview()
    Visuals_Render();       // ESP en foreground drawlist

    bool aiming = (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0;
    if (!aiming && g_cfg.aimHold && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
        aiming = true;
    Aimbot_Tick(aiming && !g_menu.open);

    ImGui::EndFrame();
    ImGui::Render();

    if (ImGui::GetDrawData())
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return oEndScene(dev);
}

// ---------------------------------------------------------------- ciclo de vida
bool Overlay_Init()
{
    if (g_running) return true;
    if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED)
        return false;

    // espera a que exista una ventana de juego real
    HWND target = nullptr;
    for (int i = 0; i < 300 && !target; ++i)
    {
        target = FindWindowA(nullptr, "Multi Theft Auto: San Andreas");
        if (!target) target = FindWindowA(nullptr, "GTA: San Andreas");
        if (!target) Sleep(200);
    }

    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return false;

    D3DPRESENT_PARAMETERS pp{};
    pp.Windowed          = TRUE;
    pp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat  = D3DFMT_UNKNOWN;
    pp.hDeviceWindow     = target ? target : GetForegroundWindow();

    IDirect3DDevice9* dummy = nullptr;
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.hDeviceWindow,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &dummy);

    bool ok = false;
    if (dummy)
    {
        void** vt = *reinterpret_cast<void***>(dummy);

        ok = MH_CreateHook(vt[42], &hkEndScene, reinterpret_cast<void**>(&oEndScene)) == MH_OK;
        ok &= MH_CreateHook(vt[16], &hkReset,    reinterpret_cast<void**>(&oReset))    == MH_OK;

        dummy->Release();
    }
    d3d->Release();

    if (!ok) return false;

    MH_CreateHook(reinterpret_cast<void*>(FUNC_CPad_UpdatePads), &hkUpdatePads,
                  reinterpret_cast<void**>(&oUpdatePads));

    MH_EnableHook(MH_ALL_HOOKS);

    Lua_Init();

    g_running = true;
    return true;
}

void Overlay_Shutdown()
{
    if (!g_running) return;

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    if (g_ready)
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        g_ready = false;
    }

    if (g_hWnd && g_oWndProc)
    {
        SetWindowLongPtrA(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_oWndProc));
        g_oWndProc = nullptr;
    }

    SetCursorState(false);
    g_running = false;
}