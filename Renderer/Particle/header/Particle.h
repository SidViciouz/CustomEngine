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
		eBottomLeft,
		eTopRight,
		eCurrentFrame,
	};

	class CParticle
	{
	public:
												CParticle(int pMemoryPoolIndex);

		void									Reset();

		int										GetMemoryPoolIndex() const;

	private:

		int										mMemoryPoolIndex;

		Math::SVector3							mPosition;
		Math::SVector2							mScale;
		float									mAngle;
		Math::SVector3							mVelocity;
		Math::SVector3							mAcceleration;
		float									mAngularVelocity;
		float									mAngularAcceleration;
		float									mDuration;
		float									mCurrentTime;
		Math::SVector2							mBottomLeft;
		Math::SVector2							mTopRight;
		int										mCurrentFrame;
	};
}
