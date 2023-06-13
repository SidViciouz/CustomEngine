#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleMemoryManager.h"
#include "../Particle/header/ParticleSystem.h"
#include "../Particle/header/ParticleVertexBuffer.h"

namespace Renderer
{
	//manage resource about particles and render it
	class CParticleManager
	{
	public:
												CParticleManager();

		
		shared_ptr<CParticleSystem>				AddParticleSystem();

		shared_ptr<CParticleEmitter>			AddParticleEmitter(shared_ptr<CParticleSystem> pParticleSystem);
		
		template<typename T>
		void									SetParticleEmitterValue(shared_ptr<CParticleEmitter> pParticleEmitter, EParticleEmitterProperty pProperty, const T& pValue);
		
		void									Update();

	private:
	
		shared_ptr<CParticleMemoryManager>		mParticleMemoryManager;
		
		vector<shared_ptr<CParticleSystem>>		mParticleSystems[MAX_PARTICLE_MEMORY_POOL];

		shared_ptr<CParticleVertexBuffer>		mVertexBuffer;
	};

}
