#pragma once
#include "GameObject.h"
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "Graphics/Shaders/VertexShader.h"
#include "Graphics/Shaders/PixelShader.h"

class Model : public GameObject
{
public:

    Model(ID3D11Device* device, int instanceCount);
    ~Model();
    void Update(float deltaTime) override;
    void Render(ID3D11DeviceContext* deviceContext) override;
    void UpdateInstanceData(const InstanceData& data);

    std::shared_ptr<VertexShader> GetVertexShader() const {
        return m_VertexShader;
    }

    std::shared_ptr<PixelShader> GetPixelShader() const {
        return m_PixelShader;
    }
    ;
    void SetTexture(const std::string& path);
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer; // Buffer for instance data
    std::shared_ptr<VertexShader> m_VertexShader;
    std::shared_ptr<PixelShader> m_PixelShader;
    std::shared_ptr<class Texture> m_Texture;
    std::shared_ptr<class Sampler> m_Sampler;

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    int m_instanceCount;
public:
    bool canRotate = false;
    bool InitializeBuffers(ID3D11Device* device);
};

