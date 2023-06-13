#pragma once

#include "../Common/header/Common.h"
#include "../../Maths/header/Vector3.h"
#include "../../Maths/header/Vector2.h"

namespace Renderer
{
	enum class EParticleProperty
	{
		ePosition,
		eScale,
		eAngle,
		eVelocity,
		eAcceleration,
		eAngularVelocity,
		eAngularAccelration,
		eDuration,
		eCurrentTime,
		eCurrentFrame,
		eFrameRate,
		eNumTextureRow,
		eNumTextureColumn,
		eInitialRow,
		eInitialColumn
	};

	class CParticle
	{
	public:
												CParticle(int pMemoryPoolIndex);

		void									Reset();

		int										GetMemoryPoolIndex() const;

		void									Update(const float& pDelatTime);

		Math::SVector3							mPosition;
		Math::SVector2							mScale;
		float									mAngle = 0;
		Math::SVector3							mVelocity;
		Math::SVector3							mAcceleration;
		float									mAngularVelocity = 0;
		float									mAngularAcceleration = 0;
		float									mDuration = 0;
		float									mCurrentTime = 0;
		int										mNumTextureRow = 0;
		int										mNumTextureColumn = 0;
		int										mCurrentRow = 0;
		int										mCurrentColumn = 0;
		float									mCurrentFrame = 0;
		float									mFrameRate = 0;

	private:

		int										mMemoryPoolIndex;

	};
}
