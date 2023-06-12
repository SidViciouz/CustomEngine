#include "../Particle/header/ParticleMemoryPool.h"

namespace Renderer
{
	CParticleMemoryPool::CParticleMemoryPool(int pPoolIndex, int pParticleNum, int pParticleEmitterNum, int pParticleSystemNum)
		: mPoolIndex{pPoolIndex}, mMaxParticleNum{ pParticleNum }, mMaxParticleEmitterNum{ pParticleEmitterNum }, mMaxParticleSystemNum{ pParticleSystemNum }
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
			mAllParticleSystems.push_back(make_shared<CParticleSystem>(mPoolIndex));
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
			mAllParticleEmitters.push_back(make_shared<CParticleEmitter>(mPoolIndex));
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
			mAllParticles.push_back(make_shared<CParticle>(mPoolIndex));
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



	int CParticleMemoryPool::GetParticleSystemNum() const
	{
		return mParticleSystemNum;
	}



	int CParticleMemoryPool::GetParticleEmitterNum() const
	{
		return mParticleEmitterNum;
	}



	int	CParticleMemoryPool::GetParticleNum() const
	{
		return mParticleNum;
	}



	bool CParticleMemoryPool::IsAvailable() const
	{
		return (mParticleSystemNum < mMaxParticleSystemNum) && (mParticleEmitterNum < mMaxParticleEmitterNum) && (mParticleNum < mMaxParticleNum);
	}
}