// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include "imgui_internal.h"
#include "byte.h"
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

float dpi_scale = 1.f;

ImFont* iconfont = nullptr;
ImFont* info = nullptr;
ImFont* three = nullptr;
ImFont* watermark = nullptr;

ImVec2 pos;
ImDrawList* draw;
ImDrawList* drawbg;

#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop)


// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int tabs = 0;

void Tabs()
{
    ImGui::PushFont(info);

        ImGui::SetCursorPos(ImVec2(22, 60));

            ImGui::BeginGroup();

            if (ImGui::Tab("Legit", "A", "2", 0 == tabs))
                tabs = 0;
            ImGui::Spacing();
            if (ImGui::Tab("Players", "B", "2",1 == tabs))
                tabs = 1;
            if (ImGui::Tab("Weapons", "C", "3", 2 == tabs))
                tabs = 2;
            if (ImGui::Tab("World", "D", "0", 3 == tabs))
                tabs = 3;
            ImGui::Spacing();
            if (ImGui::Tab("Skins", "E", "0", 4 == tabs))
                tabs = 4;
            if (ImGui::Tab("Misc", "F", "0", 5 == tabs))
                tabs = 5;
            if (ImGui::Tab("Settings", "G", "0", 6 == tabs))
                tabs = 6;
 
            ImGui::EndGroup();

    ImGui::PopFont();

}

void TabElements()
{
    static bool da = false;
    static int sliderint = 4;
    static int combo = 0;

    if (tabs == 0)
    {
        ImGui::SetCursorPos(ImVec2(152, 20));
            ImGui::BeginChild("##Child0", ImVec2(600, 500));
            {
                ImGui::SetCursorPos(ImVec2(20, 55));
                ImGui::Combo("##combo1", &combo, "1\0\r2");
                ImGui::SetCursorPos(ImVec2(5, 85));
                ImGui::Checkbox("Enabled", &da);
                ImGui::SetCursorPos(ImVec2(155, 85));
                ImGui::ColorEdit4("##test_colour", accent_colour, NO_ALPHA);
            }
            ImGui::EndChild();
    }
}



int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("DirectX 9"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle* style = &ImGui::GetStyle();

    ImGui::StyleColorsDark();

    style->Alpha = 1.f;
    style->WindowRounding = 5;
    style->FramePadding = ImVec2(4, 3);
    style->WindowPadding = ImVec2(8, 8);
    style->ItemInnerSpacing = ImVec2(4, 4);
    style->ItemSpacing = ImVec2(8, 5);
    style->FrameRounding = 12;
    style->ScrollbarSize = 2.f;
    style->ScrollbarRounding = 12.f;
    style->PopupRounding = 4.f;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);


    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15);
    colors[ImGuiCol_ChildBg] = ImColor(15, 15, 15, 0);
    colors[ImGuiCol_Border] = ImVec4(255, 255, 255, 0);
    colors[ImGuiCol_FrameBg] = ImColor(18, 19, 23, 255);
    colors[ImGuiCol_FrameBgActive] = ImColor(25, 25, 33, 255);
    colors[ImGuiCol_FrameBgHovered] = ImColor(25, 25, 33, 255);
    colors[ImGuiCol_Header] = ImColor(141, 142, 144, 255);
    colors[ImGuiCol_HeaderActive] = ImColor(141, 142, 144, 255);
    colors[ImGuiCol_HeaderHovered] = ImColor(141, 142, 144, 255);
    colors[ImGuiCol_PopupBg] = ImColor(22, 22, 22, 255);
    colors[ImGuiCol_Button] = ImColor(160, 30, 30, 255);
    colors[ImGuiCol_ButtonHovered] = ImColor(190, 45, 35, 255);
    colors[ImGuiCol_ButtonActive] = ImColor(220, 60, 40, 255);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
    colors[ImGuiCol_SliderGrab] = ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
    colors[ImGuiCol_CheckMark] = ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
    colors[ImGuiCol_TitleBg] = ImColor(27, 27, 27);
    colors[ImGuiCol_TitleBgActive] = ImColor(27, 27, 27);

    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF,
        0x0400, 0x044F,
        0,
    };


    info = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/arial.ttf", 14);
    three = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/arialbd.ttf", 15.0f);
    iconfont = io.Fonts->AddFontFromMemoryTTF((void*)icon, sizeof(icon), 17, &font_config, ranges);
    watermark = io.Fonts->AddFontFromMemoryTTF((void*)watermarkicons, sizeof(watermarkicons), 17, &font_config, ranges);

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);



    // Our state
    bool show_demo_window = true, loader_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {

        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                static int x = 720 * dpi_scale, y = 450 * dpi_scale;
                pos = ImGui::GetWindowPos();
                draw = ImGui::GetWindowDrawList();

                ImGui::SetWindowSize(ImVec2(ImFloor(x * dpi_scale), ImFloor(y * dpi_scale)));

                draw = ImGui::GetWindowDrawList();
                drawbg = ImGui::GetBackgroundDrawList();

                ImVec2 screenSize = ImGui::GetIO().DisplaySize;
                ImGui::Particles(drawbg, ImVec2(0,0), screenSize, screenSize, ImVec4(), ImGui::GetTime());

                const ImVec2 pos(ImGui::GetWindowPos().x + 6, ImGui::GetWindowPos().y + 6);


                draw->AddRectFilled(pos, ImVec2(pos.x + 720, pos.y + 450), ImColor(17, 17, 17));

                draw->AddRect(ImVec2(pos.x + 3, pos.y + 2), ImVec2(pos.x + 717, pos.y + 507), ImColor(35, 35, 35), 5.f, 15, 1.5f);
                draw->AddLine(ImVec2(pos.x + 145, pos.y + 2), ImVec2(pos.x + 145, pos.y + 507), ImColor(35, 35, 35));
                draw->AddLine(ImVec2(pos.x + 15, pos.y + 90), ImVec2(pos.x + 130, pos.y + 90), ImColor(35, 35, 35));


                draw->AddLine(ImVec2(pos.x + 25, pos.y + 195), ImVec2(pos.x + 120, pos.y + 195), ImColor(35, 35, 35));

                ImU32 linecol;
                linecol = ImGui::GetColorU32(ImLerp(ImVec4(0.12f, 0.20f, 0.32f, 1.f), ImVec4(accent_colour[0], accent_colour[1], accent_colour[2], 1.f), 1.f));

                draw->AddLine(ImVec2(pos.x + -5, pos.y + -5), ImVec2(pos.x + 720, pos.y + -5), linecol, 18.f);

                Tabs();
                TabElements();

            }
            ImGui::End();

            ImGui::SetNextWindowSize(ImVec2(150, 70));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 0.59f));
            ImGui::Begin("Spectators",nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Player1");
            ImGui::End();
            ImGui::PopStyleColor();

            ImGui::SetNextWindowSize(ImVec2(270, 55));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 0.59f));
            ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::PushFont(watermark);
            ImGui::Text("A");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("Text");
            ImGui::SameLine();
            ImGui::PushFont(watermark);
            ImGui::Text("B");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("Text");
            ImGui::SameLine();
            ImGui::PushFont(watermark);
            ImGui::Text("C");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("Text");
            ImGui::SameLine();
            ImGui::PushFont(watermark);
            ImGui::Text("D");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("Text");
            ImGui::SameLine();

            ImGui::End();
            ImGui::PopStyleColor();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

