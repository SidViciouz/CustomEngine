#include "Game/header/Game.h"
#include "../Renderer/Render/header/Renderer.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Player.h"
#include "Actor/header/Character.h"
#include "Actor/header/ActorPool.h"
#include "../Input/header/InputManager.h"
#include "World/header/World.h"

namespace Game
{
	
	CGame::CGame()
	{

	}

	CGame::~CGame()
	{

	}
	
	void CGame::Initialize(HINSTANCE pHInstance)
	{
		// create world
		mWorld = CWorld::Create();

		// create renderer and initialze it
		mRenderer = Renderer::CRenderer::Singleton(pHInstance);
		mRenderer->Initialize();

		// set camera into renderer
		mRenderer->SetCamera(mWorld->GetCamera());

		// set particle manager into renderer
		mRenderer->SetParticleManager(mWorld->GetParticleManager());
	}


	void CGame::Begin()
	{
		shared_ptr<Renderer::CMesh>	lMesh = make_shared<Renderer::CMesh>("../Model/AnimMan2.FBX");

		shared_ptr<CActor> lActor = CActorPool::Singleton()->NewActor<CActor>(lMesh);
		RegisterActor(lActor);

		shared_ptr<Game::CPlayer> lPlayer = CActorPool::Singleton()->NewActor<CPlayer>(lMesh);
		RegisterActor(lPlayer);

		lActor->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		//lActor->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lActor->SetTranslation(Math::SVector3(3, 0, 3));

		lPlayer->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		lPlayer->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lPlayer->SetTranslation(Math::SVector3(0, 0, 3));

		//set animation
		lPlayer->LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
		lPlayer->LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
		lPlayer->LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
		lPlayer->ResetAnimation("n_walk_f");
		lPlayer->AddAnimTransition("cls_walk_f", "n_run_f", []()->bool {return true; }, 1.0f);
		lPlayer->AddAnimTransition("n_run_f", "n_walk_f", []()->bool {return true; }, 1.0f);
		lPlayer->AddAnimTransition("n_walk_f", "cls_walk_f", []()->bool {return true; }, 1.0f);
	}

	void CGame::Loop()
	{
		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();
		
		shared_ptr<Renderer::CMesh>	lMesh = make_shared<Renderer::CMesh>("../Model/Sphere.FBX");
		float lX = 0;

		while (mRenderer->Loop())
		{
			if (lInputManager->GetKeyDown(VK_UP))
			{
				shared_ptr<CActor> lActor = CActorPool::Singleton()->NewActor<CActor>(lMesh);
				RegisterActor(lActor);
				lActor->SetTranslation(Math::SVector3(lX, 0, 5));
				lX += 0.2f;
			}

			mWorld->Update(1 / 60.0f);


			mRenderer->DrawBegin();

			mWorld->ToAllActors([this](shared_ptr<CActor> pActor)
			{
				mRenderer->DrawMeshPBR(pActor->GetMeshHandle(), pActor->GetObjectHandle(), -1, -1, -1, -1,-1);
			});

			
			/*
			mRenderer->DrawLine(lData1);
			mRenderer->DrawLine(lData2);
			mRenderer->DrawLine(lData3);

			mRenderer->DrawMeshPBR(lMesh4Handle, lObject4Handle, -1, -1, lTileNormalHandle, -1, lTileAOHandle);
			mRenderer->DrawMeshPBR(lMesh3Handle, lObject3Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			mRenderer->DrawMeshPBR(lMesh1Handle, lObject1Handle, lTexture1Handle, lTexture2Handle, lTexture3Handle, lTexture4Handle, -1);
			mRenderer->DrawParticles(lParticleSpriteHandle);
			mRenderer->DrawMeshPBR(lMesh2Handle, lObject2Handle, -1, -1, -1, -1, -1);
			*/
			mRenderer->DrawEnd();
		}
	}

	void CGame::RegisterActor(shared_ptr<CActor> pActor)
	{
		//add actor to world
		mWorld->Add(pActor);

		//register actor to renderer
		pActor->SetMeshHandle(mRenderer->SetMesh(pActor->GetMesh()));
		pActor->SetObjectHandle(mRenderer->SetObject(pActor->GetObject()));

	}

	void CGame::UnregisterActor(shared_ptr<CActor> pActor)
	{

	}
}