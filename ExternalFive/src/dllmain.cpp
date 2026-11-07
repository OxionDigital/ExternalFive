#include "pch.h"
#include "Overlay.h"

static DWORD WINAPI Main(LPVOID)
{
    // el proceso arranca antes de que exista d3d9 y la ventana
    for (int i = 0; i < 600; ++i)
    {
        if (GetModuleHandleA("d3d9.dll")) break;
        Sleep(100);
    }
    Sleep(1500);

    if (!Overlay_Init())
    {
        MessageBoxA(nullptr, "Overlay_Init fallo (¿d3d9 no cargado?)", "mtacheat", MB_ICONERROR);
        return 0;
    }

    while (!(GetAsyncKeyState(VK_END) & 0x8000)) Sleep(100);
    Overlay_Shutdown();

    FreeLibraryAndExitThread(GetModuleHandleA("mtacheat.dll"), 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, Main, nullptr, 0, nullptr);
    }
    return TRUE;
}