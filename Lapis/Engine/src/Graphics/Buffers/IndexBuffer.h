#pragma once
#include <vector>
#include <d3d11.h>
#include <wrl.h>
class IndexBuffer
{
public:
	IndexBuffer( const std::vector<unsigned short>& indices);
	void Bind();
	UINT GetCount() const noexcept;
protected:
	UINT m_Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
};

