#include "pch.h"
#include "Menu.h"
#include "Widgets.h"
#include "../Cheats.h"

MenuState g_menu;

static void Tab(const char* label, int idx, ImVec2 pos)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(pos);
    ImVec2 sz(ImGui::CalcTextSize(label).x + 20.f, 30.f);
    if (ImGui::InvisibleButton(label, sz)) g_menu.tab = idx;

    bool on = g_menu.tab == idx;
    dl->AddText(th::F.reg, 15.f, { pos.x + 10, pos.y + 6 },
                on ? th::Text : th::Muted, label);
    if (on)
        dl->AddRectFilled({ pos.x + 10, pos.y + 26 }, { pos.x + sz.x - 10, pos.y + 28 }, th::Accent, 1.f);
}

static void DrawColumnPlayers(ImVec2 o, float w)
{
    ImGui::SetCursorScreenPos(o);
    ImGui::BeginChild("##colA", { w, 0 }, false);

    wd::Header("Drawing");
    wd::Row("Enable Players Visuals", "ev", &g_cfg.espEnable, &g_cfg.colBox, &g_cfg.colBox2);

    ImGui::Dummy({ 0, 2 });
    ImGui::TextColored(ImColor(th::Muted), "Draw Distance");
    ImGui::SetNextItemWidth(w - 16);
    ImGui::SliderFloat("##dist", &g_cfg.espDistance, 50.f, 1000.f, "%.0f M");

    ImGui::Dummy({ 0, 4 });
    wd::Row("Box ESP Filled", "bf", &g_cfg.espBoxFilled);
    wd::Row("Box ESP",        "bx", &g_cfg.espBox,    &g_cfg.colBox, &g_cfg.colBox2);
    wd::Row("Skeleton",       "sk", &g_cfg.espSkeleton, &g_cfg.colSkel, &g_cfg.colSkel2);
    wd::Row("Player Name",    "nm", &g_cfg.espName,   &g_cfg.colName, &g_cfg.colName2);
    wd::Row("Weapon",         "wp", &g_cfg.espWeapon, &g_cfg.colWpn, &g_cfg.colWpn2);
    wd::Row("Distance",       "ds", &g_cfg.espDist,   &g_cfg.colDist, &g_cfg.colDist2);
    wd::Row("Snapline",       "sl", &g_cfg.espSnapline, &g_cfg.colSnap, &g_cfg.colSnap2);
    wd::Row("Chinese Hats",   "ch", &g_cfg.espHats,   &g_cfg.colHat, &g_cfg.colHat2);
    wd::Row("Health Bar",     "hb", &g_cfg.espHealth);
    wd::Row("Armor Bar",      "ab", &g_cfg.espArmor);

    ImGui::EndChild();
}

static void DrawColumnFilters(ImVec2 o, float w)
{
    ImGui::SetCursorScreenPos(o);
    ImGui::BeginChild("##colB", { w, 0 }, false);

    wd::Header("Filters");
    wd::Row("Draw Friends",       "fr", &g_cfg.fFriends, &g_cfg.colFriend, &g_cfg.colFriend2);
    wd::Row("Draw Dead Players",  "dp", &g_cfg.fDead);
    wd::Row("Draw Local Player",  "lp", &g_cfg.fLocal);
    ImGui::TextColored(ImColor(th::Muted), "All Dimensions");

    ImGui::EndChild();
}

static void DrawAim(ImVec2 o, float w)
{
    ImGui::SetCursorScreenPos(o);
    ImGui::BeginChild("##aim", { w, 0 }, false);

    wd::Header("Aimbot");
    wd::Row("Enable Aimbot",   "en", &g_cfg.aimEnable);
    wd::Row("Draw FOV",        "fv", &g_cfg.aimDrawFov);
    ImGui::Dummy({ 0, 2 });
    ImGui::TextColored(ImColor(th::Muted), "FOV (px)");
    ImGui::SetNextItemWidth(w - 16);
    ImGui::SliderFloat("##fov", &g_cfg.aimFov, 20.f, 600.f, "%.0f");

    ImGui::TextColored(ImColor(th::Muted), "Smooth");
    ImGui::SetNextItemWidth(w - 16);
    ImGui::SliderFloat("##sm", &g_cfg.aimSmooth, 1.f, 25.f, "%.1f");

    wd::Row("Visible Only",    "vo", &g_cfg.aimVisible);
    wd::Row("Target Vehicles", "tv", &g_cfg.aimVehicles);

    ImGui::Dummy({ 0, 2 });
    ImGui::TextColored(ImColor(th::Muted), "Bone");
    const char* bones[] = { "Head", "Neck", "Pelvis" };
    ImGui::SetNextItemWidth(w - 16);
    ImGui::Combo("##bone", &g_cfg.aimBone, bones, 3);

    ImGui::EndChild();
}

static void DrawLua(ImVec2 o, float w)
{
    ImGui::SetCursorScreenPos(o);
    ImGui::BeginChild("##lua", { w, 0 }, false);

    wd::Header("Lua Executor");
    ImGui::InputTextMultiline("##src", g_cfg.luaBuf, sizeof(g_cfg.luaBuf), { w - 16, 170.f });
    if (ImGui::Button("Run (F5)", { 90, 26 })) Lua_Run(g_cfg.luaBuf);
    ImGui::SameLine();
    if (ImGui::Button("Clear", { 70, 26 })) g_cfg.luaBuf[0] = 0;

    ImGui::Dummy({ 0, 4 });
    ImGui::TextColored(ImColor(th::Muted), "Output");
    ImGui::BeginChild("##out", { w - 16, 90 }, true);
    ImGui::TextUnformatted(g_cfg.luaOut);
    ImGui::EndChild();

    ImGui::EndChild();
}

void Menu_Render()
{
    if (!g_menu.open) return;

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = true;

    ImGui::SetNextWindowPos(g_menu.pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ 760, 470 }, ImGuiCond_Always);
    ImGui::Begin("##main", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();

    // fondo + borde
    dl->AddRectFilled(wp, wp + ws, th::Bg, 10.f);
    dl->AddRect(wp, wp + ws, th::Border, 10.f);

    // rail
    dl->AddRectFilled(wp, { wp.x + 52, wp.y + ws.y }, th::Rail, 10.f, ImDrawFlags_RoundCornersLeft);
    dl->AddLine({ wp.x + 52, wp.y }, { wp.x + 52, wp.y + ws.y }, th::Border);

    // drag desde el rail
    ImGui::SetCursorScreenPos({ wp.x, wp.y });
    ImGui::InvisibleButton("##drag", { 52, 40 });
    if (ImGui::IsItemActive())
        g_menu.pos = ImVec2(g_menu.pos.x + io.MouseDelta.x, g_menu.pos.y + io.MouseDelta.y);

    wd::Icon_(dl, Icon::Logo, { wp.x + 26, wp.y + 30 }, 30.f, 0);

    struct { Icon ic; int id; } rail[] = {
        { Icon::Person, 0 }, { Icon::Aim, 1 }, { Icon::Eye, 2 },
        { Icon::Weapon, 3 }, { Icon::Car, 4 }, { Icon::Lua, 5 }
    };
    float ry = wp.y + 78;
    for (auto& r : rail)
    {
        if (wd::Rail(dl, ("##r" + std::to_string(r.id)).c_str(), r.ic, g_menu.tab == r.id, { wp.x, ry }))
            g_menu.tab = r.id;
        ry += 50;
    }
    wd::Rail(dl, "##set", Icon::Gear, g_menu.tab == 6, { wp.x, wp.y + ws.y - 56 });

    // barra superior
    float cx = wp.x + 72;
    Tab("Players", 0, { cx, wp.y + 14 });      cx += 78;
    Tab("Peds",    3, { cx, wp.y + 14 });      cx += 62;
    Tab("Vehicles",4, { cx, wp.y + 14 });

    ImGui::SetCursorScreenPos({ wp.x + 300, wp.y + 16 });
    ImGui::SetNextItemWidth(190);
    ImGui::InputTextWithHint("##search", "Search Functions", g_menu.search, sizeof(g_menu.search));

    wd::Icon_(dl, Icon::Gear, { wp.x + ws.x - 26, wp.y + 30 }, 24.f, th::Muted);

    // cuerpo
    float bodyY = wp.y + 60;
    float colW  = (ws.x - 100.f) * 0.5f;

    if (g_menu.search[0])
    {
        ImGui::SetCursorScreenPos({ wp.x + 72, bodyY });
        ImGui::BeginChild("##sr", { ws.x - 92, ws.y - 70 }, false);
        wd::Header("Results");
        if (strstr("Enable Players Visuals", g_menu.search)) wd::Row("Enable Players Visuals", "s1", &g_cfg.espEnable);
        if (strstr("Box ESP", g_menu.search))               wd::Row("Box ESP", "s2", &g_cfg.espBox);
        if (strstr("Skeleton", g_menu.search))              wd::Row("Skeleton", "s3", &g_cfg.espSkeleton);
        if (strstr("Player Name", g_menu.search))           wd::Row("Player Name", "s4", &g_cfg.espName);
        if (strstr("Weapon", g_menu.search))                wd::Row("Weapon", "s5", &g_cfg.espWeapon);
        if (strstr("Distance", g_menu.search))              wd::Row("Distance", "s6", &g_cfg.espDist);
        ImGui::EndChild();
    }
    else switch (g_menu.tab)
    {
    case 0:
    case 3: DrawColumnPlayers({ wp.x + 72, bodyY }, colW);
            DrawColumnFilters({ wp.x + 84 + colW, bodyY }, colW); break;
    case 1: DrawAim({ wp.x + 72, bodyY }, ws.x - 92); break;
    case 5: DrawLua({ wp.x + 72, bodyY }, ws.x - 92); break;
    default:
        ImGui::SetCursorScreenPos({ wp.x + 72, bodyY });
        ImGui::TextColored(ImColor(th::Muted), "Proximamente");
    }

    ImGui::End();

    // HUD superior derecha
    ImGui::SetNextWindowPos({ io.DisplaySize.x - 340, 12 });
    ImGui::Begin("##hud", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoBackground);
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(), IM_COL32(10, 11, 14, 220), 6.f);
    ImGui::TextColored(ImColor(th::Text), "NEXIDA CC BASIC");
    ImGui::SameLine(); ImGui::TextColored(ImColor(th::Muted), "|");
    ImGui::SameLine(); ImGui::TextColored(ImColor(th::Muted), "Guest  |  %.1f FPS", io.Framerate);
    ImGui::End();

    Menu_Preview();
}