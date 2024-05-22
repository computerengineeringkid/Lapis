#pragma once
#include <Windows.h>
#include "Window.h"
#include "Utils/Timer.h"
#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "Graphics/GraphicsManager.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <vector>
#include "Graphics/Objects/Cube.h"
namespace wrl = Microsoft::WRL;

class App
{
public:
	
	static App& Get() {
		static App instance;  // Guaranteed to be destroyed and instantiated on first use.
		return instance;
	}

	int Run();

	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime);
	void DrawScene() ;
	float AspectRatio() const;
	bool ProcessMessages();
	void RenderFrame();
	
	
protected:
	void UpdateConstantBuffer(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);

	
	void CalculateFrameStats();
	void ProcessInput();
	bool IsKeyDown(int key);
	
	void InitConstantBuffer();
	

	
private:
	App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
protected:
	std::unique_ptr<Window> m_Window;
	
	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	UINT m_4xMsaaQuality;
	Timer m_Timer;
public:
	HWND& GetWindow() const { return m_Window->GetWindow(); }

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
		
	};

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 tex;
	};

	std::vector<std::unique_ptr<Cube>> m_Cubes;

	
	
};

