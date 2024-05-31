#include "Shader.h"
#include <D3Dcompiler.h>

Shader::Shader(const std::wstring& filePath)
{
}

bool Shader::CompileShader(std::string entryPoint, std::string shaderModel) {
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(), shaderModel.c_str(), flags, 0, &shaderBuffer, &errorBlob);

    if (errorBlob != nullptr) {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    return SUCCEEDED(hr);
}