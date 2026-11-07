#include "pch.h"
#include "LuaExec.h"
#include "../Cheats.h"

using pcall_t = int(__cdecl*)(void*, int, int, int);
using load_t  = int(__cdecl*)(void*, const char*, size_t, const char*);

static pcall_t g_pcall = nullptr;
static load_t  g_load  = nullptr;
static void*   g_state = nullptr;

static int __cdecl hk_pcall(void* L, int a, int b, int c)
{
    g_state = L;
    return g_pcall(L, a, b, c);
}

void Lua_Init()
{
    HMODULE core = GetModuleHandleA("core.dll");
    if (!core) return;

    // MTA no exporta Lua, así que se localiza por RVA (IDA / x32dbg)
    // y se cae a los nombres por si el build los expone
    for (const char* mod : { "lua5.1.dll", "lua51.dll", "core.dll" })
    {
        HMODULE m = GetModuleHandleA(mod);
        if (!m) continue;
        g_pcall = (pcall_t)GetProcAddress(m, "lua_pcall");
        g_load  = (load_t)GetProcAddress(m, "luaL_loadbuffer");
        if (g_pcall && g_load) break;
    }
    if (!g_pcall || !g_load) return;

    MH_CreateHook(g_pcall, &hk_pcall, (void**)&g_pcall);
    MH_EnableHook(g_pcall);
}

void Lua_Run(const char* code)
{
    if (!g_state || !g_load || !g_pcall)
    {
        strcpy_s(g_cfg.luaOut, "Lua no inicializado (falta lua_pcall / estado)");
        return;
    }
    if (g_load(g_state, code, strlen(code), "cheat") != 0)
    {
        const char** err = (const char**)(*(uintptr_t*)((uintptr_t)g_state + 0x18) + 0x0);
        sprintf_s(g_cfg.luaOut, "Error de sintaxis");
        return;
    }
    if (g_pcall(g_state, 0, 0, 0) != 0)
    {
        const char* msg = *(const char**)(*(uintptr_t*)g_state + 0x00);
        sprintf_s(g_cfg.luaOut, "Error: %s", msg ? msg : "?");
        return;
    }
    strcpy_s(g_cfg.luaOut, "OK");
}