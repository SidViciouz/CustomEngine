#include "../Mesh/header/Skeleton.h"
#include <stack>
#include <utility>

namespace Renderer
{

	CBone::CBone(FbxNode* pLimbNode, int pBoneIndex) :
		mBoneIndex{ pBoneIndex }, mLimbNode{ pLimbNode }
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



	void CBone::GetChildIterators(vector<shared_ptr<CBone>>::iterator& lBegin, vector<shared_ptr<CBone>>::iterator& lEnd)
	{
		lBegin = mChilds.begin();
		lEnd = mChilds.end();
	}




	shared_ptr<CBone> CBone::GetParent() const
	{
		return mParent;
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



	void CBone::SetGlobalTransform(const Math::CTransform& pTransform)
	{
		mGlobalTransform = pTransform;
	}




	Math::CTransform CBone::GetLocalTransform() const
	{
		return mLocalTransform;
	}



	void CBone::SetLocalTransform(const Math::CTransform& pTransform)
	{
		mLocalTransform = pTransform;
	}



	void CBone::SetInverseInitialTransformMatrix(const Math::SMatrix4& pMatrix)
	{
		mInverseInitialTransformMatrix = pMatrix;
	}



	Math::SMatrix4 CBone::GetInverseInitialTransformMatrix() const
	{
		return mInverseInitialTransformMatrix;
	}



	Math::CTransform CBone::EvaluateGlobalTransform(double pTime)
	{
		FbxTime lFbxTime;
		lFbxTime.SetSecondDouble(pTime);

		FbxAMatrix lMatrix = mLimbNode->EvaluateGlobalTransform(lFbxTime);

		FbxVector4 lScale = lMatrix.GetS();
		FbxQuaternion lOrientation = lMatrix.GetQ();
		FbxVector4 lTranslation = lMatrix.GetT();

		Math::CTransform lTransform;

		lTransform.SetScale(Math::SVector3(lScale[0], lScale[1], lScale[2]));
		lTransform.SetOrientation(Math::SQuaternion(lOrientation[0], lOrientation[1], lOrientation[2], lOrientation[3]));
		lTransform.SetTranslation(Math::SVector3(lTranslation[0], lTranslation[1], lTranslation[2]));

		return lTransform;
	}



	Math::CTransform CBone::EvaluateLocalTransform(double pTime)
	{
		FbxTime lFbxTime;
		lFbxTime.SetSecondDouble(pTime);

		FbxAMatrix lMatrix = mLimbNode->EvaluateLocalTransform(lFbxTime);

		FbxVector4 lScale = lMatrix.GetS();
		FbxQuaternion lOrientation = lMatrix.GetQ();
		FbxVector4 lTranslation = lMatrix.GetT();

		Math::CTransform lTransform;

		lTransform.SetScale(Math::SVector3(lScale[0], lScale[1], lScale[2]));
		lTransform.SetOrientation(Math::SQuaternion(lOrientation[0], lOrientation[1], lOrientation[2], lOrientation[3]));
		lTransform.SetTranslation(Math::SVector3(lTranslation[0], lTranslation[1], lTranslation[2]));

		return lTransform;
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

					// FbxAMatrix is multiplied like column major
					lFbxInvinit =  lFbxInvinit * lInvReference;
				
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



	void CSkeleton::GetBonesIterator(Bones::iterator& pBegin, Bones::iterator& pEnd)
	{
		pBegin = mBones.begin();
		pEnd = mBones.end();
	}



	shared_ptr<CBone> CSkeleton::GetBone(const string& pName)
	{
		return mBones[pName];
	}



	void CSkeleton::UpdateGlobalFromLocal()
	{
		stack<shared_ptr<CBone>> lStack;

		lStack.push(mRootBone);

		while (!lStack.empty())
		{
			shared_ptr<CBone> lTop = lStack.top();
			lStack.pop();

			if (lTop == nullptr)
				continue;

			shared_ptr<CBone> lParent = lTop->GetParent();
			if (lParent != nullptr)
			{
				Math::CTransform lLocalTransform = lTop->GetLocalTransform();
				const Math::SVector3 lLocalScale = lLocalTransform.GetScale();
				const Math::SQuaternion lLocalOrientation = lLocalTransform.GetOrientation();
				const Math::SVector3 lLocalTranslation = lLocalTransform.GetTranslation();

				FbxAMatrix lLocalMatrix;
				lLocalMatrix.SetS(FbxVector4(lLocalScale.mX, lLocalScale.mY, lLocalScale.mZ));
				lLocalMatrix.SetQ(FbxQuaternion(lLocalOrientation.mX, lLocalOrientation.mY, lLocalOrientation.mZ, lLocalOrientation.mW));
				lLocalMatrix.SetT(FbxVector4(lLocalTranslation.mX, lLocalTranslation.mY, lLocalTranslation.mZ));


				Math::CTransform lParentTransform = lParent->GetGlobalTransform();
				const Math::SVector3 lParentScale = lParentTransform.GetScale();
				const Math::SQuaternion lParentOrientation = lParentTransform.GetOrientation();
				const Math::SVector3 lParentTranslation = lParentTransform.GetTranslation();

				FbxAMatrix lParentMatrix;
				lParentMatrix.SetS(FbxVector4(lParentScale.mX, lParentScale.mY, lParentScale.mZ));
				lParentMatrix.SetQ(FbxQuaternion(lParentOrientation.mX, lParentOrientation.mY, lParentOrientation.mZ, lParentOrientation.mW));
				lParentMatrix.SetT(FbxVector4(lParentTranslation.mX, lParentTranslation.mY, lParentTranslation.mZ));

				FbxAMatrix lBlendMatrix = lParentMatrix * lLocalMatrix;

				FbxVector4 lBlendScale = lBlendMatrix.GetS();
				FbxQuaternion lBlendOrientation = lBlendMatrix.GetQ();
				FbxVector4 lBlendTranslation = lBlendMatrix.GetT();

				Math::CTransform lBlendTransform;
				lBlendTransform.SetScale(Math::SVector3(lBlendScale[0], lBlendScale[1], lBlendScale[2]));
				lBlendTransform.SetOrientation(Math::SQuaternion(lBlendOrientation[0], lBlendOrientation[1], lBlendOrientation[2], lBlendOrientation[3]));
				lBlendTransform.SetTranslation(Math::SVector3(lBlendTranslation[0], lBlendTranslation[1], lBlendTranslation[2]));

				lTop->SetGlobalTransform(lBlendTransform);
			}
			else
			{
				lTop->SetGlobalTransform(lTop->GetLocalTransform());
			}

			
			vector<shared_ptr<CBone>>::iterator lBegin, lEnd;
			lTop->GetChildIterators(lBegin, lEnd);
			
			for (auto it = lBegin; it != lEnd; it++)
			{
				lStack.push(*it);
			}
		}

	}
}
