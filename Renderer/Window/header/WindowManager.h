#pragma once

#include <Windows.h>

namespace Renderer
{
	class CWindowManager
	{
	public:
												CWindowManager(HINSTANCE pHInstance);
		HWND									GetHandle() const;

	private:

		HWND									mWindowHandle;
	};
}
