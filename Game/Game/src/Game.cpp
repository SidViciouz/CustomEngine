#include "Game/header/Game.h"
#include "../Renderer/Render/header/Renderer.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Player.h"
#include "Actor/header/Character.h"

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
		shared_ptr<Game::CPlayer> lPlayer = Game::CPlayer::Create("../Model/AnimMan2.FBX");

		RegisterActor(lPlayer);
	}

	void CGame::RegisterActor(shared_ptr<CActor> pActor)
	{
		//add actor to world
		mWorld->Add(pActor);

		//register actor to renderer
		mRenderer->SetMesh(pActor->GetMesh());
		mRenderer->SetObject(pActor->GetObject());

	}

	void CGame::UnregisterActor(shared_ptr<CActor> pActor)
	{

	}
}