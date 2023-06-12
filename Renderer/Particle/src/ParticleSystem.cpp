#include "../Particle/header/ParticleSystem.h"

namespace Renderer
{
	CParticleSystem::CParticleSystem(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}


	void CParticleSystem::Reset()
	{

	}



	int CParticleSystem::GetMemoryPoolIndex() const
	{
		return mMemoryPoolIndex;
	}


	
	void CParticleSystem::AddParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter)
	{
		mParticleEmitters.push_back(pParticleEmitter);
	}
	
}