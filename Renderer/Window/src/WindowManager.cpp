#include "../Window/header/WindowManager.h"
#include "../Input/header/InputManager.h"
#include <iostream>

namespace Renderer
{
	shared_ptr<CWindowManager> CWindowManager::mThis = nullptr;

	LRESULT CALLBACK WndProc(HWND pHwnd, UINT pMsg, WPARAM pWParam, LPARAM pLParam)
	{
		shared_ptr<CWindowManager> lWindowManager = CWindowManager::Get();

		if (lWindowManager == nullptr)
			return DefWindowProc(pHwnd, pMsg, pWParam, pLParam);

		Input::CInputManager* lInputManager = reinterpret_cast<Input::CInputManager*>(GetWindowLongPtr(lWindowManager->GetHandle(), GWLP_USERDATA));

		if (lInputManager == nullptr)
			return DefWindowProc(pHwnd, pMsg, pWParam, pLParam);


		switch (pMsg)
		{
		case WM_KEYDOWN : 
			lInputManager->SetKeyDown(pWParam);
			break;

		case WM_KEYUP :
			lInputManager->SetKeyUp(pWParam);
			break;

		case WM_LBUTTONDOWN :
			lInputManager->SetKeyDown(VK_LBUTTON);
			break;

		case WM_LBUTTONUP:
			lInputManager->SetKeyUp(VK_LBUTTON);
			break;

		case WM_MOUSEMOVE :
			lInputManager->SetMousePosition(LOWORD(pLParam), HIWORD(pLParam));
			break;

		default : 
			return DefWindowProc(pHwnd, pMsg, pWParam, pLParam);
		}

	}

	CWindowManager::CWindowManager(HINSTANCE pHInstance)
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = pHInstance;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"MainWindow";

		if (!RegisterClass(&wc))
		{
			MessageBox(0, L"RegisterClass Failed.", 0, 0);
		}
		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, 800, 600 };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		mWindowHandle = CreateWindow(L"MainWindow", L"Untitled",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, pHInstance, 0);

		if (!mWindowHandle)
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
		}

		SetWindowLongPtr(mWindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(Input::CInputManager::Singleton().get()));

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

	}

	shared_ptr<CWindowManager> CWindowManager::Create(HINSTANCE pHInstance)
	{
		if(mThis == nullptr)
			mThis = make_shared<CWindowManager>(pHInstance);

		return mThis;
	}

	shared_ptr<CWindowManager> CWindowManager::Get()
	{
		return mThis;
	}

	HWND CWindowManager::GetHandle() const
	{
		return mWindowHandle;
	}

}