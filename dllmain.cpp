#define _CRT_SECURE_NO_WARNINGS

#pragma region headers
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>

#include "esp.h"
#include "scanner.h"
#include "menu.h"
#pragma endregion headers

#pragma region defs

ID3D11RenderTargetView* rendertarget;
ID3D11DeviceContext* context;
ID3D11Device* device;
HRESULT(*present_original)(IDXGISwapChain* swapchain, UINT sync, UINT flags) = nullptr;
int X, Y;
HWND hwnd;
bool beingcalled = false;
bool LoadImGui = true;
bool is_flying = false;
Menu gui;

uint64_t client_entities;
uintptr_t client_input_ptr = NULL;
uintptr_t buffer_list = NULL;

#pragma endregion defs

#pragma region menu
Menu::Menu(void)
{
    this->Visible = true;
}

void Menu::Init_Menu(LPCSTR Title, int x, int y)
{
    this->Is_Ready = true;
    this->sMenu.Title = Title;
    this->sMenu.x = x;
    this->sMenu.y = y;
}

void Menu::AddFolder(LPCSTR Name, int* Pointer, int limit)
{
    sOptions[this->Items].Name = Name;
    sOptions[this->Items].Function = Pointer;
    sOptions[this->Items].Type = T_FOLDER;
    this->Items++;
}

void Menu::AddOption(LPCSTR Name, int* Pointer, int* Folder, int type, int lower_limit = 0, int upper_limit = 2, int increase_by = 1)
{
    if (*Folder == 0)
        return;
    sOptions[this->Items].Name = Name;
    sOptions[this->Items].Function = Pointer;
    sOptions[this->Items].Type = type;
    sOptions[this->Items].upper_limit = upper_limit - 1;
    sOptions[this->Items].lower_limit = lower_limit;
    sOptions[this->Items].increase_by = increase_by;
    this->Items++;
}

void Menu::Navigation()
{
    if (!LocalPlayer.Player)
    {
        return;
    }

    if (get_key(KeyCode::Insert) & 1)
        this->Visible = !this->Visible;

    if (!this->Visible)
        return;

    int value = 0;

    if (get_key(KeyCode::DownArrow) & 1)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        this->Cur_Pos++;
        if (sOptions[this->Cur_Pos].Name == 0)
            this->Cur_Pos--;
    }

    if (get_key(KeyCode::UpArrow) & 1) //if (LocalPlayer.is_key_down(SPRINT_DUCK_FORWARD) & 1)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        this->Cur_Pos--;
        if (this->Cur_Pos == -1)
            this->Cur_Pos++;
    }

    else if (get_key(KeyCode::RightArrow)) //if (LocalPlayer.is_key_down(SPRINT_DUCK_RIGHT) & 1)
    {
        //std::cout << "VK_RIGHT: | " << this->Cur_Pos << " | " << *sOptions[this->Cur_Pos].Function << " | " << sOptions[this->Cur_Pos].lower_limit << " | " << sOptions[this->Cur_Pos].upper_limit << " | " << sOptions[this->Cur_Pos].increase_by << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        if (*sOptions[this->Cur_Pos].Function <= sOptions[this->Cur_Pos].upper_limit)
        {
            value++;
        }
    }

    else if (get_key(KeyCode::LeftArrow)) //if (LocalPlayer.is_key_down(SPRINT_DUCK_LEFT) & 1)
    {
        //std::cout << "VK_LEFT: | " << this->Cur_Pos << " | " << *sOptions[this->Cur_Pos].Function << " | " << sOptions[this->Cur_Pos].lower_limit << " | " << sOptions[this->Cur_Pos].increase_by << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        if (*sOptions[this->Cur_Pos].Function > sOptions[this->Cur_Pos].lower_limit)
        {
            value--;
        }
    }

    if (value != 0)
    {
        if (sOptions[this->Cur_Pos].Type != NULL)
        {
            //std::cout << "VALUE: " << value << std::endl;
            *sOptions[this->Cur_Pos].Function += value;
            if (sOptions[this->Cur_Pos].Type == T_FOLDER)
            {
                memset(&sOptions, 0, sizeof(sOptions));
                this->Items = 0;
            }
        }
    }


}

void Menu::Draw_Menu()
{
    if (!this->Visible)
        return;

    ImDrawList* list = ImGui::GetOverlayDrawList();
    //this->DrawText(this->sMenu.Title, 14, sMenu.x + 10, sMenu.y, this->Color_Font);
    for (int i = 0; i < this->Items; i++)
    {
        if (this->sOptions[i].Type == T_OPTION)
        {
            if (*this->sOptions[i].Function)
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(0, 255, 60), "Enabled");
                //DrawString("On", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"On");
                //Render::String(vec2_t{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"On");
                //this->DrawText(L"On", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_On);
            }
            else
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(255, 0, 0), "Disabled");
                //DrawString("Off", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"Off");
                //this->DrawText(L"Off", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Off);
            }
        }
        if (this->sOptions[i].Type == T_FOLDER)
        {
            if (*this->sOptions[i].Function)
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(255, 255, 255), "Show");
                //DrawString("Opened", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"Opened");
                //this->DrawText(L"Opend", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Folder);
            }
            else
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(0, 0, 0), "Hide");
                //DrawString("Closed", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"Closed");
                //this->DrawText(L"Closed", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Folder);
            }
        }
        if (this->sOptions[i].Type == T_SPACER)
        {
            if (*this->sOptions[i].Function)
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(0, 255, 60), "          ");
                //DrawString("On", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"On");
                //Render::String(vec2_t{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"On");
                //this->DrawText(L"On", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_On);
            }
            else
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(255, 0, 0), "           ");
                //DrawString("Off", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"Off");
                //this->DrawText(L"Off", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Off);
            }
        }
        if (this->sOptions[i].Type == T_BOOL)
        {
            if (*this->sOptions[i].Function)
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(0, 255, 60), "Enabled");
                //DrawString("True", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)), D2D1::ColorF::Green);
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"True", D2D1::ColorF::Green);
                //this->DrawText(L"Opend", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Folder);
            }
            else
            {
                list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(255, 0, 0), "Disabled");
                //DrawString("False", 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)), D2D1::ColorF::Red);
                //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, L"False", D2D1::ColorF::Red);
                //this->DrawText(L"Closed", 12, sMenu.x + 150, sMenu.y + LineH * (i + 2), this->Color_Folder);
            }
        }
        if (this->sOptions[i].Type == T_INT)
        {
            auto sval = std::to_string((*this->sOptions[i].Function));
            //std::wstring wide_string = std::wstring(sval.begin(), sval.end());
            //const wchar_t* result = wide_string.c_str();
            //i_renderer.helper->string(Vector2{ (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)) }, result);
            list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(255, 255, 255), sval.c_str());
            //DrawWString(result, 15, (float)(sMenu.x + 150), (float)(sMenu.y + LineH * (i + 2)));
        }
        ImColor Color = ImColor(250, 250, 250);
        if (this->Cur_Pos == i)
            Color = ImColor(250, 0, 200);
        else if (this->sOptions[i].Type == T_FOLDER)
            Color = ImColor(0, 250, 250);

        //this->DrawText(this->sOptions[i].Name, 12, sMenu.x + 5, sMenu.y + LineH * (i + 2), Color);
        if (this->sOptions[i].Type == T_FOLDER)
            //list->AddText(ImVec2(sMenu.x + 150, sMenu.y + LineH * (i + 2)), ImColor(0, 0, 0), this->sOptions[i].Name);
            list->AddText(ImVec2((float)(sMenu.x + 15), (float)(sMenu.y + LineH * (i + 2))), Color, this->sOptions[i].Name);
        else
            list->AddText(ImVec2((sMenu.x + 30), (sMenu.y + LineH * (i + 2))), Color, this->sOptions[i].Name);
    }
}

void SetupMenu()
{
    //Player ESP
    //gui.AddFolder("Player ESP", &settings::folder, T_FOLDER);

    gui.AddFolder("Display Menu", &settings::folder, T_FOLDER);

    gui.AddOption("Crosshair", &settings::aimbot::crosshair, &settings::folder, T_BOOL);
    gui.AddOption("Aimbot", &settings::aimbot::enabled, &settings::folder, T_BOOL);
    gui.AddOption("Shoot Head", &settings::aimbot::shoot_head, &settings::folder, T_BOOL);
    gui.AddOption("Silent Aim (buggy)", &settings::aimbot::silent, &settings::folder, T_BOOL);
    gui.AddOption("Use Mouse", &settings::aimbot::use_mouse, &settings::folder, T_BOOL);
    gui.AddOption("FOV Circle", &settings::aimbot::fov, &settings::folder, T_INT, 0, 1000, 5);
    gui.AddOption("Max Distance", &settings::aimbot::max_distance, &settings::folder, T_INT, 10, 1000, 10);

    gui.AddOption("             ", &settings::spacer, &settings::folder, T_SPACER);

    gui.AddOption("Player Boxes", &settings::Visuals::boxes, &settings::folder, T_BOOL);
   // gui.AddOption("Player Chams", &settings::Visuals::chams, &settings::folder, T_BOOL);
    gui.AddOption("Player Cornered", &settings::Visuals::boxescorner, &settings::folder, T_BOOL);
    gui.AddOption("Player Skeleton", &settings::Visuals::skeleton, &settings::folder, T_BOOL);
    gui.AddOption("Player Name", &settings::Visuals::name, &settings::folder, T_BOOL);
    gui.AddOption("Player Health", &settings::Visuals::health, &settings::folder, T_BOOL);
    gui.AddOption("Player Distance", &settings::Visuals::distance, &settings::folder, T_BOOL);
    gui.AddOption("Player Held Item", &settings::Visuals::held_item, &settings::folder, T_BOOL);
    gui.AddOption("Player Hotbar", &settings::Visuals::hotbar, &settings::folder, T_BOOL);
    gui.AddOption("Max Distance", &settings::Visuals::max_distance, &settings::folder, T_INT, 10, 1000, 10);

    gui.AddOption("             ", &settings::spacer, &settings::folder, T_SPACER);

    gui.AddOption("Stone Node", &settings::ore::stone, &settings::folder, T_BOOL);
    gui.AddOption("Metal Node", &settings::ore::metal, &settings::folder, T_BOOL);
    gui.AddOption("Sulfur Node", &settings::ore::sulfur, &settings::folder, T_BOOL);
    gui.AddOption("Distance", &settings::ore::distance, &settings::folder, T_BOOL);

    gui.AddOption("             ", &settings::spacer, &settings::folder, T_SPACER);

    gui.AddOption("Hemp Collectable", &settings::collectable::hemp, &settings::folder, T_BOOL);
    gui.AddOption("Node Collectable", &settings::collectable::collectables, &settings::folder, T_BOOL);
    gui.AddOption("Food Collectable", &settings::collectable::food, &settings::folder, T_BOOL);
    gui.AddOption("Collectabled Distance", &settings::collectable::distance, &settings::folder, T_BOOL);

    gui.AddOption("Stashes", &settings::otheresp::stashes::enabled, &settings::folder, T_BOOL);
    if (settings::otheresp::stashes::enabled)
    {
        settings::otheresp::stashes::max_distance = true;
    }

    gui.AddOption("             ", &settings::spacer, &settings::folder, T_SPACER);

    gui.AddOption("Dropped Items", &settings::dropped::show_all, &settings::folder, T_BOOL);
    gui.AddOption("Dropped Weapons", &settings::dropped::show_weapons, &settings::folder, T_BOOL);
    gui.AddOption("Dropped Distance", &settings::dropped::distance, &settings::folder, T_BOOL);

    gui.AddOption("             ", &settings::spacer, &settings::folder, T_SPACER);

    gui.AddOption("Spider", &settings::misc::spider, &settings::folder, T_BOOL);
    gui.AddOption("Admin Commands", &settings::misc::debug_cam, &settings::folder, T_BOOL);
    gui.AddOption("Always Eoka", &settings::misc::always_eoka, &settings::folder, T_BOOL);
    gui.AddOption("All Automatic", &settings::misc::automatic, &settings::folder, T_BOOL);
    gui.AddOption("Always Day", &settings::misc::always_day, &settings::folder, T_BOOL);
    gui.AddOption("Flight (buggy)", &settings::misc::water_walk, &settings::folder, T_BOOL);
    gui.AddOption("Rapid Fire", &settings::misc::rapid_fire, &settings::folder, T_BOOL);
    gui.AddOption("No Recoil", &settings::misc::no_recoil, &settings::folder, T_BOOL);
    gui.AddOption("No Spread", &settings::misc::no_spread, &settings::folder, T_BOOL);
    gui.AddOption("Fast Bow", &settings::misc::fast_bow, &settings::folder, T_BOOL);
    gui.AddOption("LongNeck UP", &settings::misc::longneck, &settings::folder, T_BOOL);

}

bool Menu::IsReady()
{
    if (this->Items)
        return true;
    return false;
}

#pragma endregion menu

uintptr_t hook_virtual_function(const char* classname, const char* function_to_hook, void* our_func, const char* name_space = ("")) {
    uintptr_t search = *reinterpret_cast<uintptr_t*>(il2cpp::method(classname, function_to_hook, -1, (""), name_space));
    uintptr_t table = il2cpp::init_class(classname, name_space);

    if (search == (uintptr_t)our_func)
    {
        std::cout << "search == our_func" << std::endl;
        return (uintptr_t)our_func;
    }

    for (uintptr_t i = table; i <= table + 0x1500; i += 0x1) {
        uintptr_t addr = *reinterpret_cast<uintptr_t*>(i);
        if (addr == search)
        {
            std::cout << "addr == search" << std::endl;
            *reinterpret_cast<uintptr_t*>(i) = (uintptr_t)our_func;
            std::cout << function_to_hook << " successfully hooked!" << std::endl;
            return addr;
        }
    }
}

#pragma region hooked
bool hook_baseprojectile_can_attack(uintptr_t base_projectile)
{
    std::cout << "Can Attack Hook!" << std::endl;
    if (settings::misc::run_and_gun)
        return true;

    return orig_baseprojectile_can_attack(base_projectile);
}

void hook_baseplayer_block_sprint(uintptr_t base_player, float duration)
{
    std::cout << "Block Sprint Hook!" << std::endl;
    if (settings::misc::run_and_gun)
        return;

    return orig_baseplayer_block_sprint(base_player, duration);
}

void OnGUI(uintptr_t rcx)
{
    std::cout << "OnGUI!" << std::endl;
}

Vector3 hook_aimconeutil_getmodifiedaimconedirection(float aimCone, Vector3 inputVec, bool anywhereInside)
{
    std::cout << "Called GetModifiedAimConeDirection" << std::endl;
    closestPlayer = get_closest_to_crosshair(buffer_list);
    if (LocalPlayer.Player != NULL && closestPlayer.Player != NULL)
    {
        std::cout << "Silent Aim" << std::endl;
        auto localpos = LocalPlayer.GetBonePosition(spine1);
        auto targetpos = closestPlayer.GetBonePosition(spine1);

        Vector3 direction = Vector3(targetpos.x - localpos.x, targetpos.y - localpos.y, targetpos.z - localpos.z);

        return orig_aimconeutil_getmodifiedaimconedirection(aimCone, direction, anywhereInside);
    }
    else
    {
        return orig_aimconeutil_getmodifiedaimconedirection(aimCone, inputVec, anywhereInside);
    }
}

void hook_playerwalkmovement_client_input(uintptr_t player_walk_movement, uintptr_t input_state, uintptr_t model_state)
{
    std::cout << "Calling Orig PWM CI" << std::endl;
    orig_playerwalkmovement_client_input(player_walk_movement, input_state, model_state);
    std::cout << "Called Orig PWM CI" << std::endl;
    set_sprinting(model_state, true);
    is_flying = mem.Read<bool>(model_state + OFFSET_PWM_FLYING);

    if (get_key(KeyCode::Z))
    {
        std::cout << "Z Key Held!" << std::endl;
        set_onLadder(model_state, true);
    }

    int flags = *reinterpret_cast<int*>((uintptr_t)model_state + 0x24);
    flags &= ~(int)64;

    *reinterpret_cast<int*>((uintptr_t)model_state + 0x24) = flags;
    std::cout << "Returning Hooked PWM CI" << std::endl;
}

void hook_baseplayer_client_input(uintptr_t baseplayer, uintptr_t state)
{
    BasePlayer player;
    player.Player = baseplayer;
    player.ObjectClass = NULL;

    if (!buffer_list)
        return;

    ModelState model_state = player.GetModelState();

    set_sprinting(model_state.state, true);
    is_flying = mem.Read<bool>(model_state.state + OFFSET_PWM_FLYING);

    if (get_key(KeyCode::Z))
    {
        std::cout << "Z Key Held!" << std::endl;
        set_onLadder(model_state.state, true);
    }

    model_state.remove_flag(64);

    closestPlayer = get_closest_to_crosshair(buffer_list);

    if (settings::aimbot::enabled)
    {
        if (closestPlayer.Player != NULL && LocalPlayer.Player != NULL)
        {
            if (settings::aimbot::silent)
            {
                Vector3 EnemyPos = Prediction(LocalPosition, closestPlayer);
                Vector3 LocalPos = LocalPlayer.GetBonePosition(neck);
                auto rotation = QuaternionLookRotation(EnemyPos - LocalPos, Vector3(0, 1, 0));
                LocalPlayer.GetPlayerEyes().SetBodyRotations(rotation);
            }
            else if (LocalPlayer.is_key_down(FIRE_SECONDARY) && !settings::aimbot::silent)
            {
                Vector2 ScreenPos;
                Vector3 Pos = Prediction(LocalPosition, closestPlayer);

                auto distance = Math::Calc3D_Dist(LocalPosition, Pos);

                if (distance <= settings::aimbot::max_distance)
                {
                    if (WorldToScreen(Pos, ScreenPos))
                    {
                        auto fov = distance_cursor(ScreenPos);
                        if (fov <= settings::aimbot::fov)
                        {
                            auto LocalPos = LocalPlayer.GetBonePosition(neck);

                            auto AimOffset = (Math::CalcAngle(LocalPos, Pos) - LocalPlayer.GetPlayerInput().GetViewAngles());

                            auto AngleToAim = LocalPlayer.GetPlayerInput().GetViewAngles() + AimOffset;

                            Normalize(AngleToAim.y, AngleToAim.x);

                            LocalPlayer.GetPlayerInput().SetViewAngles(AngleToAim);
                        }
                    }
                }
            }
        }
    }

    orig_baseplayer_client_input(baseplayer, state);
}

HRESULT present_hooked(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
    if (!device)
    {
        ID3D11Texture2D* renderTarget = 0;
        ID3D11Texture2D* backBuffer = 0;
        D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
        //std::cout << "Getting Device and ImmediateContext" << std::endl;
        swapchain->GetDevice(__uuidof(device), (PVOID*)&device);
        device->GetImmediateContext(&context);
        //std::cout << "Getting Buffer and Releasing" << std::endl;
        swapchain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
        device->CreateRenderTargetView(renderTarget, nullptr, &rendertarget);
        renderTarget->Release();
        //std::cout << "Getting Buffer and Desc" << std::endl;
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
        backBuffer->GetDesc(&backBufferDesc);

        X = backBufferDesc.Width;
        Y = backBufferDesc.Height;
        //std::cout << "Releasing backbuf" << std::endl;
        backBuffer->Release();

        if (!hwnd)
        {
            //Change window name and class for the specific game, to check use spy++ its in the tool tab of visual studio :D
            //For loading a non steam game for example rogue company press add a game -> add a non-steam game
            hwnd = FindWindowW(L"UnityWndClass", nullptr);
            //if not found try to use foreground window?
            if (!hwnd)
                hwnd = GetForegroundWindow();

            RECT rect;
            if (GetWindowRect(hwnd, &rect))
            {
                Globals::rWidth = rect.right - rect.left;
                Globals::rHeight = rect.bottom - rect.top;
            }
        }


        if (LoadImGui)
        {
            //std::cout << "adawdawdad" << std::endl;
            ImGui_ImplDX11_Init(hwnd, device, context);
            ImGui_ImplDX11_CreateDeviceObjects();

            //std::cout << "adawdawdad" << std::endl;
            ImGuiStyle* style = &ImGui::GetStyle();
            style->WindowPadding = ImVec2(15, 15);
            style->WindowRounding = .0f;
            style->FramePadding = ImVec2(5, 5);
            style->FrameRounding = .0f;
            style->ItemSpacing = ImVec2(12, 8);
            style->ItemInnerSpacing = ImVec2(8, 6);
            style->IndentSpacing = 25.0f;
            style->ScrollbarSize = 10.0f;
            style->ScrollbarRounding = 9.0f;
            style->GrabMinSize = 5.0f;
            style->GrabRounding = .0f;

            style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
            style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
            style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
            style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
            style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
            style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
            style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
            style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
            style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
            style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
            style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
            style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
            style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
            style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
            ImGui::SetNextWindowSize(ImVec2(625, 410));
        }
        std::cout << "Initializing Il2CPP!" << std::endl;
        test::il2cpp::InitIL();
        il2cpp::init();
        std::cout << "Il2CPP Initialized!" << std::endl;

        orig_baseplayer_client_input = reinterpret_cast<void (*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("BasePlayer"), ("ClientInput"), -1, (""), (""))));
        orig_baseplayer_block_sprint = reinterpret_cast<void (*)(uintptr_t, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("BasePlayer"), ("BlockSprint"), -1, (""), (""))));
        orig_baseprojectile_can_attack = reinterpret_cast<bool (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("BaseProjectile"), ("CanAttack"), -1, (""), (""))));

        hook_virtual_function("BasePlayer", "ClientInput", &hook_baseplayer_client_input);
        hook_virtual_function("BasePlayer", "BlockSprint", &hook_baseplayer_block_sprint);
        hook_virtual_function("BaseProjectile", "CanAttack", &hook_baseprojectile_can_attack);

        get_key = reinterpret_cast<bool(*)(KeyCode)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("Input"), ("GetKeyInt"), 1, (""), ("UnityEngine"))));
        line_of_sight = reinterpret_cast<bool(*)(Vector3, Vector3, Layers, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("GamePhysics"), ("LineOfSight"), -1, (""), (""))));
        set_sprinting = reinterpret_cast<void (*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("ModelState"), ("set_sprinting"), -1, (""), (""))));
        set_flying = reinterpret_cast<void (*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("ModelState"), ("set_flying"), 1, (""), (""))));
        set_onLadder = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(("ModelState"), ("set_onLadder"), 1, (""), (""))));
    }

    if (!beingcalled)
    {
        beingcalled = true;

        //std::cout << "adawdawdad" << std::endl;
        context->OMSetRenderTargets(1, &rendertarget, nullptr);

        ImGui_ImplDX11_NewFrame();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin("##scene", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
        ImGuiIO& IO = ImGui::GetIO();
        ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(IO.DisplaySize.x, IO.DisplaySize.y), ImGuiCond_Always);
        ImGuiWindow& window = *ImGui::GetCurrentWindow();

        /*
        int id = 1;
        ImGui::Checkbox("- ESP Enabled", &settings::Visuals::enabled);
        ImGui::Checkbox("- Names Enabled", &settings::Visuals::name);
        ImGui::Checkbox("- Health Enabled", &settings::Visuals::health);
        ImGui::Checkbox("- Distance Enabled", &settings::Visuals::distance);
        ImGui::Checkbox("- Boxes Enabled", &settings::Visuals::boxes);
        ImGui::PushID(id);
        ImGui::Text("- Max Distance");
        ImGui::SameLine();
        ImGui::SliderInt("", &settings::Visuals::max_distance, 1, 1000);
        ImGui::PopID();
        id++;
        */
        //auto drawlist = ImGui::GetOverlayDrawList();

        //window.DrawList->AddText(ImVec2(10, 10), ImGui::GetColorU32(ImVec4(1.f, 0.f, 0.f, 1.f)), "Rust Internal");

        if (!gui.IsReady())
        {
            SetupMenu();
        }
        else
        {
            if (gui.Visible)
            {
                gui.Draw_Menu();
            }
            gui.Navigation();
        }

        //std::cout << "GOM" << std::endl;
        GOM = mem.Read((uint64_t)GetModuleHandle(L"UnityPlayer.dll") + 0x17C1F18);
        if (!GOM || GOM == NULL)
        {
            std::cout << "!GOM" << std::endl;
            return present_original(swapchain, sync, flags);;
        }

        //std::cout << "Tagged Object" << std::endl;
        auto tagged_objects = mem.Read(GOM + 0x8);

        if (!tagged_objects)
        {
            std::cout << "!tagged_objects" << std::endl;
            return present_original(swapchain, sync, flags);;
        }

        //std::cout << "Game Object" << std::endl;
        auto game_object = mem.Read(tagged_objects + 0x10);

        if (!game_object)
        {
            std::cout << "!game_object" << std::endl;
            return present_original(swapchain, sync, flags);;
        }

        //std::cout << "Object Class" << std::endl;
        auto object_class = mem.Read(game_object + 0x30);

        if (!object_class)
        {
            std::cout << "!object_class" << std::endl;
            return present_original(swapchain, sync, flags);;
        }

        //std::cout << "Ent" << std::endl;
        auto ent = mem.Read(object_class + 0x18);

        if (!ent)
        {
            std::cout << "!ent" << std::endl;
            return present_original(swapchain, sync, flags);;
        }

        //std::cout << "ViewMatrix" << std::endl;
        ViewMatrix = mem.Read<Matrix4x4>(ent + 0xDC);

        //std::cout << "BaseNetworkable" << std::endl;
        BaseNetworkable = mem.Read((uint64_t)GetModuleHandle(L"GameAssembly.dll") + 0x3234020); //2FC4010

        //uint64_t client_entities = il2cpp::value(("BaseNetworkable"), ("clientEntities"), false);
        //printf("client_entities: 0x%x\n", client_entities);
        /*

        if (!client_entities)
        {
            std::cout << "Updating Client Entities!" << std::endl;
            //get_client_entities();
            std::cout << "Updated Client Entities!" << std::endl;
        }

        if (!client_entities)
        {
            std::cout << "!client_entities" << std::endl;
            return;
        }

        */

        if (!BaseNetworkable || BaseNetworkable == NULL)
        {
            std::cout << "!BaseNetworkable" << std::endl;
            return present_original(swapchain, sync, flags);;
        }


        //std::cout << "Unk1" << std::endl;
        auto unk1 = mem.Read(BaseNetworkable + 0xB8);
        if (!unk1)
        {
            std::cout << "!unk1" << std::endl;
            //get_client_entities();
            return present_original(swapchain, sync, flags);;
        }

        //client_entities = mem.Read(unk1);

        /*
        if (!client_entities || client_entities == 0xff000000)
        {
            //std::cout << "!client_entities" << std::endl;
            //get_client_entities();
            return;
        }
        */

        auto c_ents = mem.Read(unk1);
        if (!c_ents)
        {
            return present_original(swapchain, sync, flags);;
        }
        //std::cout << "entity_realm" << std::endl;
        //std::cout << "c_ent: 0x" << c_ents << std::endl;
        //Sleep(1000000);
        auto entity_realm = mem.Read(c_ents + 0x10);
        if (!entity_realm)
        {
            std::cout << "!entity_realm" << std::endl;
            //get_client_entities();
            return present_original(swapchain, sync, flags);;
        }
        //printf("Entity Realm: 0x%x\n", entity_realm);

        //std::cout << "buffer_list" << std::endl;
        buffer_list = mem.Read(entity_realm + 0x28);
        if (!buffer_list)
        {
            std::cout << "!buffer_list" << std::endl;
            return present_original(swapchain, sync, flags);;
        }
        //printf("buffer_list: 0x%x\n", buffer_list);

        if (settings::aimbot::crosshair)
        {
            window.DrawList->AddLine(ImVec2(Globals::rWidth / 2, Globals::rHeight / 2 - 9), ImVec2(Globals::rWidth / 2, Globals::rHeight / 2 + 9), ImColor(255, 255, 255), 1);
            window.DrawList->AddLine(ImVec2(Globals::rWidth / 2 - 8, Globals::rHeight / 2), ImVec2(Globals::rWidth / 2 + 9, Globals::rHeight / 2), ImColor(255, 255, 255), 1);

        }

        FOV = settings::aimbot::fov;

        //std::cout << "Drawing Circle" << std::endl;
        if (settings::aimbot::fov)
        {
            window.DrawList->AddCircle(ImVec2(Globals::rWidth / 2, Globals::rHeight / 2), FOV, ImColor(255, 0, 0));
        }
        //std::cout << "Drew Circle" << std::endl;

        render_entities(buffer_list, window.DrawList);
        call_misc_hacks();

        //std::cout << "Rendered Entities!" << std::endl;
        //call_misc_hacks();


        window.DrawList->PushClipRectFullScreen();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
        ImGui::Render();
        beingcalled = false;
    }

    return present_original(swapchain, sync, flags);
}
#pragma endregion hooked

void hook(__int64 addr, __int64 func, __int64* orig)
{
    static uintptr_t hook_addr;
    if (!hook_addr)
        hook_addr = sigscan("GameOverlayRenderer64.dll", "48 ? ? ? ? 57 48 83 EC 30 33 C0");


    auto hook = ((__int64(__fastcall*)(__int64 addr, __int64 func, __int64* orig, __int64 smthng))(hook_addr));
    hook((__int64)addr, (__int64)func, orig, (__int64)1);
}

void init()
{
    if (!GetModuleHandleA("GameOverlayRenderer64.dll"))
    {
        MessageBoxA(0, "SteamOverlayRenderer64.dll Not Loaded!", "Fatal Error", MB_ICONERROR);
        exit(0);
    }

    //please dont kill me for using this 🙏
    AllocConsole();
    FILE* file;
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    uintptr_t Steam_DXGI_PresentScene = sigscan("GameOverlayRenderer64.dll", "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8");

    //std::cout << "GameOverlayRenderer64.dll!DXGIPresentSceneHooked: 0x" << std::hex << Steam_DXGI_PresentScene << std::endl;

    if (Steam_DXGI_PresentScene)
    {
        /*
        if (MH_Initialize() != MH_OK)
        {
            std::cout << "Minhook failed to initialize." << std::endl;
        }
        */

        //std::cout << "Hooking!" << std::endl;
        hook(Steam_DXGI_PresentScene, (__int64)present_hooked, (__int64*)&present_original);

        std::cout << "Successfully Injected Rust Private" << std::endl;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        init();

    return TRUE;
}

