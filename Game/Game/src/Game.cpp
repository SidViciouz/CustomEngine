#include "Game/header/Game.h"
#include "Game/header/Timer.h"
#include "../Renderer/Render/header/Renderer.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Player.h"
#include "Actor/header/Character.h"
#include "Actor/header/ActorPool.h"
#include "../Input/header/InputManager.h"
#include "World/header/World.h"
#include "../Particle/header/ParticleManager.h"
#include "Component/header/PhysicsComponent.h"
#include "Component/header/CameraComponent.h"
#include "Actor/header/Bullet.h"

namespace Game
{	
	shared_ptr<CGame> CGame::Singleton()
	{
		static shared_ptr<CGame> lGameSingleton = shared_ptr<CGame>(new CGame());

		return lGameSingleton;
	}

	CGame::CGame()
	{

	}

	CGame::~CGame()
	{

	}
	
	void CGame::Initialize(HINSTANCE pHInstance)
	{
		// create world
		mWorld = CWorld::Singleton();

		// create renderer and initialze it
		mRenderer = Renderer::CRenderer::Singleton(pHInstance);
		mRenderer->Initialize();

		// set camera into renderer
		mRenderer->SetCamera(mWorld->GetCamera());

		// set particle manager into renderer
		mRenderer->SetParticleManager(mWorld->GetParticleManager());

		mTimer = make_shared<CTimer>();
	}


	void CGame::Begin()
	{
		mRenderer->Resize(1800, 1000);

		shared_ptr<Renderer::CMesh>	lMesh = make_shared<Renderer::CMesh>("../Model/AnimMan2.fbx");
		mRenderer->SetMesh(lMesh);

		shared_ptr<Renderer::CMesh>	lKnightMesh = make_shared<Renderer::CMesh>("../Model/Knight.fbx");
		mRenderer->SetMesh(lKnightMesh);

		shared_ptr<Renderer::CMesh>	lFloorMesh = make_shared<Renderer::CMesh>("../Model/Simple_Floor_20x20.FBX");
		mRenderer->SetMesh(lFloorMesh);

		shared_ptr<CActor> lActor = CActorPool::Singleton()->NewActor<CActor>(lKnightMesh);
		RegisterActor(lActor);
		
		shared_ptr<Game::CPlayer> lPlayer = CActorPool::Singleton()->NewActor<CPlayer>(lMesh);
		RegisterActor(lPlayer);
		shared_ptr<CameraComponent> lCameraComponent = make_shared<CameraComponent>(lPlayer, mWorld->GetCamera());
		lCameraComponent->SetDistance(5);
		lCameraComponent->SetPitch(0.5f);
		lCameraComponent->SetYaw(-0.25f);
		lPlayer->AddComponent(lCameraComponent);

		shared_ptr<CActor> lFloorActor = CActorPool::Singleton()->NewActor<CActor>(lFloorMesh);
		RegisterActor(lFloorActor);

		lActor->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		lActor->SetTranslation(Math::SVector3(3, 0, 3));
		
		lPlayer->SetScale(Math::SVector3(0.01f, 0.01f, 0.01f));
		lPlayer->SetTranslation(Math::SVector3(0, -1.5, 3));

		lFloorActor->SetTranslation(Math::SVector3(0, -3.0f, 0.0f));
		lFloorActor->SetOrientation(Math::SQuaternion(1 * cosf(DirectX::XMConvertToRadians(-45)), 0, 0, sinf(DirectX::XMConvertToRadians(-45))));
		lFloorActor->SetScale(Math::SVector3(0.03f, 0.03f, 0.03f));
		
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
		mRenderer->LoadBegin();
		int lBaseColor = mRenderer->LoadTexture(L"../Material/rustediron2_basecolor.dds");
		int lMetallic = mRenderer->LoadTexture(L"../Material/rustediron2_metallic.dds");
		int lNormal = mRenderer->LoadTexture(L"../Material/rustediron2_normal.dds");
		int lRoughness = mRenderer->LoadTexture(L"../Material/rustediron2_roughness.dds");
		int lTileAO = mRenderer->LoadTexture(L"../Material/T_Tiles_M.dds");
		int lTileNormal = mRenderer->LoadTexture(L"../Material/T_Tiles_N.dds");
		int lParticleSprite = mRenderer->LoadTexture(L"../Material/T_Smoke_A.dds");
		mRenderer->LoadEnd();

		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();
		
		shared_ptr<Renderer::CMesh>	lMesh = make_shared<Renderer::CMesh>("../Model/Sphere.FBX");
		mRenderer->SetMesh(lMesh);

		mTimer->Reset();

		while (mRenderer->Loop())
		{
			if (lInputManager->GetKeyDown(VK_UP))
			{
				shared_ptr<CActor> lActor = CActorPool::Singleton()->NewActor<CBullet>(lMesh);
				RegisterActor(lActor);
				lActor->SetTranslation(Math::SVector3(0, 0, 5));
			}


			mTimer->Tick();

			//update world objects
			mWorld->Update(mTimer->GetDeltaTime());

			//upload data to renderer
			mRenderer->UploadBegin();

			mWorld->ToAllActors([this](shared_ptr<CActor> pActor)
			{
				mRenderer->UploadObject(pActor->GetObject());
			});
			

			mRenderer->UploadEnd();

			//draw
			mRenderer->DrawBegin();

			mWorld->ToAllActors([&,this](shared_ptr<CActor> pActor)
			{
				mRenderer->DrawMeshPBR(pActor->GetMesh(), pActor->GetObject(), lBaseColor, lMetallic, lNormal, lRoughness,-1);
			});

			mRenderer->DrawParticles(lParticleSprite);
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