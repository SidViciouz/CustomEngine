#include "../Render/header/Renderer.h"
#include <iostream>
#include "../Camera/header/Camera.h"
#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Object.h"
#include "../Animation/header/AnimationGraph.h"
#include "../Particle/header/ParticleManager.h"

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


		shared_ptr<CParticleManager> lParticleManager = make_shared<CParticleManager>();
		shared_ptr<CParticleSystem> lParticleSystem = lParticleManager->AddParticleSystem();
		shared_ptr<CParticleEmitter> lParticleEmitter =  lParticleManager->AddParticleEmitter(lParticleSystem);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eAngle, Math::SVector2(1, 1));

		shared_ptr<CCamera> lCamera = make_shared<CCamera>(800,600);
		shared_ptr<CMesh>	lMesh1 = make_shared<CMesh>("../Model/Complex_Arch_01.fbx");
		shared_ptr<CObject>	lObject1 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh2 = make_shared<CMesh>("../Model/AnimMan2.FBX");
		shared_ptr<CObject>	lObject2 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh3 = make_shared<CMesh>("../Model/Sphere.fbx");
		shared_ptr<CObject>	lObject3 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh4 = make_shared<CMesh>("../Model/Simple_Floor_20x20.FBX");
		shared_ptr<CObject>	lObject4 = make_shared<CObject>();

		lObject1->SetTranslation(Math::SVector3(0, -3, 3));
		lObject1->SetOrientation(Math::SQuaternion(1*cosf(DirectX::XMConvertToRadians(-45)),0,0, sinf(DirectX::XMConvertToRadians(-45))));
		lObject1->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		lObject2->SetTranslation(Math::SVector3(0,-1.5,2));
		lObject2->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lObject2->SetScale(Math::SVector3(0.01f,0.01f,0.01f));
		lObject3->SetTranslation(Math::SVector3(0, 1.5, 5));
		lObject3->SetScale(Math::SVector3(2, 2, 2));
		lObject4->SetTranslation(Math::SVector3(0, -3.0f, 0.0f));
		lObject4->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lObject4->SetScale(Math::SVector3(0.03f, 0.03f, 0.03f));


		CAnimationGraph lAnimGraph(lMesh2->GetSkeleton());
		lAnimGraph.LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
		lAnimGraph.LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
		lAnimGraph.LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
		lAnimGraph.LoadAnimation("tpose", "../Model/ALS_Mannequin_T_Pose.FBX");
		lAnimGraph.LoadAnimation("clf_getup_back", "../Model/ALS_CLF_GetUp_Back.FBX");
		lAnimGraph.LoadAnimation("n_pose", "../Model/ALS_N_Pose.FBX");
		lAnimGraph.LoadAnimation("n_jumpwalk_lf", "../Model/ALS_N_JumpWalk_LF.FBX");
		lAnimGraph.LoadAnimation("kicking", "../Model/Kicking.fbx");
		lAnimGraph.Reset("n_walk_f");
		lAnimGraph.AddTransition("n_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
		lAnimGraph.AddTransition("n_run_f", "n_walk_f", []()->bool {return true; }, 1.0f);



		CRenderer r(lWindowHandle);
		r.Initialize();

		r.LoadBegin();
		int lTexture1Handle = r.LoadTexture(L"../Material/rustediron2_basecolor.dds");
		int lTexture2Handle = r.LoadTexture(L"../Material/rustediron2_metallic.dds");
		int lTexture3Handle = r.LoadTexture(L"../Material/rustediron2_normal.dds");
		int lTexture4Handle = r.LoadTexture(L"../Material/rustediron2_roughness.dds");
		int lTileAOHandle = r.LoadTexture(L"../Material/T_Tiles_M.dds");
		int lTileNormalHandle = r.LoadTexture(L"../Material/T_Tiles_N.dds");
		r.LoadEnd();

		float lData1[9] = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f };
		float lData2[9] = { 0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f };
		float lData3[9] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,2.0f };

		r.SetCamera(lCamera);

		int lMesh1Handle = r.SetMesh(lMesh1);
		int lObject1Handle = r.SetObject(lObject1);

		int lMesh2Handle = r.SetMesh(lMesh2);
		int lObject2Handle = r.SetObject(lObject2);

		int lMesh3Handle = r.SetMesh(lMesh3);
		int lObject3Handle = r.SetObject(lObject3);

		int lMesh4Handle = r.SetMesh(lMesh4);
		int lObject4Handle = r.SetObject(lObject4);

		SetWindowPos(lWindowHandle, NULL, 0, 0, 2000, 1000, SWP_NOZORDER | SWP_NOACTIVATE);
		lCamera->Resize(2000, 1000);
		r.Resize();

		while (1)
		{
			lAnimGraph.Update(1 / 60.0f);

			r.DrawBegin();

			r.DrawLine(lData1);
			r.DrawLine(lData2);
			r.DrawLine(lData3);

			//r.DrawMesh(lMesh1Handle, lObject1Handle);
			//r.DrawMesh(lMesh2Handle, lObject2Handle);
			r.DrawMeshPBR(lMesh1Handle, lObject1Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			r.DrawMeshPBR(lMesh2Handle, lObject2Handle, -1,-1, -1, -1, -1);
			r.DrawMeshPBR(lMesh3Handle, lObject3Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			r.DrawMeshPBR(lMesh4Handle, lObject4Handle, -1, -1, lTileNormalHandle, -1, lTileAOHandle);


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