#include "World/header/World.h"
#include "Actor/header/Actor.h"
#include "../Camera/header/Camera.h"
#include "../Particle/header/ParticleManager.h"
#include "Game/header/EventManager.h"

namespace Game
{
	shared_ptr<CWorld> CWorld::Create()
	{
		return shared_ptr<CWorld>(new CWorld{});
	}

	CWorld::CWorld()
	{
		mCamera = make_shared<Renderer::CCamera>(800, 600);
		mParticleManager = Renderer::CParticleManager::Singleton();
		mEventManager = CEventManager::Singleton();
	}
	
	CWorld::~CWorld()
	{

	}

	void CWorld::Update(double pDeltaTime)
	{
		for (auto& lActor : mActors)
		{
			lActor->Update(pDeltaTime);
		}

		mParticleManager->Update(pDeltaTime,mCamera->GetPosition());
	}

	void CWorld::Add(shared_ptr<CActor> pActor)
	{
		mActors.push_back(pActor);
		mEventManager->AddListener(pActor);
	}

	void CWorld::Remove(shared_ptr<CActor> pActor)
	{
		// There must be better way to find pActor from mActors
		for (int i = 0; i < mActors.size(); ++i)
		{
			if (mActors[i].get() == pActor.get())
			{
				mActors.erase(mActors.begin() + i);
				return;
			}
		}

		mEventManager->RemoveListener(pActor);
	}

	shared_ptr<Renderer::CCamera> CWorld::GetCamera() const
	{
		return mCamera;
	}

	shared_ptr<Renderer::CParticleManager>	CWorld::GetParticleManager() const
	{
		return mParticleManager;
	}
}