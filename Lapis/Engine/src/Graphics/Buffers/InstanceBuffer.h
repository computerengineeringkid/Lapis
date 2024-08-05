#pragma once
#include <d3d11.h>
#include <wrl.h>
class InstanceBuffer
{
public:
	InstanceBuffer(int count);

	void UpdateBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer>& GetInstanceBuffer() { return m_InstanceBuffer; }
protected:
	int m_InstanceCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer; // Buffer for instance data

};

