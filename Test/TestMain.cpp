#include "Renderer.h"
#include <iostream>

using namespace Renderer;

#ifdef _DEBUG
#ifdef UNICODE                                                                                      
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else                                                                                                    
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")   
#endif                                                                                                   
#endif   

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
		HWND lWindowHandle;
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
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

		lWindowHandle = CreateWindow(L"MainWindow", L"Untitled",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);

		if (!lWindowHandle)
		{
			MessageBox(0, L"CreateWindow Failed.", 0, 0);
		}

		ShowWindow(lWindowHandle, SW_SHOW);
		UpdateWindow(lWindowHandle);

		CRenderer r(lWindowHandle);
		r.Initialize();

		float lData1[9] = { 1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f };
		float lData2[9] = { 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,1.0f,1.0f };
		float lData3[9] = { 0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,-1.0f,1.0f };

		while (1)
		{
			r.DrawBegin();

			r.DrawLine(lData1);
			r.DrawLine(lData2);
			r.DrawLine(lData3);

			r.DrawEnd();
		}
    }
    catch (std::string errorMessage)
    {
		std::cout << errorMessage << std::endl;
    }

	while (1);

	return 0;
}