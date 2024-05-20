#include <iostream>
#include "Window.h"
#include "App.h"

int main()
{
	App* pApp = new App();
	
	bool running = true;
	while (running)
	{
		if (!pApp->ProcessMessages())
		{
			running = false;
		}
		pApp->RenderFrame();
		
	}
	delete pApp;
	return 0;
}