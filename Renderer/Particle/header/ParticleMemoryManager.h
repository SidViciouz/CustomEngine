#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleMemoryPool.h"

namespace Renderer
{
	class CParticleMemoryManager
	{
	public:
												CParticleMemoryManager(int pNumPools, int pNumParticles,int pNumEmitters,int pNumParticleSystems);
												CParticleMemoryManager(const CParticleMemoryManager& pPMM) = delete;
												CParticleMemoryManager(CParticleMemoryManager&& pPMM) = delete;
		CParticleMemoryManager&					operator=(const CParticleMemoryManager& pPMM) = delete;
		CParticleMemoryManager&					operator=(CParticleMemoryManager&& pPMM) = delete;

		static shared_ptr<CParticleMemoryManager>	Get();

		//										GetNewParticle(int pPoolIndex);
		//										ReleaseParticle(Particle* pParticle);

	private:

		shared_ptr<CParticleMemoryPool>			mParticleMemoryPools[MAX_PARTICLE_MEMORY_POOL];

		static shared_ptr<CParticleMemoryManager>	mThis;

		int										mNumPools;
		int										mMaxParticleNum;
		int										mMaxEmitterNum;
		int										mMaxParticleSystemNum;
	};
}
