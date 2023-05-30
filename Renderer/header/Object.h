#pragma once
#include "../../Maths/header/Transform.h"
#include "../../Maths/header/Matrix4.h"

namespace Math
{
	struct SVector3;
}

namespace Renderer
{

	class CObject
	{
	public:
												CObject();
												CObject(const Math::CTransform& pTransform);

		Math::SMatrix4							GetWorldMatrix() const;

		void									SetTranslation(const Math::SVector3& pTranslation);
		void									SetOrientation(const Math::SQuaternion& pOrientation);
		void									SetScale(const Math::SVector3& pScale);

	protected:
		//transform, bone, ...
		Math::CTransform						mTransform;
	};
}
