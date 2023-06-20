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

		shared_ptr<CInputManager> lInputManager = CInputManager::Singleton();

		shared_ptr<Game::CPlayer> lActor = Game::CPlayer::Create("../Model/AnimMan2.FBX");

		shared_ptr<CParticleManager> lParticleManager = CParticleManager::Singleton();
		shared_ptr<CParticleSystem> lParticleSystem = lParticleManager->AddParticleSystem();
		shared_ptr<CParticleEmitter> lParticleEmitter = lParticleManager->AddParticleEmitter(lParticleSystem);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eEmissionRate, 1.5f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::ePosition, Math::SVector3(1, 0, 3));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::ePositionVariance, Math::SVector3(1.0, 0.2, 1.0));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eVelocity, Math::SVector3(0.2, 0.2, 0.2));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eVelocityVariance, Math::SVector3(0.1, 0.1, 0.1));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eAcceleration, Math::SVector3(0.2, 0.2, 0.2));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eAccelerationVariance, Math::SVector3(0.1, 0.1, 0.1));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eScale, Math::SVector2(0.5, 0.5));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eCurrentFrame, 0.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eDuration, 0.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eNumTextureRow, 8);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eNumTextureColumn, 8);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eFrameRate, 30.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eParticleDuration, 3.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, EParticleEmitterProperty::eParticleDurationVariance, 1.0f);

		shared_ptr<CCamera> lCamera = make_shared<CCamera>(800, 600);
		shared_ptr<CMesh>	lMesh1 = make_shared<CMesh>("../Model/Complex_Arch_01.fbx");
		shared_ptr<CObject>	lObject1 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh2 = make_shared<CMesh>("../Model/AnimMan2.FBX");
		shared_ptr<CObject>	lObject2 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh3 = make_shared<CMesh>("../Model/Sphere.fbx");
		shared_ptr<CObject>	lObject3 = make_shared<CObject>();
		shared_ptr<CMesh>	lMesh4 = make_shared<CMesh>("../Model/Simple_Floor_20x20.FBX");
		shared_ptr<CObject>	lObject4 = make_shared<CObject>();

		lObject1->SetTranslation(Math::SVector3(0, -3, 3));
		lObject1->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lObject1->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		lObject2->SetTranslation(Math::SVector3(0, -1.5, 2));
		lObject2->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lObject2->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
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
		lAnimGraph.LoadAnimation("zombie_walk", "../Model/Zombie_Walk.fbx");
		lAnimGraph.Reset("n_walk_f");
		lAnimGraph.AddTransition("cls_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
		lAnimGraph.AddTransition("n_run_f", "n_walk_f", []()->bool {return true; }, 1.0f);
		lAnimGraph.AddTransition("n_walk_f", "cls_walk_f", []()->bool {return true; }, 1.0f);

		shared_ptr<CRenderer> r = CRenderer::Singleton(hInstance);


		r->Initialize();

		r->LoadBegin();
		int lTexture1Handle = r->LoadTexture(L"../Material/rustediron2_basecolor.dds");
		int lTexture2Handle = r->LoadTexture(L"../Material/rustediron2_metallic.dds");
		int lTexture3Handle = r->LoadTexture(L"../Material/rustediron2_normal.dds");
		int lTexture4Handle = r->LoadTexture(L"../Material/rustediron2_roughness.dds");
		int lTileAOHandle = r->LoadTexture(L"../Material/T_Tiles_M.dds");
		int lTileNormalHandle = r->LoadTexture(L"../Material/T_Tiles_N.dds");
		int lParticleSpriteHandle = r->LoadTexture(L"../Material/T_Smoke_A.dds");
		r->LoadEnd();

		float lData1[9] = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f };
		float lData2[9] = { 0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f };
		float lData3[9] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,2.0f };

		r->SetCamera(lCamera);

		int lMesh1Handle = r->SetMesh(lMesh1);
		int lObject1Handle = r->SetObject(lObject1);

		int lMesh2Handle = r->SetMesh(lMesh2);
		int lObject2Handle = r->SetObject(lObject2);

		int lMesh3Handle = r->SetMesh(lMesh3);
		int lObject3Handle = r->SetObject(lObject3);

		int lMesh4Handle = r->SetMesh(lMesh4);
		int lObject4Handle = r->SetObject(lObject4);

		r->SetParticleManager(lParticleManager);

		r->Resize(1800, 1000);

		while (r->Loop())
		{
			if (lInputManager->GetKeyPressed(0x41))
			{
				lObject2->AddTranslation(Math::SVector3(-0.01f, 0, 0));
			}
			if (lInputManager->GetKeyPressed(0x44))
			{
				lObject2->AddTranslation(Math::SVector3(0.01f, 0, 0));
			}
			if (lInputManager->GetKeyPressed(0x57))
			{
				lObject2->AddTranslation(Math::SVector3(0, 0, 0.01f));
			}
			if (lInputManager->GetKeyPressed(0x53))
			{
				lObject2->AddTranslation(Math::SVector3(0, 0, -0.01f));
			}

			lParticleManager->Update(1 / 60.0f, lCamera->GetPosition());

			//animation multithreading으로 최적화해야함.
			lAnimGraph.Update(1 / 60.0f);

			r->DrawBegin();

			r->DrawLine(lData1);
			r->DrawLine(lData2);
			r->DrawLine(lData3);

			//r.DrawMesh(lMesh1Handle, lObject1Handle);
			r->DrawMeshPBR(lMesh4Handle, lObject4Handle, -1, -1, lTileNormalHandle, -1, lTileAOHandle);
			r->DrawMeshPBR(lMesh3Handle, lObject3Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			r->DrawMeshPBR(lMesh1Handle, lObject1Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			r->DrawParticles(lParticleSpriteHandle);
			r->DrawMeshPBR(lMesh2Handle, lObject2Handle, -1, -1, -1, -1, -1);

			r->DrawEnd();
		}
		
	}
	catch (std::string errorMessage)
	{
		std::cout << errorMessage << std::endl;
	}

	return 0;
}