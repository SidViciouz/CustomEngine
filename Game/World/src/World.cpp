#include "World/header/World.h"
#include "Actor/header/Actor.h"

namespace Game
{
	shared_ptr<CWorld> CWorld::Create()
	{
		return shared_ptr<CWorld>(new CWorld{});
	}

	CWorld::CWorld()
	{

	}
	
	CWorld::~CWorld()
	{

	}

	void CWorld::Update()
	{
		for (auto& lActor : mActors)
		{
			lActor->Update();
		}
	}

	void CWorld::Add(shared_ptr<CActor> pActor)
	{
		mActors.push_back(pActor);
	}

	template<class F, class... Args>
	void CWorld::ToAllActors(F pFunction, Args... pArgs)
	{
		for (auto& lActor : mActors)
		{
			pFunction(pArgs, lActor);
		}
	}
}