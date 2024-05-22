#pragma once
#include "Shader.h"
class PixelShader : public Shader {
public:
    
    PixelShader(const std::wstring& filePath) : Shader(filePath) {}
    bool Initialize(ID3D11Device* device, std::string entryPoint = "PS", std::string shaderModel = "ps_5_0") override;
    void Bind(ID3D11DeviceContext* deviceContext) override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
};
