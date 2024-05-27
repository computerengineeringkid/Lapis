#pragma once
#include "Windows.h"
class Input
{
public:
	static bool IsKeyDown(int vKey);
	static bool IsMouseButtonDown(int button);
	static POINT GetMousePosition();
};

