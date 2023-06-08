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

		const double&							GetBeginTime() const;
		const double&							GetEndTime() const;

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

		void									Update(float pDeltaTime);

	protected:

		shared_ptr<CSkeleton>					mOutputSkeleton;

		Animations								mAnimations;

		string									mOutputAnimName;

		// this value is in [0,1], and is translated to specific time depending on animation
		double									mAnimPhase = 0.0f; 
	};
}


