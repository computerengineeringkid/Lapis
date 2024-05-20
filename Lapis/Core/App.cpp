#include "App.h"
#include "Window.h"
#include <iostream>
#include <string>

App::App()
{
    Init();
}

int App::Run()
{
    return 0;
}

bool App::Init()
{
    m_Window = std::make_unique<Window>();
    m_ClientWidth = m_Window->GetWindowWidth();
    m_ClientHeight = m_Window->GetWindowHeight();
    InitDirect3D();
    return true;
    
}

void App::OnResize()
{
}

void App::UpdateScene(float deltaTime)
{
}

void App::DrawScene()
{
}

float App::AspectRatio() const
{
    return static_cast<float>(m_ClientWidth / m_ClientHeight);
}

bool App::ProcessMessages()
{
    return m_Window->ProcessMessages();
}



bool App::InitDirect3D()
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_Window->GetWindow();
    scd.SampleDesc.Count = 4;
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

    hr  = m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_RenderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view. HRESULT: " << hr << std::endl;
        return false;
    }
    pBackBuffer->Release();

    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);

    ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
    m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
    m_ScreenViewport.MinDepth = 0;
    m_ScreenViewport.MaxDepth = 1;

    m_DeviceContext->RSSetViewports(1, &m_ScreenViewport);
    return true;
}

void App::CalculateFrameStats()
{
}

void App::RenderFrame()
{
    ClearBuffers();
    DrawTestTriangle();
    EndFrame();
    
}

void App::EndFrame()
{
    m_SwapChain->Present(0, 0);
}

void App::ClearBuffers()
{
    const float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
}

void App::DrawTestTriangle()
{
    HRESULT hr;
    namespace wrl = Microsoft::WRL;
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    struct Vertex {
        float x, y;
    };
    const Vertex vertices[] =
    {
        {0.0f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f}
    };
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    m_Device->CreateBuffer(&bd, &sd, &pVertexBuffer);
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    m_DeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    wrl::ComPtr<ID3DBlob> pBlob;

    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
    if (FAILED(hr)) {
        std::cerr << "Failed to load pixel shader. HRESULT: " << hr << std::endl;
        return;
    }
    hr = m_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
    if (FAILED(hr)) {
        std::cerr << "Failed to create vertex shader. HRESULT: " << hr << std::endl;
        return;
    }

    m_DeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
    if (FAILED(hr)) {
        std::cerr << "Failed to load pixel shader. HRESULT: " << hr << std::endl;
        return;
    }
    hr = m_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
    if (FAILED(hr)) {
        std::cerr << "Failed to create pixel shader. HRESULT: " << hr << std::endl;
        return;
    }
    m_DeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    m_Device->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    );
    m_DeviceContext->IASetInputLayout(pInputLayout.Get());
    m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    

    m_DeviceContext->Draw((UINT)std::size(vertices), 0u);
}
