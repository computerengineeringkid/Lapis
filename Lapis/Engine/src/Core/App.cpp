#include "App.h"
#include "Window.h"
#include <iostream>
#include <string>
#include "Graphics/Objects/Camera.h"




App::App()
{
    Init();
}

HWND& App::GetWindowHandle() const
{
     return m_Window->GetWindow();
}

Window* App::GetWindow() const
{
     return m_Window.get();
}

int App::Run()
{
    bool running = true;
    
    while (running)
    {
        double deltaTime = m_Timer.Tick();
        
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
    m_ImGuiManager = std::make_unique<ImGuiManager>(GraphicsManager::Get().GetDevice(), GraphicsManager::Get().GetDeviceContext(),m_Window->GetWindow());
    
    m_Camera = new Camera(AspectRatio());
    //model = std::make_shared<Model>(GraphicsManager::Get().GetDevice(), 1);
    //model->SetScale({ 0.1,0.1,0.1 });
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
    m_Cubes[1]->SetTexture("Engine\\src\\Graphics\\Images\\Billy.png");
    m_Cubes[0]->SetTexture("Engine\\src\\Graphics\\Images\\shot.png");
    m_Timer.Start();
    
    return true;
    
}

void App::OnResize(float width, float height)
{
    m_ClientWidth = width;
    m_ClientHeight = height;
    m_Camera->SetProjectionMatrix(AspectRatio());
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
    InstanceData data;
    /*data.world = DirectX::XMMatrixTranspose(model->CalculateWorldMatrix());
    model->Update(deltaTime);
    model->UpdateInstanceData(data);
    model->Render(GraphicsManager::Get().GetDeviceContext());*/
    GraphicsManager::Get().UpdateConstantBuffer(viewMatrix, projectionMatrix);
    m_ImGuiManager->Render();
    GraphicsManager::Get().RenderEnd();
}


void App::CalculateFrameStats()
{
    static int frameCount = 0;
    static double timeElapsed = 0.0;
    double deltaTime = m_Timer.Tick();  // Get the time elapsed since last frame
    timeElapsed += deltaTime;
    frameCount++;

    // Update window title with FPS and frame time every second
    if (timeElapsed >= 1.0) {
        double fps = frameCount / timeElapsed;  // Average FPS over the second
        double mspf = 1000.0 / fps;  // Milliseconds per frame

        std::string windowText = "FPS: " + std::to_string(fps) + " Frame Time: " + std::to_string(mspf) + " ms";
        SetWindowText(App::Get().GetWindowHandle(), windowText.c_str());

        // Reset for the next average calculation
        frameCount = 0;
        timeElapsed = 0.0;
    }
}








