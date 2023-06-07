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
												CBone(FbxNode* pLimbNode);

		void									AddChild(shared_ptr<CBone> pChild);
		void									SetParent(shared_ptr<CBone> pParent);

		const char*								GetName() const;

		shared_ptr<SCluster>					GetCluster();
		void									SetCluster(shared_ptr<SCluster> pCluster);

	protected:

		const char*								mName;

		shared_ptr<Math::CTransform>			mLocalTransform;
		shared_ptr<Math::CTransform>			mGlobalTransform;

		shared_ptr<CBone>						mParent;
		vector<shared_ptr<CBone>>				mChilds;

		shared_ptr<SCluster>					mCluster;
	};



	using Bones = unordered_map<string, shared_ptr<CBone>>;



	class CSkeleton
	{	
	public:
												CSkeleton(FbxSkeleton* pFbxSkeleton,FbxSkin* pSkinDeformer,int pControlPointCount);

	protected:
		shared_ptr<CBone>						mRootBone;
		Bones									mBones;

		bool									mHasDeformer = false;

		//get bones that affect the controlPoint
		vector<vector<string>>					mControlPointToBones;
	};
}