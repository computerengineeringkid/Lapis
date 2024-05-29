#pragma once
#include <string>
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
class Shader {
public:
    Shader(const std::wstring& filePath);
    virtual bool Initialize(class ID3D11Device* device, std::string entryPoint, std::string shaderModel) = 0;
    virtual void Bind(ID3D11DeviceContext* deviceContext) = 0;

protected:
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
    std::wstring filePath;

    bool CompileShader(std::string entryPoint, std::string shaderModel);
};


