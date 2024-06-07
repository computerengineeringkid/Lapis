#include "VertexBuffer.h"
#include "Graphics/GraphicsManager.h"


void VertexBuffer::Bind()
{
	const UINT offset = 0u;
	GraphicsManager::Get().GetDeviceContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &m_Stride, &offset);
}
