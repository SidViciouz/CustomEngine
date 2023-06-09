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

	enum class EAnimState
	{
		eNode,
		eTransition
	};

	struct SAnimState
	{
												SAnimState() :
													mState{ EAnimState::eNode }, Transition{"","",0,0} {}
												~SAnimState(){}
		EAnimState mState;
		
		union
		{
			struct
			{
				string mFrom;
				string mTo;
				double mProgress;
				double mDuration;
			} Transition;

			struct
			{
				string mNode;
			} Node;
		};
	};

	struct STransition
	{
												STransition(const string& pTo,const std::function<bool(void)>& pCondition,const double& pDuration) : 
													mTo{ pTo }, mCondition{ pCondition }, mDuration{ pDuration } {}

		string mTo;
		std::function<bool(void)> mCondition;
		double mDuration;
	};

	using Animations = unordered_map<string, shared_ptr<CAnimation>>;

	using Transitions = unordered_map<string, vector<STransition>>;

	class CAnimationGraph
	{
	public:
												CAnimationGraph(shared_ptr<CSkeleton> pOutputSkeleton);

		void									LoadAnimation(const string& pName,const char* pPath);

		void									AddTransition(const string& pFrom,const string& pTo,const std::function<bool(void)>& pCondition,const double& pDuration);

		bool									Reset(const string& pName);

		void									Update(float pDeltaTime);

	protected:

		shared_ptr<CSkeleton>					mOutputSkeleton;

		Animations								mAnimations;

		string									mOutputAnimName;

		// this value is in [0,1], and is translated to specific time depending on animation
		double									mAnimPhase = 0.0f; 
		
		SAnimState								mCurrentState;

		Transitions								mTransitions;
	};
}


