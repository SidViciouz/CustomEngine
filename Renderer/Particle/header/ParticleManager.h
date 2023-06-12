#pragma once

#include "../Common/header/Common.h"
#include "../Particle/header/ParticleMemoryManager.h"

namespace Renderer
{
	//manage resource about particles and render it
	class CParticleManager
	{
	public:
												CParticleManager();


	private:
	
		shared_ptr<CParticleMemoryManager>		mParticleMemoryManager;

	};

}
