#pragma once
#include <Windows.h>
#include "Timer.h"
#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;
class App
{
public:
	App();

	int Run();

	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime);
	void DrawScene() ;
	float AspectRatio() const;
	bool ProcessMessages();
	void RenderFrame();
	void EndFrame();
	void ClearBuffers();
	void DrawCube(const DirectX::XMFLOAT3& position);
	void CreateDepthBuffer();
protected:
	void UpdateConstantBuffer(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);

	bool InitDirect3D();
	void CalculateFrameStats();
	void ProcessInput();
	bool IsKeyDown(int key);
	void InitBuffers();
	void InitConstantBuffer();
	void InitShadersAndInputLayout();

protected:
	std::unique_ptr<class Window> m_Window;
	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	UINT m_4xMsaaQuality;
	Timer m_Timer;


protected:
	wrl::ComPtr<ID3D11Device> m_Device;
	wrl::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	wrl::ComPtr<IDXGISwapChain> m_SwapChain;
	wrl::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	wrl::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
	wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	wrl::ComPtr<ID3D11InputLayout> m_InputLayout;
	
	wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
	wrl::ComPtr<ID3D11PixelShader> m_PixelShader;

	D3D11_VIEWPORT m_ScreenViewport;
	class Camera* m_Camera;
	D3D_DRIVER_TYPE m_D3dDriverType;
	int m_ClientWidth, m_ClientHeight;
	bool m_Enable4xMsaa;

	struct ConstantBuffer {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX world;
	};

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	
	
};

