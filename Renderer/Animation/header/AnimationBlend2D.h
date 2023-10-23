#pragma once

#include "AnimationBase.h"

namespace Math
{
	struct SVector2;
	class CTransform;
}

namespace Renderer
{
	class CAnimation;

	class CAnimationBlend2D : public IAnimationBase
	{
	public:
												CAnimationBlend2D(string pName, const float& pX, const float& pY);

		void									SetAxis(float pXMin, float pXMax, float pYMin, float pYMax);

		void									SetAnimation(shared_ptr<CAnimation> pBottomLeft, shared_ptr<CAnimation> pBottomRight,
														shared_ptr<CAnimation> pTopLeft, shared_ptr<CAnimation> pTopRight);

		virtual Math::CTransform				EvaluateGlobalTransform(const string& pBoneName, double pTime) override;
		virtual Math::CTransform				EvaluateLocalTransform(const string& pBoneName, double pTime) override;

		virtual bool							IsBone(const string& pBoneName) const override;

		virtual double							GetBeginTime() const override;
		virtual double							GetEndTime() const override;


		const float&							mConditionX;
		const float&							mConditionY;

		float									mXMin;
		float									mXMax;
		float									mYMin;
		float									mYMax;

		shared_ptr<CAnimation>					mBottomLeft;
		shared_ptr<CAnimation>					mBottomRight;
		shared_ptr<CAnimation>					mTopLeft;
		shared_ptr<CAnimation>					mTopRight;
	};
}
