#include "Input.h"
#include "Core/App.h"

POINT Input::GetMousePosition()  {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(App::Get().GetWindowHandle(), &p);  // Convert the screen coordinates to client coordinates
	return p;
}
bool Input::IsKeyDown(int vKey)
{
	return(GetAsyncKeyState(vKey) & 0x8000) != 0;
}

bool Input::IsMouseButtonDown(int button)
{
	return (GetAsyncKeyState(button) & 0x8000) != 0;
}
