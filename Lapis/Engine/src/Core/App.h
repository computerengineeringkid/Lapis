#pragma once
#include <Windows.h>
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
#include "Graphics/GDIPlusManager.h"
#include "Graphics/Objects/Model.h"
#include "Graphics/ImGui/ImGuiManager.h"


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
	void OnResize(float width, float height);
	void UpdateScene(float deltaTime);
	float AspectRatio() const;
	bool ProcessMessages();
	void RenderScene(float deltaTime);
	
	
protected:
	void CalculateFrameStats();

private:
	App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
protected:
	std::shared_ptr<class Window> m_Window;
	
	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	UINT m_4xMsaaQuality;
	Timer m_Timer;
	std::unique_ptr<ImGuiManager> m_ImGuiManager;
public:
	HWND& GetWindowHandle() const;
	Window* GetWindow() const;
	

protected:
	
	class Camera* m_Camera;
	int m_ClientWidth, m_ClientHeight;
	bool m_Enable4xMsaa;
	std::unique_ptr<GDIPlusManager> m_GDIManager;
	std::shared_ptr<Model> model;

	std::vector<std::unique_ptr<Cube>> m_Cubes;


	
	
};

