#pragma once

#include "../Common/header/Common.h"
#include "../Animation/header/AnimationBase.h"

namespace Renderer
{
	class CSkeleton;

	class CAnimation : public IAnimationBase
	{
	public:
												CAnimation(string pName, const char* pPath);

		virtual shared_ptr<CSkeleton>			GetSkeleton() override;

		virtual const double&					GetBeginTime() const override;
		virtual const double&					GetEndTime() const override;
	};
}