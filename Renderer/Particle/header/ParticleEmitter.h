#pragma once

#include "../Common/header/Common.h"
#include "../../Maths/header/Vector2.h"
#include "../../Maths/header/Vector3.h"
#include "../Particle/header/Particle.h"

namespace Renderer
{
	enum class EParticleEmitterProperty
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
		eNumTextureRow,
		eNumTextureColumn
	};

	class CParticleEmitter
	{
	public:
												CParticleEmitter(int pMemoryPoolIndex);

		void									Reset();

		int										GetMemoryPoolIndex() const;
		
		template<typename T>
		void									SetProperty(EParticleEmitterProperty pProperty, const T& pValue);
		
	private:

		int										mMemoryPoolIndex;

		vector<shared_ptr<CParticle>>			mParticles;

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
		int										mNumTextureRow;
		int										mNumTextureColumn;
	};
}
