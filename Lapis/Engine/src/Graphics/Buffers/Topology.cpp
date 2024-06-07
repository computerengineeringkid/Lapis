#include "Topology.h"
#include "Graphics/GraphicsManager.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_Type(type)
{}

void Topology::Bind() 
{
	GraphicsManager::Get().GetDeviceContext()->IASetPrimitiveTopology(m_Type);
}
