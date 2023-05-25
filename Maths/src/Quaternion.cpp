#include "Quaternion.h"

namespace Math
{
	SQuaternion::SQuaternion()
	{
		mX = 0;
		mY = 0;
		mZ = 0;
		mW = 1;
	}

	SQuaternion::SQuaternion(const float& pX, const float& pY, const float& pZ, const float& pW)
	{
		mX = pX;
		mY = pY;
		mZ = pZ;
		mW = pW;
	}
}