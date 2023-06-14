#pragma once

#include <Windows.h>
#include <memory>

using namespace std;

namespace Input
{
	class CInputManager;
}

namespace Renderer
{

	class CWindowManager
	{
	public:
												CWindowManager(HINSTANCE pHInstance);

		static shared_ptr<CWindowManager>		Create(HINSTANCE pHInstance);

		static shared_ptr<CWindowManager>		Get();

		HWND									GetHandle() const;

	private:

		static shared_ptr<CWindowManager>		mThis;

		HWND									mWindowHandle;
	};
}
