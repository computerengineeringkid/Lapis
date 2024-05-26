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
    CreateDepthBuffer();
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
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
    D3D11_DEPTH_STENCIL_DESC depthDesc = {};
    depthDesc.DepthEnable = TRUE;
    depthDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_Device->CreateDepthStencilState(&depthDesc, &m_DepthStencilState);
    m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1u);
    
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = m_ClientWidth;
    descDepth.Height = m_ClientHeight;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;


    m_DepthStencilBuffer = nullptr;
    if (FAILED(m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencilBuffer)))
    {
        std::cout << "DXGI: Failed to create texture for DepthStencilView\n";
        return;
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &descDSV, &m_DepthStencilView)))
    {
        std::cout << "DXGI: Failed to create DepthStencilView\n";
        return;
    }
    
}

void GraphicsManager::CreateConstantBuffer()
{
    HRESULT hr;
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = 0;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;

    // hr = m_Device->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer);
    hr = GraphicsManager::Get().GetDevice()->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer);
    if (FAILED(hr)) {
        std::cerr << "Failed to create constant buffer. HRESULT: " << hr << std::endl;
    }
}

void GraphicsManager::UpdateConstantBuffer(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
    ConstantBuffer cb;

    cb.view = DirectX::XMMatrixTranspose(viewMatrix);
    cb.projection = DirectX::XMMatrixTranspose(projectionMatrix);

    if (m_ConstantBuffer)
    {
        m_DeviceContext->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
        m_DeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
    }
}



void GraphicsManager::RenderBegin() {
    ClearBuffer(0.0f, 1.0f, 0.0f);
    
}

void GraphicsManager::RenderEnd() {
    m_SwapChain->Present(1u, 0u);
}

ID3D11Device* GraphicsManager::GetDevice() const {
    return m_Device.Get();
}

ID3D11DeviceContext* GraphicsManager::GetDeviceContext() const {
    return m_DeviceContext.Get();
}

