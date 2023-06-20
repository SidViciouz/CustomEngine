#include "../Render/header/Renderer.h"
#include <iostream>
#include "../Camera/header/Camera.h"
#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Object.h"
#include "../Animation/header/AnimationGraph.h"
#include "../Particle/header/ParticleManager.h"
#include "../Input/header/InputManager.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Player.h"
#include "../Multithreading/header/ThreadPool.h"
#include "Game/header/Game.h"
#include <time.h>

using namespace Renderer;
using namespace Input;

#ifdef _DEBUG
#ifdef UNICODE                                                                                      
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else                                                                                                    
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")   
#endif                                                                                                   
#endif   

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		Game::CGame lGame;
		lGame.Initialize(hInstance);
		lGame.Begin();
		lGame.Loop();
	}
	catch (std::string errorMessage)
	{
		std::cout << errorMessage << std::endl;
	}

	return 0;
}