#pragma once
#include <wrl.h>
#include <d3d11.h>

class Texture
{
public:
	Texture(const class Surface& s);
	void Bind();
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};
