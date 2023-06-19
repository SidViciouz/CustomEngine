#include "World/header/World.h"
#include "Actor/header/Actor.h"
#include "../Camera/header/Camera.h"
#include "../Particle/header/ParticleManager.h"

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
	}

	shared_ptr<Renderer::CCamera> CWorld::GetCamera() const
	{
		return mCamera;
	}

	shared_ptr<Renderer::CParticleManager>	CWorld::GetParticleManager() const
	{
		return mParticleManager;
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