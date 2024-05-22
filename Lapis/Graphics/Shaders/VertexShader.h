#pragma once
#include "Shader.h"
class VertexShader : public Shader {
public:
    
    VertexShader(const std::wstring& filePath) : Shader(filePath) {}
    bool Initialize(ID3D11Device* device, std::string entryPoint = "VS", std::string shaderModel = "vs_5_0") override;
    void Bind(ID3D11DeviceContext* deviceContext) override;

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
    // Possibly an input layout specific to this shader
};

