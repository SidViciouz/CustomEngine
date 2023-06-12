#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleSystem.h"
#include "../Particle/header/ParticleEmitter.h"
#include "../Particle/header/Particle.h"

namespace Renderer
{
	class CParticleMemoryPool
	{
	public:
												CParticleMemoryPool(int pParticleNum, int pParticleEmitterNum, int pParticleSystemNum);

		shared_ptr<CParticleSystem>				GetParticleSystem();
		void									ReleaseParticleSystem(shared_ptr<CParticleSystem> pParticleSystem);

		shared_ptr<CParticleEmitter>			GetParticleEmitter();
		void									ReleaseParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter);

		shared_ptr<CParticle>					GetParticle();
		void									ReleaseParticle(shared_ptr<CParticle> pParticle);

	private:
		vector<shared_ptr<CParticleSystem>>		mAllParticleSystems;
		vector<shared_ptr<CParticleEmitter>>	mAllParticleEmitters;
		vector<shared_ptr<CParticle>>			mAllParticles;

		stack<shared_ptr<CParticleSystem>>		mReleasedParticleSystems;
		stack<shared_ptr<CParticleEmitter>>		mReleasedParticleEmitters;
		stack<shared_ptr<CParticle>>			mReleasedParticles;

		int										mMaxParticleNum;
		int										mMaxParticleEmitterNum;
		int										mMaxParticleSystemNum;

		int										mParticleNum = 0;
		int										mParticleEmitterNum = 0;
		int										mParticleSystemNum = 0;
	};
}
