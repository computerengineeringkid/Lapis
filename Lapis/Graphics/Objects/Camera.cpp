#include "Camera.h"
#include "Core/Window.h"
#include "Utils/KeyCodes.h"
#include "Utils/MouseCodes.h"
#include "Core/App.h"
#include "Core/Input.h"

Camera::Camera(float aspectRatio)
   
{
    position = DirectX::XMVectorSet(0, 0, -5, 0);
    lookAt = DirectX::XMVectorSet(0, 0, 1,0);
    up = DirectX::XMVectorSet(0, 1,0,0);
    // Initialize view matrix
    // Set up the projection matrix
    SetProjectionMatrix(aspectRatio);
    UpdateViewMatrix();
}

void Camera::SetProjectionMatrix(float aspectRatio)
{
    // Field of View (FOV), Aspect Ratio, Near Plane, Far Plane
    projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.1f, 100.0f);
}

void Camera::ProcessInput(float deltaTime)
{
    const float speed = 10.0f *deltaTime;
    const float rotationSpeed = 90.0f * deltaTime;
    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, -speed, 0.0f);
    DirectX::XMVECTOR backward = DirectX::XMVectorSet(0.0f, 0.0f, speed, 0.0f);
    DirectX::XMVECTOR right = DirectX::XMVectorSet(speed, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR left = DirectX::XMVectorSet(-speed, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, speed, 0.0f, 0.0f);
    DirectX::XMVECTOR down = DirectX::XMVectorSet(0.0f, -speed, 0.0f, 0.0f);
    if (Input::IsKeyDown(LAPIS_KEY_Q))
    {
        this->Move(down);
    }
    if (Input::IsKeyDown(LAPIS_KEY_E))
    {
        this->Move(up);
    }
    if (Input::IsKeyDown(LAPIS_KEY_D))
    {
        this->Move(right);
    }
    if (Input::IsKeyDown(LAPIS_KEY_A))
    {
        this->Move(left);
    }
    if (Input::IsKeyDown(LAPIS_KEY_W))
    {
        this->Move(backward);

    }
    if (Input::IsKeyDown(LAPIS_KEY_S))
    {

        this->Move(forward);
    }
    //------ arrows ------

    if (Input::IsKeyDown(LAPIS_RIGHT_ARROW))
    {
        this->Move(right);
    }
    if (Input::IsKeyDown(LAPIS_LEFT_ARROW))
    {
        this->Move(left);
    }
    if (Input::IsKeyDown(LAPIS_UP_ARROW))
    {
        this->Move(up);
    }
    if (Input::IsKeyDown(LAPIS_DOWN_ARROW))
    {
        this->Move(down);

    }
    static bool isRightButtonDown = false;
    static int lastX = 0, lastY = 0;

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(App::Get().GetWindowHandle(), &mousePos);

    if (Input::IsMouseButtonDown(LAPIS_MOUSE_BUTTON_RIGHT)) {
        if (!isRightButtonDown) {
            // Right mouse button just pressed, initialize last position
            lastX = mousePos.x;
            lastY = mousePos.y;
            isRightButtonDown = true;
            SetCapture(App::Get().GetWindowHandle());  // Capture the mouse
        }

        int deltaX = mousePos.x - lastX;
        int deltaY = mousePos.y - lastY;

        // Normalize the deltas
        float normalizedDeltaX = static_cast<float>(deltaX) / App::Get().GetWindow()->GetWindowWidth();
        float normalizedDeltaY = static_cast<float>(deltaY) / App::Get().GetWindow()->GetWindowHeight();

        if (deltaX != 0 || deltaY != 0) {
            this->Rotate(static_cast<float>(normalizedDeltaY) * rotationSpeed, static_cast<float>(normalizedDeltaX) * rotationSpeed);
        }

        // Update last position
        lastX = mousePos.x;
        lastY = mousePos.y;
    }
    else {
        if (isRightButtonDown) {
            ReleaseCapture();  // Release mouse capture when the right button is released
            isRightButtonDown = false;
        }
    }
}

void Camera::UpdateViewMatrix() {
    using namespace DirectX;
    

    viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void Camera::Update(float deltaTime)
{
    ProcessInput(deltaTime);
}

void Camera::Rotate(float dpitch, float dyaw)
{
    using namespace DirectX;

    // Update cumulative angles
    pitch += dpitch;
    yaw += dyaw;

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
    XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR lookDirection = XMVector3TransformCoord(defaultForward, rotationMatrix);
    lookAt = XMVectorAdd(position, lookDirection);
    XMVECTOR right = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), lookDirection);
    up = XMVector3Cross(lookDirection, right);

    // Update view matrix
    UpdateViewMatrix();
}


void Camera::Move(DirectX::XMVECTOR direction)
{
    using namespace DirectX;

    // Calculate the current rotation matrix
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

    // Transform the direction vector by the rotation matrix
    XMVECTOR directionInWorld = XMVector3TransformCoord(direction, rotationMatrix);

    // Update the position and the lookAt point
    position = XMVectorAdd(position, directionInWorld);
    lookAt = XMVectorAdd(lookAt, directionInWorld);

    // Update the view matrix
    UpdateViewMatrix();
    
}
