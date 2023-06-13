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
	


	void CParticleSystem::Update(shared_ptr<CParticleVertexBuffer> pVertexBuffer, const float& pDeltaTime, const Math::SVector3& pEmitterDirection, const Math::SVector3& pCameraPosition)
	{
		for (auto& lEmitter : mParticleEmitters)
		{
			lEmitter->Update(pVertexBuffer,pDeltaTime,pEmitterDirection,pCameraPosition);
		}
	}

}