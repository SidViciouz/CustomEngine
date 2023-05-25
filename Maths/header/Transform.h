#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Math
{
	class CTransform
	{
	public:
												CTransform();
	protected:
		SVector3								mTranslation;
		SQuaternion								mOrientation;
		SVector3								mScale;
	};
}
