#include "Actor/header/ActorPool.h"
#include "Actor/header/Actor.h"

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

	shared_ptr<CActor> CActorPool::NewActor(shared_ptr<Renderer::CMesh> pMesh)
	{
		if (!mReleasedActors.empty())
		{
			shared_ptr<CActor> lTop = mReleasedActors.top();
			mReleasedActors.pop();

			return lTop;

		}
		else if (mActorNum < mMaxActorNum)
		{
			mActorPool.push_back(CActor::Create(pMesh));

			++mActorNum;

			return mActorPool.back();
		}
		else
		{
			return nullptr;
		}
	}

	void CActorPool::ReleaseActor(shared_ptr<CActor> pActor)
	{
		pActor->Reset();
		mReleasedActors.push(pActor);
	}
}