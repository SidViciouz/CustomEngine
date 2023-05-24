#include "Vector3.h"

namespace Math
{
	SVector3::SVector3()
	{
		Set(0, 0, 0);
	}

	SVector3::SVector3(const float& pX, const float& pY, const float& pZ)
	{
		Set(pX, pY, pZ);
	}

	void SVector3::Set(const float& pX, const float& pY, const float& pZ)
	{
		mX = pX;
		mY = pY;
		mZ = pZ;
	}

	SVector3 SVector3::operator-()
	{
		return SVector3(-mX, -mY, -mZ);
	}
}