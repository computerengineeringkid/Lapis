#include "App.h"
#include <iostream>
#include <string>
#include "Graphics/Objects/Camera.h"




App::App()
{
    Init();
}

int App::Run()
{
    bool running = true;
    
    while (running)
    {
        double deltaTime = m_Timer.Tick();
        std::cout << deltaTime << std::endl;
        if (!ProcessMessages())
        {
            running = false;
        }
        UpdateScene(deltaTime);

    }
    return 0;
}

bool App::Init()
{
    m_Window = std::make_shared<Window>();
    m_ClientWidth = m_Window->GetWindowWidth();
    m_ClientHeight = m_Window->GetWindowHeight();
    m_GDIManager = std::make_unique<GDIPlusManager>();
    GraphicsManager::Get().SetGraphicsManager(m_ClientWidth, m_ClientHeight, m_Window->GetWindow());
    GraphicsManager::Get().InitializeDirect3D();
    GraphicsManager::Get().CreateConstantBuffer();
    
    m_Camera = new Camera(AspectRatio());
    // Create cubes
    for (int i = 0; i < 3; ++i) {
        auto cube = std::make_unique<Cube>(GraphicsManager::Get().GetDevice(), 1);
        m_Cubes.push_back(std::move(cube));
    }
    DirectX::XMFLOAT3 pos1(-3.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 pos2(-7.0f, 0.0f, 0.0f);
    m_Cubes[1]->SetPosition(pos1);
    m_Cubes[2]->SetPosition(pos2);
    m_Cubes[1]->canRotate = true;
    m_Cubes[1]->SetTexture("Graphics\\Images\\Billy.png");
    m_Timer.Start();
    
    return true;
    
}

void App::OnResize()
{
}

void App::UpdateScene(float deltaTime)
{
    m_Camera->Update(deltaTime);
    RenderScene(deltaTime);
}



float App::AspectRatio() const
{
    return static_cast<float>(m_ClientWidth / m_ClientHeight);
}

bool App::ProcessMessages()
{
    return m_Window->ProcessMessages();
}



void App::RenderScene(float deltaTime)
{
    //ClearBuffers();
    GraphicsManager::Get().RenderBegin();
    DirectX::XMMATRIX viewMatrix = m_Camera->GetViewMatrix();
    DirectX::XMMATRIX projectionMatrix = m_Camera->GetProjectionMatrix();

    // Update each cube's instance data and render
    for (auto& cube : m_Cubes) {
        InstanceData data; 
        data.world = DirectX::XMMatrixTranspose(cube->CalculateWorldMatrix());  // Ensure CalculateWorldMatrix exists and returns the correct matrix

        cube->Update(deltaTime);
        cube->UpdateInstanceData(data);  // Update instance data for each cube

        // Update the constant buffer

        // Render the cube
        cube->Render(GraphicsManager::Get().GetDeviceContext());
    }
    GraphicsManager::Get().UpdateConstantBuffer(viewMatrix, projectionMatrix);

    GraphicsManager::Get().RenderEnd();
}


void App::CalculateFrameStats()
{
}








