#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/Particle.h"

namespace Renderer
{
	class CParticleEmitter
	{
	public:
												CParticleEmitter();

		void									Reset();

	private:
		vector<shared_ptr<CParticle>>			mParticles;
	};
}
