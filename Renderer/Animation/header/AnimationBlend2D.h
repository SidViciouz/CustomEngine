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

	using AnimCoordinate = vector< pair<shared_ptr<CAnimation>, Math::SVector2>>;

	class CAnimationBlend2D : public IAnimationBase
	{
	public:
												CAnimationBlend2D(string pName, const float& pX, const float& pY);

		// set animation at pValue coordinates
		void									SetAnimation(shared_ptr<CAnimation> pAnimation, Math::SVector2 pValue);

		virtual Math::CTransform				EvaluateGlobalTransform(const string& pBoneName, double pTime) override;
		virtual Math::CTransform				EvaluateLocalTransform(const string& pBoneName, double pTime) override;

		virtual bool							IsBone(const string& pBoneName) const override;

		virtual const double&					GetBeginTime() const override;
		virtual const double&					GetEndTime() const override;

		AnimCoordinate							mAnimCoords;

		const float&							mConditionX;
		const float&							mConditionY;
	};
}
