#include "Vector2.h"

namespace Math
{
	SVector2::SVector2()
	{
		Set(0, 0);
	}

	SVector2::SVector2(const float& pX, const float& pY)
	{
		Set(pX, pY);
	}

	void SVector2::Set(const float& pX, const float& pY)
	{
		mX = pX;
		mY = pY;
	}

	SVector2 SVector2::operator-()
	{
		return SVector2(-mX, -mY);
	}

	SVector2 SVector2::operator*(const double& pValue) const
	{
		return SVector2(mX * pValue, mY * pValue);
	}

	SVector2 SVector2::operator+(const SVector2& pOther)
	{
		return SVector2(mX + pOther.mX, mY + pOther.mY);
	}
}