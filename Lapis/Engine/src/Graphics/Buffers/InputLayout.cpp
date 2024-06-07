#include "InputLayout.h"
#include "Graphics/GraphicsManager.h"

InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	GraphicsManager::Get().GetDevice()->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&m_InputLayout
	);
}

void InputLayout::Bind()
{
	GraphicsManager::Get().GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
}
