#pragma once
#include "../../Maths/header/Transform.h"
#include "../../Maths/header/Matrix4.h"

namespace Renderer
{
	class CObject
	{
	public:
												CObject();
												CObject(const Math::CTransform& pTransform);

		Math::SMatrix4							GetWorldMatrix() const;

	protected:
		//transform, bone, ...
		Math::CTransform						mTransform;
	};
}
