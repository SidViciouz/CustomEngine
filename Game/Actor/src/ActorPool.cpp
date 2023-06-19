#include "Actor/header/ActorPool.h"

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
	{

	}
}