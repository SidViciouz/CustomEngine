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
												CParticleVertexBuffer(int pParticleVertexNum);

		void									AddParticleVertex(const SParticleVertex& pParticleVertex);

		void									ClearBuffer();

	private:

		int										mVertexCount = 0;

		vector<SParticleVertex>					mVertexBuffers;
	};
}
