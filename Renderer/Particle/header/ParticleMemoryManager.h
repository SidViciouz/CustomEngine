#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleMemoryPool.h"
#include <random>

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

		shared_ptr<CParticleSystem>				GetParticleSystem();
		void									ReleaseParticleSystem(shared_ptr<CParticleSystem> pParticleSystem);

		shared_ptr<CParticleEmitter>			GetParticleEmitter(int pMemoryPoolIndex);
		void									ReleaseParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter);

		shared_ptr<CParticle>					GetParticle(int pMemoryPoolIndex);
		void									ReleaseParticle(shared_ptr<CParticle> pParticle);

		template<typename T>
		T										GetRandomValue(int pMemoryPoolIndex,const T& pMin,const T& pMax);

	private:

		int										GetNextMemoryPoolIndex();

		shared_ptr<CParticleMemoryPool>			mParticleMemoryPools[MAX_PARTICLE_MEMORY_POOL];

		shared_ptr<mt19937>						mRandomGenerator[MAX_PARTICLE_MEMORY_POOL];

		static shared_ptr<CParticleMemoryManager>	mThis;

		int										mNumPools;
		int										mMaxParticleNum;
		int										mMaxEmitterNum;
		int										mMaxParticleSystemNum;
	};
}
