#pragma once
#include <d3d11.h>
#include <wrl.h>
class InstanceBuffer
{
public:
	InstanceBuffer();

	void UpdateBuffer();
protected:
	int m_InstanceCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer; // Buffer for instance data

};

