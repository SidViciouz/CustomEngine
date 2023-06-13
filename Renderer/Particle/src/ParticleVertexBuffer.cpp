#include "../Particle/header/ParticleVertexBuffer.h"

namespace Renderer
{
	CParticleVertexBuffer::CParticleVertexBuffer(int pNumPools, int pParticleNum) :
		mNumPools{pNumPools}
	{
		int lVertexNum = pParticleNum * 4;
		int lIndexNum = pParticleNum * 6;

		for (int lPoolIndex = 0; lPoolIndex < pNumPools; ++lPoolIndex)
		{
			mVertexBuffers[lPoolIndex].resize(lVertexNum);
			mVertexCount[lPoolIndex] = 0;

			mIndexBuffers[lPoolIndex].resize(lIndexNum);
			mIndexCount[lPoolIndex] = 0;
		}
	}



	void CParticleVertexBuffer::AddParticleVertex(int pPoolIndex, const array<SParticleVertex, 4>& pParticleVertex)
	{
		int lIndex = mVertexCount[pPoolIndex];

		mVertexBuffers[pPoolIndex][mVertexCount[pPoolIndex]++] = pParticleVertex[0];
		mVertexBuffers[pPoolIndex][mVertexCount[pPoolIndex]++] = pParticleVertex[1];
		mVertexBuffers[pPoolIndex][mVertexCount[pPoolIndex]++] = pParticleVertex[2];
		mVertexBuffers[pPoolIndex][mVertexCount[pPoolIndex]++] = pParticleVertex[3];

		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex;
		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex + 1;
		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex + 2;
		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex;
		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex + 2;
		mIndexBuffers[pPoolIndex][mIndexCount[pPoolIndex]++] = lIndex + 3;
	}



	void CParticleVertexBuffer::ClearBuffer(int pPoolIndex)
	{
		mVertexCount[pPoolIndex] = 0;
		mIndexCount[pPoolIndex] = 0;
	}

}