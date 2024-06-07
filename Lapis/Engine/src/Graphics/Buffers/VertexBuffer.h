#pragma once
#include <vector>
#include <wrl.h>
#include "d3d11.h";
#include "Graphics/GraphicsManager.h"
class VertexBuffer
{
public:

	template<class V>
	VertexBuffer(std::vector<V> vertices)
		:
		m_Stride(sizeof(V))
	{
		

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GraphicsManager::Get().GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}
public:
	void Bind();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() { return pVertexBuffer; }

private:
	UINT m_Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

