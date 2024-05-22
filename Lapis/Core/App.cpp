#include "App.h"
#include <iostream>
#include <string>
#include "Graphics/Objects/Camera.h"
#include "Utils/KeyCodes.h"
#include "Utils/MouseCodes.h"


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
    GraphicsManager::Get().CreateConstantBuffer();
    m_Camera = new Camera(AspectRatio());
    // Create cubes
    for (int i = 0; i < 2; ++i) {
        auto cube = std::make_unique<Cube>(GraphicsManager::Get().GetDevice(), 1);
        m_Cubes.push_back(std::move(cube));
    }
    DirectX::XMFLOAT3 pos1(-2.0f, 0.0f, 0.0f);
    m_Cubes[1]->SetPosition(pos1);
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
        
        
        data.world = DirectX::XMMatrixTranspose(cube->CalculateWorldMatrix());  // Ensure CalculateWorldMatrix exists and returns the correct matrix
        

        cube->UpdateInstanceData(data);  // Update instance data for each cube

        // Update the constant buffer

        // Render the cube
        cube->Render(GraphicsManager::Get().GetDeviceContext());
    }
    GraphicsManager::Get().UpdateConstantBuffer(viewMatrix, projectionMatrix);
    
    
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
    // Mouse rotation
    static int lastX = 0, lastY = 0;
    if (m_Window->IsMouseButtonDown(LAPIS_MOUSE_BUTTON_RIGHT)) {
        POINT mousePos = m_Window->GetMousePosition();

        // Assume lastX and lastY are class members that track the last mouse position
        int deltaX = mousePos.x - lastX;
        int deltaY = mousePos.y - lastY;

        // Reset the last position to the current position
        lastX = mousePos.x;
        lastY = mousePos.y;
        // Normalize the deltas
        float normalizedDeltaX = static_cast<float>(deltaX) / m_Window->GetWindowWidth();
        float normalizedDeltaY = static_cast<float>(deltaY) / m_Window->GetWindowHeight();

        if (deltaX != 0 || deltaY != 0) {
            m_Camera->Rotate(static_cast<float>(normalizedDeltaY) * rotationSpeed, static_cast<float>(normalizedDeltaX) * rotationSpeed);
        }
    }
}

bool App::IsKeyDown(int key)
{
    return m_Window->IsKeyDown(key);
}

bool App::IsMouseButtonDown(int button)
{
    return m_Window->IsMouseButtonDown(button);
}




