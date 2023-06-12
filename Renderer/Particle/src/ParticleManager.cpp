#include "../Particle/header/ParticleManager.h"

namespace Renderer
{
	CParticleManager::CParticleManager()
	{
		mParticleMemoryManager = CParticleMemoryManager::Get();
	}
}