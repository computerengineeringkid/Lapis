#pragma once
#include <DirectXMath.h>

struct InstanceData {

    DirectX::XMMATRIX world;
};
struct Vertex {
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
    DirectX::XMFLOAT2 TexCoord;
};
class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Render(class ID3D11DeviceContext* deviceContext);

    void SetPosition(const DirectX::XMFLOAT3& position);
    void SetRotation(const DirectX::XMFLOAT3& rotation);
    void SetScale(const DirectX::XMFLOAT3& scale);

    DirectX::XMFLOAT3 GetPosition() { return position; }
    DirectX::XMFLOAT3 GetRotation() { return rotation; }
    DirectX::XMFLOAT3 GetScale() { return scale; }

    DirectX::XMMATRIX CalculateWorldMatrix() const;

protected:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
};
