#pragma once
#include <d3d11.h>
class Topology
{
public:
	Topology( D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind();
protected:
	D3D11_PRIMITIVE_TOPOLOGY m_Type;
};

