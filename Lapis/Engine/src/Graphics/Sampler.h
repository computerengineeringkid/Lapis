#pragma once
#include <wrl.h>
#include <d3d11.h>

class Sampler
{
public:
	Sampler();
	void Bind();
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};

