#pragma once

#include "../Common/header/Common.h"
#include "../../Maths/header/Vector3.h"
#include "../../Maths/header/Vector2.h"

namespace Renderer
{
	class CSkeleton;

	struct SVertex
	{
		Math::SVector3 mPosition;
		Math::SVector2 mTextureCoord;
		Math::SVector3 mNormal;
		Math::SVector3 mTangent;
		Math::SVector3 mBinormal;
		struct SBoneIndices
		{
			unsigned int mElement[4];
		} mBoneIndices;
		Math::SVector3 mBoneWeights;
	};

	struct SSubMesh
	{
		void									AddIndex(uint16_t pIndex);
		void									SetIndexBufferHandle(int pResourceHandle);
		int										GetIndexBufferHandle() const;

		vector<uint16_t>						mIndices;
		int										mIndexBufferHandle;
	};

	class CMesh
	{
	public:
												CMesh(const char* pPath);

		int										GetVertexCount() const;
		const SVertex*							GetVertexDatas() const;

		int										GetSubMeshCount() const;

		int										GetIndexCount(int pSubMeshIndex) const;
		const uint16_t*							GetIndexDatas(int pSubMeshIndex) const;

		bool									HasSkeleton() const;
		int										GetBoneCount() const;
		void									GetBoneTransformMatrices(vector<Math::SMatrix4>& pMatrices) const;

		void									SetVertexBufferHandle(int pResourceHandle);
		int										GetVertexBufferHandle() const;

		void									SetIndexBufferHandle(int pSubMeshIndex, int pResourceHandle);
		int										GetIndexBufferHandle(int pSubMeshIndex) const;

		shared_ptr<CSkeleton>					GetSkeleton() const;

		void									SetMeshHandle(int pMeshHandle);
		int										GetMeshHandle() const;

		// mesh, bone weight, ...
	protected:
		void									Load(const char* pPath);

		vector<SVertex>							mVertices;
		vector<SSubMesh>						mSubMeshes;

		// index of the vertex corresponding to controlPoint
		vector<vector<uint16_t>>				mControlPointToVertex;
		vector<uint16_t>						mVertexToControlPoint;

		int										mVertexBufferHandle;

		bool									mHasSkeleton = false;
		shared_ptr<CSkeleton>					mSkeleton;

		//handle used by renderer
		int										mMeshHandle = 0;
	};

}