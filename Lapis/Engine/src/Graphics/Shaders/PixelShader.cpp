#include "PixelShader.h"
#include <iostream>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Graphics/GraphicsManager.h"


bool PixelShader::Initialize(ID3D11Device* device, std::string entryPoint, std::string shaderModel)
{
    HRESULT hr = this->CompileShader(entryPoint, shaderModel);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to compile the pixel shader. HRESULT: " << hr << std::endl;
        return false;
    }
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    hr = D3DReadFileToBlob(L"PixelShader.ps.cso", &pBlob);
    // Create the pixel shader
    hr = device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create the pixel shader. HRESULT: " << hr << std::endl;
        return false;
    }

    return true;
}


PixelShader::PixelShader(const std::wstring& path)
    : Shader(path)
{
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    HRESULT hr = D3DReadFileToBlob(path.c_str(), &pBlob);
    // Create the pixel shader
    hr = GraphicsManager::Get().GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create the pixel shader. HRESULT: " << hr << std::endl;
        
    }
}
void PixelShader::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

}
