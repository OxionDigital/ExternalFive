#pragma once
#include "imgui.h"

struct MenuState { bool open = true; int tab = 0; char search[64]{}; ImVec2 pos{ 300, 120 }; };
extern MenuState g_menu;

void Menu_Render();
void Menu_Preview();