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
		eAngularAcceleration,
		eDuration,
		eParticleDuration,
		eCurrentTime,
		eCurrentFrame,
		eFrameRate,
		eNumTextureRow,
		eNumTextureColumn,
		eInitialRow,
		eInitialColumn,
		eEmissionRate,
		ePositionVariance,
		eScaleVariance,
		eAngleVariance,
		eVelocityVariance,
		eAccelerationVariance,
		eAngularVelocityVariance,
		eAngularAccelerationVariance,
		eParticleDurationVariance,
		eFrameVariance
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
		float									mParticleDuration = 0;
		float									mCurrentTime = 0;
		float									mCurrentFrame = 0;
		float									mFrameRate = 0;
		int										mNumTextureRow = 0;
		int										mNumTextureColumn = 0;
		int										mInitialRow = 0;
		int										mInitialColumn = 0;
		float									mEmissionRate = 0;
		
		Math::SVector3							mPositionVariance;
		Math::SVector2							mScaleVariance;
		float									mAngleVariance = 0;
		Math::SVector3							mVelocityVariance;
		Math::SVector3							mAccelerationVariance;
		float									mAngularVelocityVariance = 0;
		float									mAngularAccelerationVariance = 0;
		float									mParticleDurationVariance = 0;
		float									mFrameVariance = 0;




		float									mEmissionUnderZero = 0;
		bool									mActivated = true;
	};
}
