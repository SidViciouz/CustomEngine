#pragma once

#include "Common.h"

namespace Renderer
{
	class CSkeleton;

	class CAnimation
	{
	public:
												CAnimation(string pName,const char* pPath);

		shared_ptr<CSkeleton>					GetSkeleton();
	protected:
		string									mName;

		shared_ptr<CSkeleton>					mSkeleton;

		double									mBeginTime;
		double									mEndTime;

	};

	using Animations = unordered_map<string, shared_ptr<CAnimation>>;

	class CAnimationGraph
	{
	public:
												CAnimationGraph(shared_ptr<CSkeleton> pOutputSkeleton);
		void									LoadAnimation(string pName,const char* pPath);

		void									LinkToOutput(string pName);

	protected:

		shared_ptr<CSkeleton>					mOutputSkeleton;

		Animations								mAnimations;
	};
}


