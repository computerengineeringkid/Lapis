#pragma once
#include "Shader.h"
#include <vector>
class VertexShader : public Shader {
public:
    
    VertexShader(const std::wstring& path);
    bool Initialize(ID3D11Device* device, std::string entryPoint = "VS", std::string shaderModel = "vs_5_0") override;
    
    void Bind(ID3D11DeviceContext* deviceContext) override;
    ID3DBlob* GetBytecode() const;

private:
    Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
};

