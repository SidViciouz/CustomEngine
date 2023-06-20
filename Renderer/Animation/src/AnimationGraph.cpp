#include "../Animation/header/AnimationGraph.h"
#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Skeleton.h"
#include "../Animation/header/AnimationBase.h"
#include "../Animation/header/Animation.h"
#include <fbxsdk.h>

namespace Renderer
{
	CAnimationGraph::CAnimationGraph(shared_ptr<CSkeleton> pOutputSkeleton) :
		mOutputSkeleton{ pOutputSkeleton }
	{

	}

	void CAnimationGraph::LoadAnimation(const string& pName, const char* pPath)
	{
		mAnimations[pName] = make_shared<CAnimation>(pName, pPath);
	}

	void CAnimationGraph::LoadAnimation(const string& pName, shared_ptr<IAnimationBase> pAnimation)
	{
		mAnimations[pName] = pAnimation;
	}

	void CAnimationGraph::AddTransition(const string& pFrom,const string& pTo,const std::function<bool(void)>& pCondition,const double& pDuration)
	{
		STransition lTransition(pTo, pCondition, pDuration);
		mTransitions[pFrom].push_back(lTransition);
	}

	bool CAnimationGraph::Reset(const string& pName)
	{
		if (mAnimations[pName] == nullptr)
			return false;

		mCurrentState.mState = EAnimState::eNode;
		mCurrentState.Node.mNode = pName;

		return true;
	}

	void CAnimationGraph::Update(float pDeltaTime)
	{
		if (mAnimations.empty())
			return;

		double lTimeSpan = 0;
		double lCurrentTime = 0;

		// check conditions
		if (mCurrentState.mState == EAnimState::eNode)
		{
			auto lTransitionBegin = mTransitions[mCurrentState.Node.mNode].begin();
			auto lTransitionEnd = mTransitions[mCurrentState.Node.mNode].end();

			for (auto it = lTransitionBegin; it != lTransitionEnd; it++)
			{
				//if condition is fulfilled, switch to transition state
				if (it->mCondition())
				{
					mCurrentState.mState = EAnimState::eTransition;
					mCurrentState.Transition.mFrom = mCurrentState.Node.mNode;
					mCurrentState.Transition.mTo = it->mTo;
					mCurrentState.Transition.mProgress = 0;
					mCurrentState.Transition.mDuration = it->mDuration;
					break;
				}
			}
		}
		else
		{
			if (mCurrentState.Transition.mProgress >= 1.0f)
			{
				mCurrentState.mState = EAnimState::eNode;
				mCurrentState.Node.mNode = mCurrentState.Transition.mTo;
			}
			
			mCurrentState.Transition.mProgress += pDeltaTime / mCurrentState.Transition.mDuration;
		}



		if (mCurrentState.mState == EAnimState::eNode)
		{
			// update time

			const double& lBeginTime = mAnimations[mCurrentState.Node.mNode]->GetBeginTime();
			const double& lEndTime = mAnimations[mCurrentState.Node.mNode]->GetEndTime();

			lTimeSpan = lEndTime - lBeginTime;

			lCurrentTime = mAnimPhase * lTimeSpan + pDeltaTime;

			if (lCurrentTime > lTimeSpan)
				lCurrentTime -= lTimeSpan;

			mAnimPhase = lCurrentTime / lTimeSpan;


			// link to output
			Bones::iterator lBegin, lEnd;
			mOutputSkeleton->GetBonesIterator(lBegin, lEnd);

			for (auto it = lBegin; it != lEnd; it++)
			{
				string lBoneName = it->first;
				shared_ptr<CBone> lOutputBone = it->second;

				if (lOutputBone == nullptr || !mAnimations[mCurrentState.Node.mNode]->IsBone(lBoneName))
					continue;

				lOutputBone->SetGlobalTransform(mAnimations[mCurrentState.Node.mNode]->EvaluateGlobalTransform(lBoneName,lCurrentTime));
			}
		}
		else // in case of transition
		{
			// blend animation

			// update time
			const double& lFromBegin = mAnimations[mCurrentState.Transition.mFrom]->GetBeginTime();
			const double& lFromEnd = mAnimations[mCurrentState.Transition.mFrom]->GetEndTime();

			const double& lToBegin = mAnimations[mCurrentState.Transition.mTo]->GetBeginTime();
			const double& lToEnd = mAnimations[mCurrentState.Transition.mTo]->GetEndTime();

			double lAlpha = mCurrentState.Transition.mProgress;
			if (lAlpha > 1.0f)
				lAlpha = 1.0f;

			double lFromTimeSpan = lFromEnd - lFromBegin;
			double lToTimeSpan = lToEnd - lToBegin;

			lTimeSpan = lFromTimeSpan * (1.0f - lAlpha) + lToTimeSpan * lAlpha;

			lCurrentTime = mAnimPhase * lTimeSpan + pDeltaTime;

			if (lCurrentTime > lTimeSpan)
				lCurrentTime -= lTimeSpan;

			double lFromCurrentTime = lCurrentTime / lTimeSpan * lFromTimeSpan;
			double lToCurrentTime = lCurrentTime / lTimeSpan * lToTimeSpan;


			mAnimPhase = lCurrentTime / lTimeSpan;


			//blend animation and link it to output

			//ToAllBones(lambda)
			Bones::iterator lBegin, lEnd;
			mOutputSkeleton->GetBonesIterator(lBegin, lEnd);

			// blend all bone's local transform
			for (auto it = lBegin; it != lEnd; it++)
			{
				string lBoneName = it->first;
				shared_ptr<CBone> lOutputBone = it->second;

				if (lOutputBone == nullptr || !mAnimations[mCurrentState.Transition.mFrom]->IsBone(lBoneName) || !mAnimations[mCurrentState.Transition.mTo]->IsBone(lBoneName))
					continue;

				const Math::CTransform lTransformA = mAnimations[mCurrentState.Transition.mFrom]->EvaluateLocalTransform(lBoneName,lFromCurrentTime);
				const Math::CTransform lTransformB = mAnimations[mCurrentState.Transition.mTo]->EvaluateLocalTransform(lBoneName,lToCurrentTime);
				
				lOutputBone->SetLocalTransform(Math::CTransform::Blend(lTransformA, lTransformB,lAlpha));
			}

			// stack local transforms up to make global transform
			mOutputSkeleton->UpdateGlobalFromLocal();
		}

	}
}