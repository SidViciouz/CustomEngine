#pragma once
#include "../../Maths/header/Transform.h"

namespace Renderer
{
	class CObject
	{
	public:
												CObject();
	protected:
		//transform, bone, ...
		Math::CTransform						mTransform;
	};
}
