#include "GraphicsManager.h"
#include "Core/App.h"
#include "Core/Window.h"

void GraphicsManager::SetGraphicsManager(int clientWidth, int clientHeight, HWND hWnd)
{
    m_ClientWidth = (clientWidth);
    m_ClientHeight = (clientHeight);
    m_hWnd = (hWnd);
}
GraphicsManager::GraphicsManager()
{
}



bool GraphicsManager::InitializeDirect3D() {
    
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        m_SwapChain.GetAddressOf(),
        m_Device.GetAddressOf(),
        NULL,
        m_DeviceContext.GetAddressOf()
    );
    if (FAILED(hr)) {
        std::cerr << "Failed to create device and swap chain. HRESULT: " << hr << std::endl;
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr)) {
        std::cerr << "Failed to get the back buffer. HRESULT: " << hr << std::endl;
        return false;
    }

    hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_RenderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view. HRESULT: " << hr << std::endl;
        return false;
    }
    pBackBuffer->Release();

    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
    CreateDepthBuffer();
    SetViewport((m_ClientWidth),(m_ClientHeight));
    return true;
}

void GraphicsManager::OnResize(UINT width, UINT height)
{
}

void GraphicsManager::SetViewport(float width, float height) {
    ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(width);
    m_ScreenViewport.Height = static_cast<float>(height);
    m_ScreenViewport.MinDepth = 0;
    m_ScreenViewport.MaxDepth = 1;

    m_DeviceContext->RSSetViewports(1, &m_ScreenViewport);
}

void GraphicsManager::ClearBuffer(float red, float green, float blue) {
    float color[4] = { red, green, blue, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GraphicsManager::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = m_ClientWidth;
    texDesc.Height = m_ClientHeight;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_D32_FLOAT;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


    m_DepthStencilBuffer = nullptr;
    if (FAILED(m_Device->CreateTexture2D(&texDesc, nullptr, &m_DepthStencilBuffer)))
    {
        std::cout << "DXGI: Failed to create texture for DepthStencilView\n";
        return;
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, &m_DepthStencilView)))
    {
        std::cout << "DXGI: Failed to create DepthStencilView\n";
        return;
    }
    D3D11_DEPTH_STENCIL_DESC depthDesc = {};
    depthDesc.DepthEnable = TRUE;
    depthDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_Device->CreateDepthStencilState(&depthDesc, m_DepthStencilState.GetAddressOf());
    m_DeviceContext->OMGetDepthStencilState(m_DepthStencilState.GetAddressOf(), 0);
}



void GraphicsManager::RenderBegin() {
    ClearBuffer(0.0f, 0.2f, 0.4f);
}

void GraphicsManager::RenderEnd() {
    m_SwapChain->Present(0, 0);
}

ID3D11Device* GraphicsManager::GetDevice() const {
    return m_Device.Get();
}

ID3D11DeviceContext* GraphicsManager::GetDeviceContext() const {
    return m_DeviceContext.Get();
}

