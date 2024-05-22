#include "App.h"
#include <iostream>
#include <string>
#include "Graphics/Objects/Camera.h"
#include "Utils/KeyCodes.h"


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
    GraphicsManager::Get().SetGraphicsManager(m_ClientWidth, m_ClientHeight, m_Window->GetWindow());
    GraphicsManager::Get().InitializeDirect3D();
    InitConstantBuffer();
    m_Camera = new Camera(AspectRatio());
    // Create cubes
    for (int i = 0; i < 2; ++i) {
        auto cube = std::make_unique<Cube>(GraphicsManager::Get().GetDevice(), 1);
        m_Cubes.push_back(std::move(cube));
    }

    m_Timer.Start();
    return true;
    
}
void App::RenderFrame()
{

    //ClearBuffers();
    GraphicsManager::Get().RenderBegin();
    DirectX::XMMATRIX viewMatrix = m_Camera->GetViewMatrix();
    DirectX::XMMATRIX projectionMatrix = m_Camera->GetProjectionMatrix();

    double deltaTime = m_Timer.Tick();
    
    
    // Update each cube's instance data and render
    for (auto& cube : m_Cubes) {
        InstanceData data;

        float rotationSpeed = 1.0f;  // Define rotation speed
        DirectX::XMFLOAT3 rotation = m_Cubes[0]->GetRotation();
        rotation.y += deltaTime * rotationSpeed;  // Increment Y rotation based on elapsed time
        m_Cubes[0]->SetRotation(rotation);
        DirectX::XMFLOAT3 pos1(-2.0f, 0.0f, 0.0f);
        m_Cubes[1]->SetPosition(pos1);
        
        data.world = DirectX::XMMatrixTranspose(cube->CalculateWorldMatrix());  // Ensure CalculateWorldMatrix exists and returns the correct matrix
        

        cube->UpdateInstanceData(data);  // Update instance data for each cube

        // Update the constant buffer

        // Render the cube
        cube->Render(GraphicsManager::Get().GetDeviceContext());
    }
    UpdateConstantBuffer(viewMatrix, projectionMatrix);
    
    
    /*DirectX::XMFLOAT3 pos1(-2.0f, 0.0f, 0.0f);
    DrawCube(pos1);
    DirectX::XMFLOAT3 pos2(2.0f, 0.0f, 0.0f);
    DrawCube(pos2);
    DirectX::XMFLOAT3 pos3(0.0f, 0.0f, 0.0f);
    DrawCube(pos3);*/

    GraphicsManager::Get().RenderEnd();
    //EndFrame();

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




void App::UpdateConstantBuffer( const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
    ConstantBuffer cb;
    
    cb.view = DirectX::XMMatrixTranspose(viewMatrix);
    cb.projection = DirectX::XMMatrixTranspose(projectionMatrix);
    if (m_ConstantBuffer)
    {
        /*m_DeviceContext->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
        m_DeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());*/
        
        GraphicsManager::Get().GetDeviceContext()->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
        GraphicsManager::Get().GetDeviceContext()->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());

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
    if (IsKeyDown(LAPIS_KEY_Q))
    {
        m_Camera->Move(forward);
    }
    if (IsKeyDown(LAPIS_KEY_E))
    {
        m_Camera->Move(backward);
    }
    if (IsKeyDown(LAPIS_KEY_D))
    {
        m_Camera->Move(right);
    }
    if (IsKeyDown(LAPIS_KEY_A))
    {
        m_Camera->Move(left);
    }
    if (IsKeyDown(LAPIS_KEY_W))
    {
        m_Camera->Move(up);
    } if (IsKeyDown(LAPIS_KEY_S))
    {
        m_Camera->Move(down);
    }
}

bool App::IsKeyDown(int key)
{
    return m_Window->IsKeyDown(key);
}

void App::InitConstantBuffer()
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



