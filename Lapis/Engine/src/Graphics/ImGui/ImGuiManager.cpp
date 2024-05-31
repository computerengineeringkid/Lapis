#include "ImGuiManager.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

ImGuiManager::ImGuiManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
    : m_Device(device), m_DeviceContext(deviceContext), m_Hwnd(hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, deviceContext);
    SetupImGuiStyle();
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Render()
{
    // Start a new frame
    BeginFrame();  // This handles ImGui_ImplDX11_NewFrame, ImGui_ImplWin32_NewFrame, and ImGui::NewFrame

    static bool show_demo_window = true;
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    // End the frame
    EndFrame();  // This will call ImGui::Render() and ImGui_ImplDX11_RenderDrawData
}


void ImGuiManager::SetupImGuiStyle()
{
    // Set your ImGui style or load from a configuration
    ImGui::StyleColorsDark();
}