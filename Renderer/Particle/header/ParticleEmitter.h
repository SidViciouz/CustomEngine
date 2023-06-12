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

	class CParticleVertexBuffer;

	class CParticleEmitter
	{
	public:
												CParticleEmitter(int pMemoryPoolIndex);

		void									Reset();

		int										GetMemoryPoolIndex() const;
		
		template<typename T>
		void									SetProperty(EParticleEmitterProperty pProperty, const T& pValue);

		void									Update(shared_ptr<CParticleVertexBuffer> pVertexBuffer,const float& pDeltaTime, const Math::SVector3& pCameraDirection, const Math::SVector3& pCameraPosition);
		
	private:

		int										mMemoryPoolIndex;

		vector<shared_ptr<CParticle>>			mParticles;

		Math::SVector3							mPosition;
		Math::SVector2							mScale;
		float									mAngle = 0;
		Math::SVector3							mVelocity;
		Math::SVector3							mAcceleration;
		float									mAngularVelocity = 0;
		float									mAngularAcceleration = 0;
		float									mDuration = 0;
		float									mCurrentTime = 0;
		Math::SVector2							mBottomLeft;
		Math::SVector2							mTopRight;
		int										mCurrentFrame = 0;
		int										mNumTextureRow = 0;
		int										mNumTextureColumn = 0;
	};
}
