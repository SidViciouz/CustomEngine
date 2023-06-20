#pragma once

#include "AnimationBase.h"

namespace Math
{
	struct SVector2;
}

namespace Renderer
{
	class CAnimation;

	using AnimCoordinate = vector< pair<shared_ptr<CAnimation>, Math::SVector2>>;

	class CAnimationBlend2D : public IAnimationBase
	{
	public:
												CAnimationBlend2D(string pName);

		// set animation at pValue coordinates
		void									SetAnimation(shared_ptr<CAnimation> pAnimation, Math::SVector2 pValue);

		virtual shared_ptr<CSkeleton>			GetSkeleton() override;

		virtual const double&					GetBeginTime() const override;
		virtual const double&					GetEndTime() const override;

		AnimCoordinate							mAnimCoords;
	};
}
