#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl.h>
class InputLayout
{
public:
	InputLayout(
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode);
	void Bind();
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
};

