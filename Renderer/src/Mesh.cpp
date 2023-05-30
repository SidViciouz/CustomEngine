#include "Mesh.h"
#include <fbxsdk.h>

void Print(FbxNode* pNode, int pTabNum)
{
	for (int i = 0; i < pTabNum; ++i)
		printf("\t");
	printf("%s[%s]\n", pNode->GetName(),pNode->GetTypeName());

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		Print(pNode->GetChild(i), pTabNum + 1);
	}
}

void StoreVertex(FbxMesh* pFbxMesh, vector<Renderer::SVertex>& pVertices)
{
	FbxVector4* lControlPoints = pFbxMesh->GetControlPoints();
	int lControlPointCount = pFbxMesh->GetControlPointsCount();

	for (int lControlPointIndex = 0; lControlPointIndex < lControlPointCount; ++lControlPointIndex)
	{
		Renderer::SVertex lVertex;
		for (int i = 0; i < 3; ++i)
			lVertex.mPosition.mElement[i] = lControlPoints[lControlPointIndex][i];

		pVertices.push_back(lVertex);
	}
}

void StoreIndex(FbxMesh* pFbxMesh, vector<Renderer::SSubMesh>& pSubMeshes,int pSubMeshCount)
{
	int lTriangle[6] = { 0,1,2,0,2,3 };

	pSubMeshes.resize(pSubMeshCount);

	if (pFbxMesh->GetElementMaterialCount() == 0)
	{
		//when there is no material


	}
	else
	{
		FbxLayerElementArrayTemplate<int>& lMaterialIndexArray = pFbxMesh->GetElementMaterial()->GetIndexArray();

		int lPolygonCount = pFbxMesh->GetPolygonCount();
		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			int lMaterialIndex = lMaterialIndexArray.GetAt(lPolygonIndex);

			int lPolygonSize = pFbxMesh->GetPolygonSize(lPolygonIndex);

			if (lPolygonSize == 3)
			{
				for (int lPolygonPosition = 0; lPolygonPosition < 3; ++lPolygonPosition)
				{
					int lIndex = pFbxMesh->GetPolygonVertex(lPolygonIndex, lPolygonPosition);
					pSubMeshes[lMaterialIndex].AddIndex(static_cast<uint16_t>(lIndex));
				}

				//pSubMeshes[lMaterialIndex]->AddTriangle();
			}
			else if (lPolygonSize == 4)
			{
				for (int i = 0; i < 6; ++i)
				{
					int lIndex = pFbxMesh->GetPolygonVertex(lPolygonIndex, lTriangle[i]);
					pSubMeshes[lMaterialIndex].AddIndex(static_cast<uint16_t>(lIndex));

				}

				//pSubMeshes[lMaterialIndex]->AddTriangle(2);
			}
			else
			{
				printf("lPolygonSize is %d\n", lPolygonSize);
			}
		}
	}
}

namespace Renderer
{
	void SSubMesh::AddIndex(uint16_t pIndex)
	{
		mIndices.push_back(pIndex);
	}

	void SSubMesh::SetIndexBufferHandle(int pResourceHandle)
	{
		mIndexBufferHandle = pResourceHandle;
	}

	int SSubMesh::GetIndexBufferHandle() const
	{
		return mIndexBufferHandle;
	}

	CMesh::CMesh(const char* pPath)
	{
		Load(pPath);
	}

	void CMesh::Load(const char* pPath)
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

		//Print(lFbxScene->GetRootNode(),0);

		FbxMesh* lMesh = FbxCast<FbxMesh>(lFbxScene->GetSrcObject(FbxCriteria::ObjectType(FbxMesh::ClassId)));

		//store vertex data
		StoreVertex(lMesh, mVertices);

		//store index data
		StoreIndex(lMesh, mSubMeshes, lFbxScene->GetMaterialCount());
	}

	int	CMesh::GetVertexCount() const
	{
		return mVertices.size();
	}

	const SVertex* CMesh::GetVertexDatas() const
	{
		return mVertices.data();
	}

	int	CMesh::GetSubMeshCount() const
	{
		return mSubMeshes.size();
	}

	int	CMesh::GetIndexCount(int pSubMeshIndex) const
	{
		return mSubMeshes[pSubMeshIndex].mIndices.size();
	}

	const uint16_t* CMesh::GetIndexDatas(int pSubMeshIndex) const
	{
		return mSubMeshes[pSubMeshIndex].mIndices.data();
	}

	void CMesh::SetVertexBufferHandle(int pResourceHandle)
	{
		mVertexBufferHandle = pResourceHandle;
	}

	int	CMesh::GetVertexBufferHandle() const
	{
		return mVertexBufferHandle;
	}

	void CMesh::SetIndexBufferHandle(int pSubMeshIndex, int pResourceHandle)
	{
		mSubMeshes[pSubMeshIndex].SetIndexBufferHandle(pResourceHandle);
	}

	int	CMesh::GetIndexBufferHandle(int pSubMeshIndex) const
	{
		return mSubMeshes[pSubMeshIndex].GetIndexBufferHandle();
	}

}
