#include "AnimationGraph.h"
#include "Mesh.h"
#include "Skeleton.h"
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

			printf("%f %f %d\n", mBeginTime, mEndTime, mSkeleton->GetBoneCount());
		}
	}

	shared_ptr<CSkeleton> CAnimation::GetSkeleton()
	{
		return mSkeleton;
	}

	CAnimationGraph::CAnimationGraph(shared_ptr<CSkeleton> pOutputSkeleton) :
		mOutputSkeleton{pOutputSkeleton}
	{

	}

	void CAnimationGraph::LoadAnimation(string pName, const char* pPath)
	{
		shared_ptr<CAnimation> lAnim = make_shared<CAnimation>(pName, pPath);
		mAnimations[pName] = lAnim;
	}

	void CAnimationGraph::LinkToOutput(string pName)
	{
		if (mAnimations[pName] == nullptr)
			return;

		shared_ptr<CSkeleton> lSkeleton = mAnimations[pName]->GetSkeleton();

		Bones::iterator lBegin, lEnd;
		mOutputSkeleton->GetBonesIterator(lBegin, lEnd);

		for (auto it = lBegin; it != lEnd; it++)
		{
			string lBoneName = it->first;
			shared_ptr<CBone> lOutputBone = it->second;
			shared_ptr<CBone> lAnimBone = lSkeleton->GetBone(lBoneName);

			if (lOutputBone == nullptr || lAnimBone == nullptr)
				continue;

			lOutputBone->SetGlobalTransform(lAnimBone->EvaluateGlobalTransform(0.5f));
		}
	}
}