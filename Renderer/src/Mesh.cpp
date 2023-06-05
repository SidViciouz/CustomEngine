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

void StoreVertex(FbxMesh* pFbxMesh, vector<Renderer::SVertex>& pVertices, vector<Renderer::SSubMesh>& pSubMeshes, int pSubMeshCount)
{
	int	lTriangle[6] = { 0,1,2,0,2,3 };

	pSubMeshes.resize(pSubMeshCount);

	FbxVector4* lControlPoints = pFbxMesh->GetControlPoints();
	int lControlPointCount = pFbxMesh->GetControlPointsCount();

	bool lIsByPolygonVertex = false;

	FbxGeometryElementUV* lUvElement = nullptr;
	FbxGeometryElementNormal* lNormalElement = nullptr;
	FbxGeometryElementTangent* lTangentElement = nullptr;
	FbxGeometryElementBinormal* lBinormalElement = nullptr;

	FbxLayerElementArrayTemplate<int>& lMaterialIndexArray = pFbxMesh->GetElementMaterial()->GetIndexArray();


	if (pFbxMesh->GetElementUVCount() != 0)
	{
		lUvElement = pFbxMesh->GetElementUV();
		if (lUvElement->GetMappingMode() == FbxLayerElement::EMappingMode::eByPolygonVertex)
			lIsByPolygonVertex = true;
	}

	if (pFbxMesh->GetElementNormalCount() != 0)
	{
		lNormalElement = pFbxMesh->GetElementNormal();
		if (lNormalElement->GetMappingMode() == FbxLayerElement::EMappingMode::eByPolygonVertex)
			lIsByPolygonVertex = true;
	}

	if (pFbxMesh->GetElementTangentCount() != 0)
	{
		lTangentElement = pFbxMesh->GetElementTangent();
		if (lTangentElement->GetMappingMode() == FbxLayerElement::EMappingMode::eByPolygonVertex)
			lIsByPolygonVertex = true;
	}

	if (pFbxMesh->GetElementBinormalCount() != 0)
	{
		lBinormalElement = pFbxMesh->GetElementBinormal();
		if (lBinormalElement->GetMappingMode() == FbxLayerElement::EMappingMode::eByPolygonVertex)
			lIsByPolygonVertex = true;
	}

	if (lIsByPolygonVertex)
	{
		
		printf("%d %d %d %d %d\n",
			lControlPointCount,
			lUvElement->GetDirectArray().GetCount(),
			lNormalElement->GetDirectArray().GetCount(),
			lTangentElement->GetDirectArray().GetCount(),
			lBinormalElement->GetDirectArray().GetCount());
		
		printf("%d\n", pFbxMesh->GetPolygonVertexCount());

		int lVertexIndex = 0;

		int lIndexCount = pFbxMesh->GetPolygonVertexCount();

		int lPolygonCount = pFbxMesh->GetPolygonCount();

		for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
		{
			int lMaterialIndex = lMaterialIndexArray.GetAt(lPolygonIndex);

			int lPolygonSize = pFbxMesh->GetPolygonSize(lPolygonIndex);
			if (lPolygonSize == 3)
			{
				for (int lPolygonPosition = 0; lPolygonPosition < 3; ++lPolygonPosition)
				{
					Renderer::SVertex lVertex;

					int lControlPointIndex = pFbxMesh->GetPolygonVertex(lPolygonIndex, lPolygonPosition);

					int lUvIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lUvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lUvIndex = lUvElement->GetIndexArray().GetAt(lUvIndex);
					FbxVector2 lUv = lUvElement->GetDirectArray().GetAt(lUvIndex);

					int lNormalIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lNormalIndex);
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

					int lTangentIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lTangentElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lTangentIndex = lTangentElement->GetIndexArray().GetAt(lTangentIndex);
					FbxVector4 lTangent = lTangentElement->GetDirectArray().GetAt(lTangentIndex);

					int lBinormalIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lBinormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lBinormalIndex = lBinormalElement->GetIndexArray().GetAt(lBinormalIndex);
					FbxVector4 lBinormal = lBinormalElement->GetDirectArray().GetAt(lBinormalIndex);


					for (int i = 0; i < 3; ++i)
						lVertex.mPosition.mElement[i] = lControlPoints[lControlPointIndex][i];

					for (int i = 0; i < 2; ++i)
						lVertex.mTextureCoord.mElement[i] = lUv[i];
					//lVertex.mTextureCoord.mElement[0] = lUv[0];
					//lVertex.mTextureCoord.mElement[1] = 1.0f - lUv[1];
					for (int i = 0; i < 3; ++i)
						lVertex.mNormal.mElement[i] = lNormal[i];

					for (int i = 0; i < 3; ++i)
						lVertex.mTangent.mElement[i] = lTangent[i];

					for (int i = 0; i < 3; ++i)
						lVertex.mBinormal.mElement[i] = lBinormal[i];
						
					pVertices.push_back(lVertex);

					pSubMeshes[lMaterialIndex].AddIndex(static_cast<uint16_t>(lVertexIndex++));

				}
			}
			else // lPolygonSize == 4
			{
				for (int i = 0; i < 6; ++i)
				{
					Renderer::SVertex lVertex;

					int lPolygonPosition = lTriangle[i];

					int lControlPointIndex = pFbxMesh->GetPolygonVertex(lPolygonIndex, lPolygonPosition);

					int lUvIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lUvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lUvIndex = lUvElement->GetIndexArray().GetAt(lUvIndex);
					FbxVector2 lUv = lUvElement->GetDirectArray().GetAt(lUvIndex);

					int lNormalIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lNormalIndex);
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

					int lTangentIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lTangentElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lTangentIndex = lTangentElement->GetIndexArray().GetAt(lTangentIndex);
					FbxVector4 lTangent = lTangentElement->GetDirectArray().GetAt(lTangentIndex);

					int lBinormalIndex = pFbxMesh->GetPolygonVertexIndex(lPolygonIndex) + lPolygonPosition;
					if (lBinormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						lBinormalIndex = lBinormalElement->GetIndexArray().GetAt(lBinormalIndex);
					FbxVector4 lBinormal = lBinormalElement->GetDirectArray().GetAt(lBinormalIndex);


					for (int i = 0; i < 3; ++i)
						lVertex.mPosition.mElement[i] = lControlPoints[lControlPointIndex][i];

					for (int i = 0; i < 2; ++i)
						lVertex.mTextureCoord.mElement[i] = lUv[i];

					for (int i = 0; i < 3; ++i)
						lVertex.mNormal.mElement[i] = lNormal[i];

					for (int i = 0; i < 3; ++i)
						lVertex.mTangent.mElement[i] = lTangent[i];

					for (int i = 0; i < 3; ++i)
						lVertex.mBinormal.mElement[i] = lBinormal[i];
						
					pVertices.push_back(lVertex);

					pSubMeshes[lMaterialIndex].AddIndex(static_cast<uint16_t>(lVertexIndex++));
				}
			}
		}
	}
	else
	{
		for (int lControlPointIndex = 0; lControlPointIndex < lControlPointCount; ++lControlPointIndex)
		{
			Renderer::SVertex lVertex;
			for (int i = 0; i < 3; ++i)
				lVertex.mPosition.mElement[i] = lControlPoints[lControlPointIndex][i];

			for (int i = 0; i < 2; ++i)
				lVertex.mTextureCoord.mElement[i] = 0;

			for (int i = 0; i < 3; ++i)
				lVertex.mNormal.mElement[i] = 0;

			for (int i = 0; i < 3; ++i)
				lVertex.mTangent.mElement[i] = 0;

			for (int i = 0; i < 3; ++i)
				lVertex.mBinormal.mElement[i] = 0;

			pVertices.push_back(lVertex);
		}

	}

	//printf("element tangent count : %d, mapping mode : %d, reference mode :%d\n", pFbxMesh->GetElementTangentCount() , pFbxMesh->GetElementTangent()->GetMappingMode(), pFbxMesh->GetElementTangent()->GetReferenceMode());
	//printf("element uv count : %d, mapping mode : %d, reference mode : %d\n", pFbxMesh->GetElementUVCount() , pFbxMesh->GetElementUV()->GetMappingMode(), pFbxMesh->GetElementUV()->GetReferenceMode());
	//printf("element binormal count : %d, mapping mode : %d, reference mode %d\n", pFbxMesh->GetElementBinormalCount(),pFbxMesh->GetElementBinormal()->GetMappingMode(),pFbxMesh->GetElementBinormal()->GetReferenceMode());
	
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
		StoreVertex(lMesh, mVertices, mSubMeshes, lFbxScene->GetMaterialCount());

		//store index data
		//StoreIndex(lMesh, mSubMeshes, lFbxScene->GetMaterialCount());
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
