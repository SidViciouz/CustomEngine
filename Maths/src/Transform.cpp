#include "Transform.h"

namespace Math
{
	CTransform::CTransform()
	{
		mTranslation = { 0,0,0 };
		mOrientation = { 0,0,0,1 };
		mScale = { 1,1,1 };
	}
}