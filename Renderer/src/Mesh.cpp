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

namespace Renderer
{
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

		FbxVector4* lControlPoints = lMesh->GetControlPoints();
		int lControlPointCount = lMesh->GetControlPointsCount();

		for (int lControlPointIndex = 0; lControlPointIndex < lControlPointCount; ++lControlPointIndex)
		{
			SVertex lVertex;
			for (int i = 0; i < 3; ++i)
				lVertex.mPosition.mElement[i] = lControlPoints[lControlPointIndex][i];

			mVertices.push_back(lVertex);
		}
	}

	int	CMesh::GetVertexCount() const
	{
		return mVertices.size();
	}

	const SVertex* CMesh::GetVertexDatas() const
	{
		return mVertices.data();
	}

}
