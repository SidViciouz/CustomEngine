#include "../Animation/header/AnimationBlend2D.h"
#include "../../Maths/header/Vector2.h"
#include "../../Maths/header/Transform.h"
#include "../Animation/header/Animation.h"
#include "../../Maths/header/Util.h"

namespace Renderer
{
	CAnimationBlend2D::CAnimationBlend2D(string pName, const float& pX, const float& pY)
		: IAnimationBase{pName}, mConditionX{pX}, mConditionY{pY}
	{

	}

	void CAnimationBlend2D::SetAnimation(shared_ptr<CAnimation> pBottomLeft, shared_ptr<CAnimation> pBottomRight, shared_ptr<CAnimation> pTopLeft, shared_ptr<CAnimation> pTopRight)
	{
		mBottomLeft = pBottomLeft;
		mBottomRight = pBottomRight;
		mTopLeft = pTopLeft;
		mTopRight = pTopRight;
	}


	void CAnimationBlend2D::SetAxis(float pXMin, float pXMax, float pYMin, float pYMax)
	{
		mXMin = pXMin;
		mXMax = pXMax;
		mYMin = pYMin;
		mYMax = pYMax;
	}

	Math::CTransform CAnimationBlend2D::EvaluateGlobalTransform(const string& pBoneName, double pTime)
	{
		if (mConditionX <= mXMin)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomLeft->EvaluateGlobalTransform(pBoneName, pTime);
			}
			else if (mConditionY >= mYMax)
			{
				return mTopLeft->EvaluateGlobalTransform(pBoneName, pTime);
			}
			else
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();
				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());

				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::CTransform::Blend(mBottomLeft->EvaluateGlobalTransform(pBoneName, lBottomLeftTime), mTopLeft->EvaluateGlobalTransform(pBoneName, lTopLeftTime), pAlpha);
			}
		}
		else if (mConditionX >= mXMax)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomRight->EvaluateGlobalTransform(pBoneName, pTime);
			}
			else if (mConditionY >= mYMax)
			{
				return mTopRight->EvaluateGlobalTransform(pBoneName, pTime);
			}
			else
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());

				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::CTransform::Blend(mBottomRight->EvaluateGlobalTransform(pBoneName, lBottomRightTime), mTopRight->EvaluateGlobalTransform(pBoneName, lTopRightTime), pAlpha);
			}
		}
		else
		{
			double lAlphaX = (mConditionX - mXMin) / (mXMax - mXMin);

			if (mConditionY <= mYMin)
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());

				return Math::CTransform::Blend(mBottomLeft->EvaluateGlobalTransform(pBoneName, lBottomLeftTime), mBottomRight->EvaluateGlobalTransform(pBoneName, lBottomRightTime), lAlphaX);

			}
			else if (mConditionY >= mYMax)
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());

				return Math::CTransform::Blend(mTopLeft->EvaluateGlobalTransform(pBoneName, lTopLeftTime), mTopRight->EvaluateGlobalTransform(pBoneName, lTopRightTime), lAlphaX);
			}
			else
			{
				double lAlphaY = (mConditionY - mYMin) / (mYMax - mYMin);

				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());
				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());

				Math::CTransform lBottom = Math::CTransform::Blend(mBottomLeft->EvaluateGlobalTransform(pBoneName, lBottomLeftTime), mBottomRight->EvaluateGlobalTransform(pBoneName, lBottomRightTime), lAlphaX);
				Math::CTransform lTop = Math::CTransform::Blend(mTopLeft->EvaluateGlobalTransform(pBoneName, lTopLeftTime), mTopRight->EvaluateGlobalTransform(pBoneName, lTopRightTime), lAlphaX);

				return Math::CTransform::Blend(lBottom, lTop, lAlphaY);
			}
		}
	}

	Math::CTransform CAnimationBlend2D::EvaluateLocalTransform(const string& pBoneName, double pTime)
	{
		if (mConditionX <= mXMin)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomLeft->EvaluateLocalTransform(pBoneName, pTime);
			}
			else if (mConditionY >= mYMax)
			{
				return mTopLeft->EvaluateLocalTransform(pBoneName, pTime);
			}
			else
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();
				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());

				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::CTransform::Blend(mBottomLeft->EvaluateLocalTransform(pBoneName, lBottomLeftTime), mTopLeft->EvaluateLocalTransform(pBoneName, lTopLeftTime), pAlpha);
			}
		}
		else if (mConditionX >= mXMax)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomRight->EvaluateLocalTransform(pBoneName, pTime);
			}
			else if (mConditionY >= mYMax)
			{
				return mTopRight->EvaluateLocalTransform(pBoneName, pTime);
			}
			else
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());

				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::CTransform::Blend(mBottomRight->EvaluateLocalTransform(pBoneName, lBottomRightTime), mTopRight->EvaluateLocalTransform(pBoneName, lTopRightTime), pAlpha);
			}
		}
		else
		{
			double lAlphaX = (mConditionX - mXMin) / (mXMax - mXMin);

			if (mConditionY <= mYMin)
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());

				return Math::CTransform::Blend(mBottomLeft->EvaluateLocalTransform(pBoneName, lBottomLeftTime), mBottomRight->EvaluateLocalTransform(pBoneName, lBottomRightTime), lAlphaX);

			}
			else if (mConditionY >= mYMax)
			{
				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());

				return Math::CTransform::Blend(mTopLeft->EvaluateLocalTransform(pBoneName, lTopLeftTime), mTopRight->EvaluateLocalTransform(pBoneName, lTopRightTime), lAlphaX);
			}
			else
			{
				double lAlphaY = (mConditionY - mYMin) / (mYMax - mYMin);

				double lTimeSpan = GetEndTime() - GetBeginTime();

				double lTopLeftTime = pTime / lTimeSpan * (mTopLeft->GetEndTime() - mTopLeft->GetBeginTime());
				double lTopRightTime = pTime / lTimeSpan * (mTopRight->GetEndTime() - mTopRight->GetBeginTime());
				double lBottomLeftTime = pTime / lTimeSpan * (mBottomLeft->GetEndTime() - mBottomLeft->GetBeginTime());
				double lBottomRightTime = pTime / lTimeSpan * (mBottomRight->GetEndTime() - mBottomRight->GetBeginTime());

				Math::CTransform lBottom = Math::CTransform::Blend(mBottomLeft->EvaluateLocalTransform(pBoneName, lBottomLeftTime), mBottomRight->EvaluateLocalTransform(pBoneName, lBottomRightTime), lAlphaX);
				Math::CTransform lTop = Math::CTransform::Blend(mTopLeft->EvaluateLocalTransform(pBoneName, lTopLeftTime), mTopRight->EvaluateLocalTransform(pBoneName, lTopRightTime), lAlphaX);

				return Math::CTransform::Blend(lBottom, lTop, lAlphaY);
			}
		}
	}

	bool CAnimationBlend2D::IsBone(const string& pBoneName) const
	{
		if (!mBottomLeft->IsBone(pBoneName))
			return false;
		else if (!mBottomRight->IsBone(pBoneName))
			return false;
		else if (!mTopLeft->IsBone(pBoneName))
			return false;
		else if (!mTopRight->IsBone(pBoneName))
			return false;
		else
			return true;
	}
	
	double CAnimationBlend2D::GetBeginTime() const
	{
		if (mConditionX <= mXMin)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomLeft->GetBeginTime();
			}
			else if (mConditionY >= mYMax)
			{
				return mTopLeft->GetBeginTime();
			}
			else
			{
				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::Lerp(mBottomLeft->GetBeginTime(), mTopLeft->GetBeginTime(), pAlpha);
			}
		}
		else if (mConditionX >= mXMax)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomRight->GetBeginTime();
			}
			else if (mConditionY >= mYMax)
			{
				return mTopRight->GetBeginTime();
			}
			else
			{
				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::Lerp(mBottomRight->GetBeginTime(), mTopRight->GetBeginTime(), pAlpha);
			}
		}
		else
		{
			double lAlphaX = (mConditionX - mXMin) / (mXMax - mXMin);

			if (mConditionY <= mYMin)
			{
				return Math::Lerp(mBottomLeft->GetBeginTime(), mBottomRight->GetBeginTime(), lAlphaX);

			}
			else if (mConditionY >= mYMax)
			{

				return Math::Lerp(mTopLeft->GetBeginTime(), mTopRight->GetBeginTime(), lAlphaX);
			}
			else
			{
				double lAlphaY = (mConditionY - mYMin) / (mYMax - mYMin);

				double lTop = Math::Lerp(mTopLeft->GetBeginTime(), mTopRight->GetBeginTime(), lAlphaX);
				double lBottom = Math::Lerp(mBottomLeft->GetBeginTime(), mBottomRight->GetBeginTime(), lAlphaX);

				return Math::Lerp(lBottom, lTop, lAlphaY);
			}
		}
	}

	double CAnimationBlend2D::GetEndTime() const
	{
		if (mConditionX <= mXMin)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomLeft->GetEndTime();
			}
			else if (mConditionY >= mYMax)
			{
				return mTopLeft->GetEndTime();
			}
			else
			{
				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::Lerp(mBottomLeft->GetEndTime(), mTopLeft->GetEndTime(), pAlpha);
			}
		}
		else if (mConditionX >= mXMax)
		{
			if (mConditionY <= mYMin)
			{
				return mBottomRight->GetEndTime();
			}
			else if (mConditionY >= mYMax)
			{
				return mTopRight->GetEndTime();
			}
			else
			{
				double pAlpha = (mConditionY - mYMin) / (mYMax - mYMin);
				return Math::Lerp(mBottomRight->GetEndTime(), mTopRight->GetEndTime(), pAlpha);
			}
		}
		else
		{
			double lAlphaX = (mConditionX - mXMin) / (mXMax - mXMin);

			if (mConditionY <= mYMin)
			{
				return Math::Lerp(mBottomLeft->GetEndTime(), mBottomRight->GetEndTime(), lAlphaX);
			}
			else if (mConditionY >= mYMax)
			{
				return Math::Lerp(mTopLeft->GetEndTime(), mTopRight->GetEndTime(), lAlphaX);
			}
			else
			{
				double lAlphaY = (mConditionY - mYMin) / (mYMax - mYMin);

				double lTop = Math::Lerp(mTopLeft->GetEndTime(), mTopRight->GetEndTime(), lAlphaX);
				double lBottom = Math::Lerp(mBottomLeft->GetEndTime(), mBottomRight->GetEndTime(), lAlphaX);

				return Math::Lerp(lBottom, lTop, lAlphaY);
			}
		}
	}
}