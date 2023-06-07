#pragma once

#include "Common.h"
#include <fbxsdk.h>
#include "Transform.h"

namespace Renderer
{
	struct SCluster
	{
		int										mCount;

		// index of the control point (not vertex)
		vector<uint16_t>						mIndices;

		vector<double>							mWeights;
	};


	class CBone
	{
	public:
												CBone(FbxNode* pLimbNode,int pBoneIndex);

		void									AddChild(shared_ptr<CBone> pChild);
		void									SetParent(shared_ptr<CBone> pParent);

		int										GetIndex() const;
		const char*								GetName() const;

		shared_ptr<SCluster>					GetCluster();
		void									SetCluster(shared_ptr<SCluster> pCluster);

		Math::CTransform						GetGlobalTransform() const;
		Math::CTransform						GetLocalTransform() const;

	protected:

		const char*								mName;
		int										mBoneIndex;

		Math::CTransform						mLocalTransform;
		Math::CTransform						mGlobalTransform;

		shared_ptr<CBone>						mParent;
		vector<shared_ptr<CBone>>				mChilds;

		shared_ptr<SCluster>					mCluster;
	};



	using Bones = unordered_map<string, shared_ptr<CBone>>;



	class CSkeleton
	{	
	public:
												CSkeleton(FbxSkeleton* pFbxSkeleton,FbxSkin* pSkinDeformer,int pControlPointCount);

		void									GetBoneTransformMatrix(vector<Math::SMatrix4>& pMatrices) const;
		int										GetBoneCount() const;

	protected:

		int										mBoneCount = 0;
		shared_ptr<CBone>						mRootBone;
		Bones									mBones;

		bool									mHasDeformer = false;

		//get bones that affect the controlPoint
		vector<vector<string>>					mControlPointToBones;
	};
}
