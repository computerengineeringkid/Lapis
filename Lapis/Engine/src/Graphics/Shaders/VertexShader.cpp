#include "VertexShader.h"
#include <iostream>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Graphics/GraphicsManager.h"

bool VertexShader::Initialize(ID3D11Device* device, std::string entryPoint, std::string shaderModel)
{
    HRESULT hr = this->CompileShader(entryPoint, shaderModel);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to compile the pixel shader. HRESULT: " << hr << std::endl;
        return false;
    }
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    hr = D3DReadFileToBlob(L"VertexShader.vs.cso", &pBlob);
    // Create the pixel shader
    hr = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create the vertex shader. HRESULT: " << hr << std::endl;
        return false;
    }
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"MATERIALINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},

        
        {"INSTANCEWORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1}

    };
    
   
   // hr = D3DReadFileToBlob(L"VertexShader.vs.cso", &pBlob);

    
}



VertexShader::VertexShader( const std::wstring& path)
    : Shader(path)
{
    

    D3DReadFileToBlob(path.c_str(), &pBytecodeBlob);
   GraphicsManager::Get().GetDevice()->CreateVertexShader(
        pBytecodeBlob->GetBufferPointer(),
        pBytecodeBlob->GetBufferSize(),
        nullptr,
        &m_VertexShader
    );
}



ID3DBlob* VertexShader::GetBytecode() const
{
    return pBytecodeBlob.Get();
}

void VertexShader::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);

}

