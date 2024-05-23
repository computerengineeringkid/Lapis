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
    UpdateViewMatrix();
}
