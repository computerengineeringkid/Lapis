#include "GameObject.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")


GameObject::GameObject() :
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f) {}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime) {
    // Update logic for the object
}

void GameObject::Render(ID3D11DeviceContext* deviceContext) {
    // Rendering logic for the object
}

void GameObject::SetPosition(const DirectX::XMFLOAT3& pos) {
    position = pos;
}

void GameObject::SetRotation(const DirectX::XMFLOAT3& rot) {
    rotation = rot;
}

void GameObject::SetScale(const DirectX::XMFLOAT3& scl) {
    scale = scl;
}

DirectX::XMMATRIX GameObject::CalculateWorldMatrix() const
{
    
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    return scaleMatrix * rotationMatrix * translationMatrix;
    
}
