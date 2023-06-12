#pragma once

#include "../Common/header/Common.h"
#include "../../Maths/header/Vector3.h"
#include "../../Maths/header/Vector2.h"

namespace Renderer
{
	class CParticle
	{
	public:
												CParticle();

		void									Reset();

	private:

		Math::SVector3							mPosition;
		Math::SVector2							mScale;
		Math::SVector3							mVelocity;
		Math::SVector3							mAcceleration;
		float									mDuration;
		float									mCurrentTime;
		Math::SVector2							mBottomLeft;
		Math::SVector2							mTopRight;
		int										mCurrentFrame;
	};
}
