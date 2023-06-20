#include "../Animation/header/Animation.h"
#include "../Mesh/header/Skeleton.h"
#include <fbxsdk.h>

namespace Renderer
{
	CAnimation::CAnimation(string pName, const char* pPath) :
		IAnimationBase{pName}
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
}
