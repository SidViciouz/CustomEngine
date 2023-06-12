#include "../Particle/header/Particle.h"

namespace Renderer
{
	CParticle::CParticle(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}
	


	void CParticle::Reset()
	{

	}



	int	CParticle::GetMemoryPoolIndex() const
	{
		return mMemoryPoolIndex;
	}
}