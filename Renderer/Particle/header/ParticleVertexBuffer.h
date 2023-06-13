#pragma once

#include "../Common/header/Common.h"
#include "../../Maths/header/Vector3.h"
#include "../../Maths/header/Vector2.h"

namespace Renderer
{
	struct SParticleVertex
	{
		Math::SVector3 mPosition;
		Math::SVector2 mTextureCoord;
	};

	class CParticleVertexBuffer
	{
	public:
												CParticleVertexBuffer(int pNumPools, int pParticleNumPerPool);

		void									AddParticleVertex(int pPoolIndex, const array<SParticleVertex,4>& pParticleVertex);

		void									ClearBuffer(int pPoolIndex);

		int										GetVertexCount(int pPoolIndex);
		int										GetIndexCount(int pPoolIndex);

		const vector<SParticleVertex>&			GetVertexBuffer(int pPoolIndex);
		const vector<uint16_t>&					GetIndexBuffer(int pPoolIndex);

	private:

		int										mNumPools;

		int										mVertexCount[MAX_PARTICLE_MEMORY_POOL];
		int										mIndexCount[MAX_PARTICLE_MEMORY_POOL];

		vector<SParticleVertex>					mVertexBuffers[MAX_PARTICLE_MEMORY_POOL];
		vector<uint16_t>						mIndexBuffers[MAX_PARTICLE_MEMORY_POOL];
	};
}
