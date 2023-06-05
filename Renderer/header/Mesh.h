#pragma once

#include "Common.h"
#include "../../Maths/header/Vector3.h"

namespace Renderer
{
	struct SVertex
	{
		Math::SVector3 mPosition;
		struct STextureCoord
		{
			union
			{
				struct
				{
					float mX;
					float mY;
				};

				float mElement[2];

				DirectX::XMFLOAT2 mXmElement;
			};

		} mTextureCoord;
		Math::SVector3 mNormal;
		Math::SVector3 mTangent;
		Math::SVector3 mBinormal;
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

		void									SetVertexBufferHandle(int pResourceHandle);
		int										GetVertexBufferHandle() const;

		void									SetIndexBufferHandle(int pSubMeshIndex, int pResourceHandle);
		int										GetIndexBufferHandle(int pSubMeshIndex) const;

		// mesh, bone weight, ...
	protected:
		void									Load(const char* pPath);

		vector<SVertex>							mVertices;
		vector<SSubMesh>						mSubMeshes;

		int										mVertexBufferHandle;
	};

}