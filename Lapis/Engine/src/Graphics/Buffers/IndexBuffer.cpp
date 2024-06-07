#include "IndexBuffer.h"
#include "Graphics/GraphicsManager.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices)
	:
	m_Count((UINT)indices.size())
{
	

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(m_Count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GraphicsManager::Get().GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);
}

void IndexBuffer::Bind()
{
	GraphicsManager::Get().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}


UINT IndexBuffer::GetCount() const noexcept
{
    return m_Count;
}
