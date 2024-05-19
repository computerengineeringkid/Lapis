#include <iostream>
#include "Window.h"

int main()
{
	
	Window* pWindow = new Window();
	bool running = true;
	while (running)
	{
		if (!pWindow->ProcessMessages())
		{
			running = false;
		}
	}
	delete pWindow;
	return 0;
}