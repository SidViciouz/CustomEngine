#include "../Particle/header/ParticleMemoryManager.h"

namespace Renderer
{
	shared_ptr<CParticleMemoryManager>	CParticleMemoryManager::mThis = nullptr;

	CParticleMemoryManager::CParticleMemoryManager(int pNumPools, int pNumParticles, int pNumEmitters, int pNumParticleSystems)
		: mNumPools{pNumPools}, mMaxParticleNum{ pNumParticles }, mMaxEmitterNum{ pNumEmitters }, mMaxParticleSystemNum{pNumParticleSystems}
	{
		int lParticlesPerPool = mMaxParticleNum / mNumPools;
		int lEmittersPerPool = mMaxEmitterNum / mNumPools;
		int lParticleSystemsPerPool = mMaxParticleSystemNum / mNumPools;

		for (int lPoolIndex = 0; lPoolIndex < pNumPools; ++lPoolIndex)
		{
			mParticleMemoryPools[lPoolIndex] = make_shared<CParticleMemoryPool>(lParticlesPerPool, lEmittersPerPool, lParticleSystemsPerPool);
		}
	}

	shared_ptr<CParticleMemoryManager> CParticleMemoryManager::Get()
	{
		if (mThis == nullptr)
		{
			mThis = make_shared<CParticleMemoryManager>(MAX_PARTICLE_MEMORY_POOL, MAX_PARTICLE_NUM, MAX_PARTICLE_EMITTER_NUM, MAX_PARTICLE_SYSTEM_NUM);
		}

		return mThis;
	}

}