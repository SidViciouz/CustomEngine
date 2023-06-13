#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleEmitter.h"

namespace Renderer
{
	class CParticleSystem
	{
	public:
												CParticleSystem(int pMemoryPoolIndex);

		void									Reset();

		int										GetMemoryPoolIndex() const;

		void									AddParticleEmitter(shared_ptr<CParticleEmitter> pParticleEmitter);

		void									Update(shared_ptr<CParticleVertexBuffer> pVertexBuffer, const float& pDeltaTime, const Math::SVector3& pCameraPosition);

	private:

		int										mMemoryPoolIndex;

		vector<shared_ptr<CParticleEmitter>>	mParticleEmitters;
	};
}
