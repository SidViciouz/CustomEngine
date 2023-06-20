#include "../Animation/header/AnimationGraph.h"
#include "../Mesh/header/Mesh.h"
#include "../Mesh/header/Skeleton.h"
#include <fbxsdk.h>

namespace Renderer
{
	CAnimation::CAnimation(string pName, const char* pPath) :
		mName{ pName }
	{
		FbxManager* lFbxManager = FbxManager::Create();

		FbxIOSettings* lFBxIOSettings = FbxIOSettings::Create(lFbxManager, IOSROOT);

		lFbxManager->SetIOSettings(lFBxIOSettings);

		FbxImporter* lImporter = FbxImporter::Create(lFbxManager, "myInporter");
		if (!lImporter->Initialize(pPath, -1, lFbxManager->GetIOSettings()))
			throw string("Initializing fbx importer fails.");

		FbxScene* lFbxScene = FbxScene::Create(lFbxManager, "myScene");

		lImporter->Import(lFbxScene);

		lImporter->Destroy();

		//store skeleton data
		FbxSkeleton* lFbxSkeleton = lFbxScene->GetSrcObject<FbxSkeleton>();

		if (lFbxSkeleton != nullptr)
		{
			FbxArray<FbxString*> lAnimStackNameArray;
			lFbxScene->FillAnimStackNameArray(lAnimStackNameArray);
			int lAnimStackCount = lAnimStackNameArray.GetCount();
			if (!lAnimStackCount)
				throw string("No anim stack.");

			FbxTakeInfo* lTakeInfo = lFbxScene->GetTakeInfo(*(lAnimStackNameArray[0]));
			if (lTakeInfo == nullptr)
				throw string("No take info.");

			mBeginTime = lTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
			mEndTime = lTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();

			mSkeleton = make_shared<CSkeleton>(lFbxSkeleton, nullptr, 0);
		}
	}

	shared_ptr<CSkeleton> CAnimation::GetSkeleton()
	{
		return mSkeleton;
	}

	const double& CAnimation::GetBeginTime() const
	{
		return mBeginTime;
	}

	const double& CAnimation::GetEndTime() const
	{
		return mEndTime;
	}

	CAnimationGraph::CAnimationGraph(shared_ptr<CSkeleton> pOutputSkeleton) :
		mOutputSkeleton{ pOutputSkeleton }
	{

	}

	void CAnimationGraph::LoadAnimation(const string& pName, const char* pPath)
	{
		shared_ptr<CAnimation> lAnim = make_shared<CAnimation>(pName, pPath);
		mAnimations[pName] = lAnim;
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

			while (lCurrentTime > lTimeSpan)
				lCurrentTime -= lTimeSpan;

			mAnimPhase = lCurrentTime / lTimeSpan;


			// link to output
			shared_ptr<CSkeleton> lSkeleton = mAnimations[mCurrentState.Node.mNode]->GetSkeleton();

			Bones::iterator lBegin, lEnd;
			mOutputSkeleton->GetBonesIterator(lBegin, lEnd);

			for (auto it = lBegin; it != lEnd; it++)
			{
				string lBoneName = it->first;
				shared_ptr<CBone> lOutputBone = it->second;
				shared_ptr<CBone> lAnimBone = lSkeleton->GetBone(lBoneName);

				if (lOutputBone == nullptr || lAnimBone == nullptr)
					continue;

				lOutputBone->SetGlobalTransform(lAnimBone->EvaluateGlobalTransform(lCurrentTime));
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

			while (lCurrentTime > lTimeSpan)
				lCurrentTime -= lTimeSpan;

			double lFromCurrentTime = lCurrentTime / lTimeSpan * lFromTimeSpan;
			double lToCurrentTime = lCurrentTime / lTimeSpan * lToTimeSpan;


			mAnimPhase = lCurrentTime / lTimeSpan;


			//blend animation and link it to output
			shared_ptr<CSkeleton> lSkeletonA = mAnimations[mCurrentState.Transition.mFrom]->GetSkeleton();
			shared_ptr<CSkeleton> lSkeletonB = mAnimations[mCurrentState.Transition.mTo]->GetSkeleton();

			//ToAllBones(lambda)
			Bones::iterator lBegin, lEnd;
			mOutputSkeleton->GetBonesIterator(lBegin, lEnd);

			// blend all bone's local transform
			for (auto it = lBegin; it != lEnd; it++)
			{
				string lBoneName = it->first;
				shared_ptr<CBone> lOutputBone = it->second;
				shared_ptr<CBone> lAnimBoneA = lSkeletonA->GetBone(lBoneName);
				shared_ptr<CBone> lAnimBoneB = lSkeletonB->GetBone(lBoneName);

				if (lOutputBone == nullptr || lAnimBoneA == nullptr || lAnimBoneB == nullptr)
					continue;

				const Math::CTransform lTransformA = lAnimBoneA->EvaluateLocalTransform(lFromCurrentTime);
				const Math::CTransform lTransformB = lAnimBoneB->EvaluateLocalTransform(lToCurrentTime);

				
				Math::SVector3 lTranslation = lTransformA.GetTranslation() * (1 - lAlpha) + lTransformB.GetTranslation() * lAlpha;
				Math::SQuaternion lOrientation = lTransformA.GetOrientation().Slerp(lTransformB.GetOrientation(), lAlpha);
				Math::SVector3 lScale = lTransformA.GetScale()* (1 - lAlpha) + lTransformB.GetScale() * lAlpha;
				Math::CTransform lLocalTransform;
				lLocalTransform.SetTranslation(lTranslation);
				lLocalTransform.SetOrientation(lOrientation);
				lLocalTransform.SetScale(lScale);
				
				lOutputBone->SetLocalTransform(lLocalTransform);
			}

			// stack local transforms up to make global transform
			mOutputSkeleton->UpdateGlobalFromLocal();
		}

	}
}