#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleEmitter.h"

namespace Renderer
{
	class CParticleSystem
	{
	public:
												CParticleSystem();

		void									Reset();

	private:
		vector<shared_ptr<CParticleEmitter>>	mParticleEmitters;
	};
}
