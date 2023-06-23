#include "World/header/World.h"
#include "Actor/header/Actor.h"
#include "../Camera/header/Camera.h"
#include "../Particle/header/ParticleManager.h"
#include "Game/header/EventManager.h"
#include "Actor/header/ActorPool.h"
#include "Game/header/Game.h"

namespace Game
{
	shared_ptr<CWorld> CWorld::Singleton()
	{
		static shared_ptr<CWorld> lWorldSingleton = shared_ptr<CWorld>(new CWorld{});
		return lWorldSingleton;
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
		//update object
		for (auto& lActor : mActors)
		{
			lActor->Update(pDeltaTime);
		}

		//update particles
		mParticleManager->Update(pDeltaTime,mCamera->GetPosition());


		//destroy object
		while(!mDestroyQueue.empty())
		{
			shared_ptr<CActor> lActor = mDestroyQueue.front();
			mDestroyQueue.pop();

			CGame::Singleton()->UnregisterActor(lActor);
			CActorPool::Singleton()->ReleaseActor(lActor);
		}
	}

	void CWorld::Add(shared_ptr<CActor> pActor)
	{
		mActors.push_back(pActor);
		mEventManager->AddListener(pActor);
	}

	void CWorld::Remove(shared_ptr<CActor> pActor)
	{
		mEventManager->RemoveListener(pActor);

		for (int i = 0; i < mActors.size(); ++i)
		{
			if (mActors[i].get() == pActor.get())
			{
				mActors.erase(mActors.begin() + i);
				return;
			}
		}

	}

	void CWorld::AddDestroyQueue(shared_ptr<CActor> pActor)
	{
		mDestroyQueue.push(pActor);
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