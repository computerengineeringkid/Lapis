#include <iostream>
#include "Window.h"
#include "App.h"

int main()
{
	std::unique_ptr<App> pApp = std::make_unique<App>();
	pApp->Run();
	
	
	return 0;
}