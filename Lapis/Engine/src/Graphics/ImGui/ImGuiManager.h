#pragma once


#include "Windows.h"
#include <d3d11.h>
class ImGuiManager
{
public:
    ImGuiManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd);
    ~ImGuiManager();

    void BeginFrame();
    void EndFrame();
    void Render();
    //void NewFrame(); // Optionally handle more specific new frame operations if needed

private:
    void SetupImGuiStyle();

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    HWND m_Hwnd;
};

