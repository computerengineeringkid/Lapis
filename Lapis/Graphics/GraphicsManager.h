#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>


namespace wrl = Microsoft::WRL;

class GraphicsManager {
public:
    void SetGraphicsManager(int clientWidth, int clientHeight, HWND hWnd);
    
    static GraphicsManager& Get() {
        static GraphicsManager instance;  // Guaranteed to be destroyed and instantiated on first use.
        return instance;
    }
    bool InitializeDirect3D();
    void OnResize(UINT width, UINT height);
    void RenderBegin();
    void RenderEnd();
    void ClearBuffer(float red, float green, float blue);
    void CreateDepthBuffer();
    

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;

private:
    GraphicsManager();
    GraphicsManager(const GraphicsManager&) = delete;
    GraphicsManager& operator=(const GraphicsManager&) = delete;
    void SetViewport(float width, float height);

    wrl::ComPtr<ID3D11Device> m_Device;
    wrl::ComPtr<ID3D11DeviceContext> m_DeviceContext;
    wrl::ComPtr<IDXGISwapChain> m_SwapChain;
    wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
    wrl::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
    wrl::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;

    D3D11_VIEWPORT m_ScreenViewport;

    int m_ClientWidth;
    int m_ClientHeight;
    HWND m_hWnd;
};