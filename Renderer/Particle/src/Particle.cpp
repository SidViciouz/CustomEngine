#include "../Particle/header/Particle.h"

namespace Renderer
{
	CParticle::CParticle(int pMemoryPoolIndex) :
		mMemoryPoolIndex{pMemoryPoolIndex}
	{

	}
	


	void CParticle::Reset()
	{

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