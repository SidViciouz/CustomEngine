#pragma once

#include "Common.h"
#include "../../Maths/header/Vector3.h"

namespace Renderer
{
	struct SVertex
	{
		Math::SVector3 mPosition;
	};

	class CMesh
	{
	public:
												CMesh(const char* pPath);

		int										GetVertexCount() const;
		const SVertex*							GetVertexDatas() const;

		// mesh, bone weight, ...
	protected:
		void									Load(const char* pPath);

		vector<SVertex>							mVertices;
	};

}