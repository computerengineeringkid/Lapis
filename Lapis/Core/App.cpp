#include "App.h"
#include "Window.h"
#include <iostream>
#include <string>
#include "Camera.h"

App::App()
{
    Init();
}

int App::Run()
{
    bool running = true;
    while (running)
    {
        if (!ProcessMessages())
        {
            running = false;
        }
        ProcessInput();
        DrawScene();

    }
    return 0;
}

bool App::Init()
{
    m_Window = std::make_unique<Window>();
    m_ClientWidth = m_Window->GetWindowWidth();
    m_ClientHeight = m_Window->GetWindowHeight();
    m_Camera = new Camera(AspectRatio());
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
    RenderFrame();
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

    hr  = m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_RenderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view. HRESULT: " << hr << std::endl;
        return false;
    }
    pBackBuffer->Release();
    CreateDepthBuffer();
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(),m_DepthStencilView.Get());

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
void App::CreateDepthBuffer()
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

void App::UpdateConstantBuffer(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
    struct ConstantBuffer {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    }cb;
    cb.view = DirectX::XMMatrixTranspose(viewMatrix);
    cb.projection = DirectX::XMMatrixTranspose(projectionMatrix);
    if (m_ConstantBuffer)
    {
        m_DeviceContext->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
        m_DeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());

    }
}

void App::CalculateFrameStats()
{
}

void App::ProcessInput()
{
    const float speed = 0.01f;
    const float rotationSpeed = 0.01f;
    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, speed, 0.0f);
    DirectX::XMVECTOR backward = DirectX::XMVectorSet(0.0f, 0.0f, -speed, 0.0f);
    DirectX::XMVECTOR right = DirectX::XMVectorSet(-speed, 0.0f,0.0f, 0.0f);
    DirectX::XMVECTOR left = DirectX::XMVectorSet(speed, 0.0f,0.0f, 0.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, -speed,0.0f, 0.0f);
    DirectX::XMVECTOR down = DirectX::XMVectorSet(0.0f,speed,0.0f, 0.0f);
    if (IsKeyDown(0x51))
    {
        m_Camera->Move(forward);
    }
    if (IsKeyDown(0x45))
    {
        m_Camera->Move(backward);
    }
    if (IsKeyDown(VK_RIGHT))
    {
        m_Camera->Move(right);
    }
    if (IsKeyDown(VK_LEFT))
    {
        m_Camera->Move(left);
    }
    if (IsKeyDown(VK_UP))
    {
        m_Camera->Move(up);
    } if (IsKeyDown(VK_DOWN))
    {
        m_Camera->Move(down);
    }
}

bool App::IsKeyDown(int key)
{
    return m_Window->IsKeyDown(key);
}

void App::RenderFrame()
{
    
    ClearBuffers();
    // Example matrices, replace with actual camera matrices
    DirectX::XMMATRIX viewMatrix = m_Camera->GetViewMatrix();
    DirectX::XMMATRIX projectionMatrix = m_Camera->GetProjectionMatrix();

    UpdateConstantBuffer(viewMatrix, projectionMatrix);
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
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0);
}

void App::DrawTestTriangle()
{
    HRESULT hr;
    namespace wrl = Microsoft::WRL;
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    struct Vertex {
        DirectX::XMFLOAT3 pos; 
        DirectX::XMFLOAT4 color;
    };
    const Vertex vertices[] =
    {
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Vertex 0
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},  // Vertex 1
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},   // Vertex 2
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // Vertex 3
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // Vertex 4
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},   // Vertex 5
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},    // Vertex 6
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}    // Vertex 7
    };
    unsigned int indices[] = {
    0, 1, 2, 0, 2, 3, // Front face
    4, 6, 5, 4, 7, 6, // Back face
    4, 5, 1, 4, 1, 0, // Bottom face
    3, 2, 6, 3, 6, 7, // Top face
    1, 5, 6, 1, 6, 2, // Right face
    4, 0, 3, 4, 3, 7  // Left face
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

    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    isd.SysMemPitch = 0;
    isd.SysMemSlicePitch = 0;

    struct ConstantBuffer {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = 0;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;

    hr = m_Device->CreateBuffer(&bd, nullptr, &m_ConstantBuffer);
    if (FAILED(hr)) {
        std::cerr << "Failed to create constant buffer. HRESULT: " << hr << std::endl;
    }

    hr = m_Device->CreateBuffer(&ibd, &isd, pIndexBuffer.GetAddressOf());
    if (FAILED(hr))
        std::cerr << "Failed to create index buffer. HRESULT: " << hr << std::endl;
    m_DeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

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
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}  
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
    m_DeviceContext->DrawIndexed((UINT)std::size(indices), 0, 0);
}


