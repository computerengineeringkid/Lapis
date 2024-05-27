#include "GameObject.h"
#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include "Graphics/Shaders/VertexShader.h"
#include "Graphics/Shaders/PixelShader.h"
#include <memory>

struct InstanceData {
    
    DirectX::XMMATRIX world;
};
struct Vertex {
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
    DirectX::XMFLOAT2 TexCoord;
};

class Cube : public GameObject {
public:
    Cube(ID3D11Device* device, int instanceCount);
    ~Cube();
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

    int m_instanceCount;
public:
    bool canRotate = false;
    bool InitializeBuffers(ID3D11Device* device);
};