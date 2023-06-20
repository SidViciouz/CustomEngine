#pragma once

#include "../Common/header/Common.h"

namespace Renderer
{
	class CSkeleton;

	class IAnimationBase
	{
	public:
												IAnimationBase(string pName);

		virtual shared_ptr<CSkeleton>			GetSkeleton() = 0;

		virtual const double&					GetBeginTime() const = 0;
		virtual const double&					GetEndTime() const = 0;

	protected:
		string									mName;

		shared_ptr<CSkeleton>					mSkeleton;

		double									mBeginTime;
		double									mEndTime;

	};
}