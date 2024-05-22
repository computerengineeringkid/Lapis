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
		static App instance; 
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

	
	void CalculateFrameStats();
	void ProcessInput();
	bool IsKeyDown(int key);
	bool IsMouseButtonDown(int button);

	

	
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
	
	class Camera* m_Camera;
	int m_ClientWidth, m_ClientHeight;
	bool m_Enable4xMsaa;

	

	std::vector<std::unique_ptr<Cube>> m_Cubes;

	
	
};

