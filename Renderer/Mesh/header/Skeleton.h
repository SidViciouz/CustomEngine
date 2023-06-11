#pragma once

#include "../Common/header/Common.h"
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
		void									GetChildIterators(vector<shared_ptr<CBone>>::iterator& lBegin, vector<shared_ptr<CBone>>::iterator& lEnd);

		shared_ptr<CBone>						GetParent() const;
		void									SetParent(shared_ptr<CBone> pParent);

		int										GetIndex() const;
		const char*								GetName() const;

		shared_ptr<SCluster>					GetCluster();
		void									SetCluster(shared_ptr<SCluster> pCluster);

		Math::CTransform						GetGlobalTransform() const;
		void									SetGlobalTransform(const Math::CTransform& pTransform);

		Math::CTransform						GetLocalTransform() const;
		void									SetLocalTransform(const Math::CTransform& pTransform);

		void									SetInverseInitialTransformMatrix(const Math::SMatrix4& pMatrix);
		Math::SMatrix4							GetInverseInitialTransformMatrix() const;

		Math::CTransform						EvaluateGlobalTransform(double pTime);
		Math::CTransform						EvaluateLocalTransform(double pTime);


	protected:

		const char*								mName;
		int										mBoneIndex;

		Math::CTransform						mLocalTransform;
		Math::CTransform						mGlobalTransform;
		Math::SMatrix4							mInverseInitialTransformMatrix;

		shared_ptr<CBone>						mParent;
		vector<shared_ptr<CBone>>				mChilds;

		shared_ptr<SCluster>					mCluster;

		FbxNode*								mLimbNode;
	};



	using Bones = unordered_map<string, shared_ptr<CBone>>;



	class CSkeleton
	{	
	public:
												CSkeleton(FbxSkeleton* pFbxSkeleton,FbxSkin* pSkinDeformer,int pControlPointCount);

		void									GetBoneTransformMatrix(vector<Math::SMatrix4>& pMatrices) const;
		int										GetBoneCount() const;

		const vector<pair<int,double>>&			GetControlPointToBones(int pControlPointIndex);

		void									GetBonesIterator(Bones::iterator& pBegin, Bones::iterator& pEnd);
		shared_ptr<CBone>						GetBone(const string& pName);

		void									UpdateGlobalFromLocal();

	protected:

		int										mBoneCount = 0;
		shared_ptr<CBone>						mRootBone;
		Bones									mBones;

		bool									mHasDeformer = false;

		//get bones that affect the controlPoint
		vector<vector<pair<int,double>>>		mControlPointToBones;
	};
}
