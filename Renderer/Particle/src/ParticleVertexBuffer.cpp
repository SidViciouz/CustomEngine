#include "../Particle/header/ParticleVertexBuffer.h"

namespace Renderer
{
	CParticleVertexBuffer::CParticleVertexBuffer(int pParticleVertexNum)
	{
		mVertexBuffers.resize(pParticleVertexNum);
	}



	void CParticleVertexBuffer::AddParticleVertex(const SParticleVertex& pParticleVertex)
	{
		mVertexBuffers[mVertexCount++] = pParticleVertex;
	}



	void CParticleVertexBuffer::ClearBuffer()
	{
		mVertexCount = 0;
	}

}