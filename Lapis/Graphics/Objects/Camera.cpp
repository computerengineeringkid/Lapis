#include "Camera.h"

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

void Camera::UpdateViewMatrix() {
    using namespace DirectX;
    

    viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void Camera::Rotate(float pitch, float yaw)
{
    using  namespace DirectX;
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
    XMVECTOR lookDirection = XMVectorSubtract(lookAt, position);
    lookDirection = XMVector3TransformCoord(lookDirection, rotationMatrix);
    lookAt = XMVectorAdd(position, lookDirection);
    XMVECTOR right = XMVector3Cross(up, lookDirection);
    up = XMVector3Cross(lookDirection, right);
    UpdateViewMatrix();
}

void Camera::Move(DirectX::XMVECTOR deltaPos)
{
    position = DirectX::XMVectorAdd(position, deltaPos);
    lookAt = DirectX::XMVectorAdd(lookAt, deltaPos);
    UpdateViewMatrix();
}
