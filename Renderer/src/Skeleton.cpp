#include "Skeleton.h"
#include <stack>
#include <utility>

namespace Renderer
{

	CBone::CBone(FbxNode* pLimbNode,int pBoneIndex) :
		mBoneIndex{pBoneIndex}
	{
		mName = pLimbNode->GetName();
		
		FbxAMatrix lLocalTransformMatrix = pLimbNode->EvaluateLocalTransform();
		FbxAMatrix lGlobalTransformMatrix = pLimbNode->EvaluateGlobalTransform();

		FbxVector4 lFbxLocalTranslation = lLocalTransformMatrix.GetT();
		Math::SVector3 lLocalTranslation(lFbxLocalTranslation[0], lFbxLocalTranslation[1], lFbxLocalTranslation[2]);

		FbxQuaternion lFbxLocalOrientation = lLocalTransformMatrix.GetQ();
		Math::SQuaternion lLocalOrientation(lFbxLocalOrientation[0], lFbxLocalOrientation[1], lFbxLocalOrientation[2], lFbxLocalOrientation[3]);

		FbxVector4 lFbxLocalScale = lLocalTransformMatrix.GetS();
		Math::SVector3 lLocalScale(lFbxLocalScale[0], lFbxLocalScale[1], lFbxLocalScale[2]);

		mLocalTransform.SetTranslation(lLocalTranslation);
		mLocalTransform.SetOrientation(lLocalOrientation);
		mLocalTransform.SetScale(lLocalScale);


		FbxVector4 lFbxGlobalTranslation = lGlobalTransformMatrix.GetT();
		Math::SVector3 lGlobalTranslation(lFbxGlobalTranslation[0], lFbxGlobalTranslation[1], lFbxGlobalTranslation[2]);

		FbxQuaternion lFbxGlobalOrientation = lGlobalTransformMatrix.GetQ();
		Math::SQuaternion lGlobalOrientation(lFbxGlobalOrientation[0], lFbxGlobalOrientation[1], lFbxGlobalOrientation[2], lFbxGlobalOrientation[3]);

		FbxVector4 lFbxGlobalScale = lGlobalTransformMatrix.GetS();
		Math::SVector3 lGlobalScale(lFbxGlobalScale[0], lFbxGlobalScale[1], lFbxGlobalScale[2]);

		mGlobalTransform.SetTranslation(lGlobalTranslation);
		mGlobalTransform.SetOrientation(lGlobalOrientation);
		mGlobalTransform.SetScale(lGlobalScale);
	}



	void CBone::AddChild(shared_ptr<CBone> pChild)
	{
		mChilds.push_back(pChild);
	}



	void CBone::SetParent(shared_ptr<CBone> pParent)
	{
		mParent = pParent;
	}



	int	CBone::GetIndex() const
	{
		return mBoneIndex;
	}



	const char* CBone::GetName() const
	{
		return mName;
	}



	shared_ptr<SCluster> CBone::GetCluster()
	{
		return mCluster;
	}



	void CBone::SetCluster(shared_ptr<SCluster> pCluster)
	{
		mCluster = pCluster;
	}



	Math::CTransform CBone::GetGlobalTransform() const
	{
		return mGlobalTransform;
	}



	Math::CTransform CBone::GetLocalTransform() const
	{
		return mLocalTransform;
	}



	void CBone::SetInverseInitialTransformMatrix(const Math::SMatrix4& pMatrix)
	{
		mInverseInitialTransformMatrix = pMatrix;
	}



	Math::SMatrix4 CBone::GetInverseInitialTransformMatrix() const
	{
		return mInverseInitialTransformMatrix;
	}




	CSkeleton::CSkeleton(FbxSkeleton* pFbxSkeleton, FbxSkin* pSkinDeformer, int pControlPointCount)
	{
		stack<pair<FbxNode*,shared_ptr<CBone>>> lStack;

		lStack.push({ pFbxSkeleton->GetNode(),nullptr });

		bool lIsRoot = true;

		//initialize bones
		while (!lStack.empty())
		{
			pair<FbxNode*, shared_ptr<CBone>> lTop = lStack.top();
			lStack.pop();

			FbxNode* lFbxNode = lTop.first;
			shared_ptr<CBone> lParentBone = lTop.second;

			shared_ptr<CBone> lNewBone = make_shared<CBone>(lFbxNode,mBoneCount++);

			if (lIsRoot)
			{
				mRootBone = lNewBone;
				lIsRoot = false;
			}

			lNewBone->SetParent(lParentBone);
			if (lParentBone != nullptr)
				lParentBone->AddChild(lNewBone);

			mBones[lFbxNode->GetName()] = lNewBone;

			int lChildCount = lFbxNode->GetChildCount();
			for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
			{
				lStack.push({ lFbxNode->GetChild(lChildIndex), lNewBone });
			}
		}


		//set cluster to bones
		if (pSkinDeformer != nullptr)
		{
			mHasDeformer = true;

			mControlPointToBones.resize(pControlPointCount);

			int lClusterCount = pSkinDeformer->GetClusterCount();

			for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
			{
				FbxCluster* lFbxCluster = pSkinDeformer->GetCluster(lClusterIndex);

				const char* lLinkName = lFbxCluster->GetLink()->GetName();

				if (mBones[lLinkName] != nullptr)
				{
					shared_ptr<SCluster> lCluster = make_shared<SCluster>();
				
					lCluster->mCount = lFbxCluster->GetControlPointIndicesCount();

					int* lControlPointIndices = lFbxCluster->GetControlPointIndices();
					double* lControlPointWeights = lFbxCluster->GetControlPointWeights();

					for (int i = 0; i < lCluster->mCount; ++i)
					{
						lCluster->mIndices.push_back(static_cast<uint16_t>(lControlPointIndices[i]));
						lCluster->mWeights.push_back(static_cast<double>(lControlPointWeights[i]));

						mControlPointToBones[lControlPointIndices[i]].push_back({ mBones[lLinkName]->GetIndex(), lControlPointWeights[i] });
					}

					mBones[lLinkName]->SetCluster(lCluster);

					//make initial transform matrix
					FbxAMatrix lFbxInvinit;
					lFbxInvinit = lFbxCluster->GetTransformLinkMatrix(lFbxInvinit).Inverse();

					FbxAMatrix lInvReference;
					lInvReference = lFbxCluster->GetTransformMatrix(lInvReference);


					lFbxInvinit = lFbxInvinit * lInvReference;
				
					Math::SMatrix4 lInvInit;
					for (int lRow = 0; lRow < 4; ++lRow)
						for (int lColumn = 0; lColumn < 4; ++lColumn)
						{
							lInvInit.mElement[lRow][lColumn] = lFbxInvinit[lRow][lColumn];
						}

					mBones[lLinkName]->SetInverseInitialTransformMatrix(lInvInit);
				}
			}
		}
	}



	void CSkeleton::GetBoneTransformMatrix(vector<Math::SMatrix4>& pMatrices) const
	{
		pMatrices.resize(mBoneCount);

		for (auto it = mBones.cbegin(); it != mBones.cend(); it++)
		{
			if (it->second != nullptr)
			{
				Math::SMatrix4 lGlobal = Math::SMatrix4::Transform(it->second->GetGlobalTransform());

				pMatrices[it->second->GetIndex()] = it->second->GetInverseInitialTransformMatrix() * lGlobal;
			}
		}
	}



	int CSkeleton::GetBoneCount() const
	{
		return mBoneCount;
	}



	const vector<pair<int,double>>& CSkeleton::GetControlPointToBones(int pControlPointIndex)
	{
		return mControlPointToBones[pControlPointIndex];
	}
}