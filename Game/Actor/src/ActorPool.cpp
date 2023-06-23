#include "Actor/header/ActorPool.h"
#include "Actor/header/Actor.h"
#include "Actor/header/Bullet.h"

namespace Game
{
	shared_ptr<CActorPool> CActorPool::Singleton()
	{
		static shared_ptr<CActorPool> lActorPoolSingleton = shared_ptr<CActorPool>(new CActorPool{});

		return lActorPoolSingleton;
	}

	CActorPool::~CActorPool()
	{

	}

	CActorPool::CActorPool()
		: mMaxActorNum{MAX_ACTOR_NUM}, mActorNum{0}
	{

	}
	
	void CActorPool::ReleaseActor(shared_ptr<CActor> pActor)
	{
		pActor->Reset();
		mReleasedActors.push(pActor);
	}
}