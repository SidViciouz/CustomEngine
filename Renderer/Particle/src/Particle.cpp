#include "../Particle/header/Particle.h"

namespace Renderer
{
	CParticle::CParticle(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}
	


	void CParticle::Reset()
	{
		for (int i = 0; i < 3; ++i)
		{
			mPosition.mElement[i] = 0;
			mVelocity.mElement[i] = 0;
			mAcceleration.mElement[i] = 0;
		}

		for (int i = 0; i < 2; ++i)
		{
			mScale.mElement[i] = 0;
		}

		mAngle = 0;
		mAngularVelocity = 0;
		mAngularAcceleration = 0;
		mDuration = 0;
		mCurrentTime = 0;
		mCurrentFrame = 0;
		mFrameRate = 0;
		mNumTextureRow = 1;
		mNumTextureColumn = 1;
		mCurrentRow = 0;
		mCurrentColumn = 0;
	}



	int	CParticle::GetMemoryPoolIndex() const
	{
		return mMemoryPoolIndex;
	}



	void CParticle::Update(const float& pDeltaTime)
	{
		mVelocity = mVelocity + mAcceleration * pDeltaTime;
		mPosition = mPosition + mVelocity * pDeltaTime;

		mAngularVelocity = mAngularVelocity + mAngularAcceleration * pDeltaTime;
		mAngle = mAngle + mAngularVelocity * pDeltaTime;

		mCurrentFrame += mFrameRate* pDeltaTime;

		int lFrameIndex = (int)mCurrentFrame;

		if (lFrameIndex >= mNumTextureRow * mNumTextureColumn)
		{
			mCurrentFrame = 0;
			lFrameIndex = 0;
		}

		mCurrentColumn = lFrameIndex % mNumTextureRow;
		mCurrentRow = lFrameIndex / mNumTextureRow;

		mCurrentTime += pDeltaTime;
	}
}