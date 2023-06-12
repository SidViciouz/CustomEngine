#include "../Particle/header/ParticleMemoryPool.h"

namespace Renderer
{
	CParticleMemoryPool::CParticleMemoryPool(int pParticleNum, int pParticleEmitterNum, int pParticleSystemNum)
		: mMaxParticleNum{ pParticleNum }, mMaxParticleEmitterNum{ pParticleEmitterNum }, mMaxParticleSystemNum{ pParticleSystemNum }
	{

	}



	shared_ptr<CParticleSystem> CParticleMemoryPool::GetParticleSystem()
	{
		if (!mReleasedParticleSystems.empty())
		{
			shared_ptr<CParticleSystem> lPop = mReleasedParticleSystems.top();
			mReleasedParticleSystems.pop();

			return lPop;
		}
		else if(mParticleSystemNum < mMaxParticleSystemNum)
		{
			mAllParticleSystems.push_back(make_shared<CParticleSystem>());
			++mParticleSystemNum;

			return mAllParticleSystems.back();
		}
		else
		{
			return nullptr;
		}
	}



	void CParticleMemoryPool::ReleaseParticleSystem(shared_ptr<CParticleSystem> pParticleSystem)
	{
		pParticleSystem->Reset();
		mReleasedParticleSystems.push(pParticleSystem);
	}



	shared_ptr<CParticleEmitter> CParticleMemoryPool::GetParticleEmitter()
	{
		if (!mReleasedParticleEmitters.empty())
		{
			shared_ptr<CParticleEmitter> lPop = mReleasedParticleEmitters.top();
			mReleasedParticleEmitters.pop();

			return lPop;
		}
		else if (mParticleEmitterNum < mMaxParticleEmitterNum)
		{
			mAllParticleEmitters.push_back(make_shared<CParticleEmitter>());
			++mParticleEmitterNum;

			return mAllParticleEmitters.back();
		}
		else
		{
			return nullptr;
		}
	}



	void CParticleMemoryPool::ReleaseParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter)
	{
		pParticleEmitter->Reset();
		mReleasedParticleEmitters.push(pParticleEmitter);
	}



	shared_ptr<CParticle> CParticleMemoryPool::GetParticle()
	{
		if (!mReleasedParticles.empty())
		{
			shared_ptr<CParticle> lPop = mReleasedParticles.top();
			mReleasedParticles.pop();

			return lPop;
		}
		else if (mParticleNum < mMaxParticleNum)
		{
			mAllParticles.push_back(make_shared<CParticle>());
			++mParticleNum;

			return mAllParticles.back();
		}
		else
		{
			return nullptr;
		}
	}



	void CParticleMemoryPool::ReleaseParticle(shared_ptr<CParticle> pParticle)
	{
		pParticle->Reset();
		mReleasedParticles.push(pParticle);
	}

}