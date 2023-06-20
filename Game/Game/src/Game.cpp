#include "Game/header/Game.h"
#include "../Renderer/Render/header/Renderer.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Player.h"
#include "Actor/header/Character.h"
#include "Actor/header/ActorPool.h"
#include "../Input/header/InputManager.h"
#include "World/header/World.h"
#include "../Particle/header/ParticleManager.h"
#include "Physics/header/PhysicsComponent.h"

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
		mRenderer->SetMesh(lMesh);

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
		lPlayer->AddAnimTransition("cls_walk_f", "n_run_f", [&lPlayer]()->bool {return true; }, 1.0f);
		lPlayer->AddAnimTransition("n_run_f", "n_walk_f", [&lPlayer]()->bool {return true; }, 1.0f);
		lPlayer->AddAnimTransition("n_walk_f", "cls_walk_f", [&lPlayer]()->bool {return true; }, 1.0f);

		shared_ptr<Renderer::CParticleManager> lParticleManager =  mWorld->GetParticleManager();
		shared_ptr<Renderer::CParticleSystem> lParticleSystem = lParticleManager->AddParticleSystem();
		shared_ptr<Renderer::CParticleEmitter> lParticleEmitter = lParticleManager->AddParticleEmitter(lParticleSystem);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eEmissionRate, 1.5f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::ePosition, Math::SVector3(1, 0, 3));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::ePositionVariance, Math::SVector3(1.0, 0.2, 1.0));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eVelocity, Math::SVector3(0.2, 0.2, 0.2));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eVelocityVariance, Math::SVector3(0.1, 0.1, 0.1));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eAcceleration, Math::SVector3(0.2, 0.2, 0.2));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eAccelerationVariance, Math::SVector3(0.1, 0.1, 0.1));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eScale, Math::SVector2(0.5, 0.5));
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eCurrentFrame, 0.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eDuration, 0.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eNumTextureRow, 8);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eNumTextureColumn, 8);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eFrameRate, 30.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eParticleDuration, 3.0f);
		lParticleManager->SetParticleEmitterValue(lParticleEmitter, Renderer::EParticleEmitterProperty::eParticleDurationVariance, 1.0f);
	}

	void CGame::Loop()
	{
		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();
		
		shared_ptr<Renderer::CMesh>	lMesh = make_shared<Renderer::CMesh>("../Model/Sphere.FBX");
		mRenderer->SetMesh(lMesh);
		float lX = 0;

		shared_ptr<CActor> lRecentCreatedActor;

		while (mRenderer->Loop())
		{
			if (lInputManager->GetKeyDown(VK_UP))
			{
				shared_ptr<CActor> lActor = CActorPool::Singleton()->NewActor<CActor>(lMesh);
				RegisterActor(lActor);
				lActor->SetTranslation(Math::SVector3(lX, 0, 5));
				lX += 0.2f;
				lRecentCreatedActor = lActor;
			}

			if (lInputManager->GetKeyDown(VK_DOWN))
			{
				if (lRecentCreatedActor != nullptr)
				{
					UnregisterActor(lRecentCreatedActor);
					CActorPool::Singleton()->ReleaseActor(lRecentCreatedActor);
					lRecentCreatedActor = nullptr;
				}
			}

			//update world objects
			mWorld->Update(1 / 60.0f);

			//upload data to renderer
			mRenderer->UploadBegin();

			mWorld->ToAllActors([this](shared_ptr<CActor> pActor)
			{
				mRenderer->UploadObject(pActor->GetObject());
			});

			mRenderer->UploadEnd();

			//draw
			mRenderer->DrawBegin();

			mWorld->ToAllActors([this](shared_ptr<CActor> pActor)
			{
				mRenderer->DrawMeshPBR(pActor->GetMesh(), pActor->GetObject(), -1, -1, -1, -1,-1);
			});

			mRenderer->DrawEnd();
		}
	}

	void CGame::RegisterActor(shared_ptr<CActor> pActor)
	{
		//add actor to world
		mWorld->Add(pActor);

		//register actor to renderer
		mRenderer->SetObject(pActor->GetObjectW());

	}

	void CGame::UnregisterActor(shared_ptr<CActor> pActor)
	{
		//release object from renderer
		mRenderer->ReleaseObject(pActor->GetObjectW());

		//remove actor from world
		mWorld->Remove(pActor);
	}
}