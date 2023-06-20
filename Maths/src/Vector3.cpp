#include "Vector3.h"
#include "Matrix3.h"

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

	SVector3 SVector3::operator*(const double& pValue) const
	{
		return SVector3(mX * pValue, mY * pValue, mZ * pValue);
	}

	SVector3 SVector3::operator+(const SVector3& pOther)
	{
		return SVector3(mX + pOther.mX, mY + pOther.mY, mZ + pOther.mZ);
	}

	SVector3 SVector3::operator-(const SVector3& pOther)
	{
		return SVector3(mX - pOther.mX, mY - pOther.mY, mZ - pOther.mZ);
	}

	void SVector3::operator+=(const SVector3& pOther)
	{
		mX += pOther.mX;
		mY += pOther.mY;
		mZ += pOther.mZ;
	}


	SVector3 SVector3::Normalize() const
	{
		SVector3 lVector(mX,mY,mZ);

		float lDenom = sqrtf(mX* mX + mY * mY + mZ * mZ);

		for (int i = 0; i < 3; ++i)
		{
			lVector.mElement[i] /= lDenom;
		}

		return lVector;
	}

	SVector3 SVector3::Cross(const SVector3& pA, const SVector3& pB)
	{
		DirectX::XMVECTOR lA = DirectX::XMLoadFloat3(&pA.mXmElement);
		DirectX::XMVECTOR lB = DirectX::XMLoadFloat3(&pB.mXmElement);

		DirectX::XMVECTOR lC = DirectX::XMVector3Cross(lA, lB);

		SVector3 lResult;

		DirectX::XMStoreFloat3(&lResult.mXmElement, lC);

		return lResult;
	}

	float SVector3::Dot(const SVector3& pA, const SVector3& pB)
	{
		DirectX::XMVECTOR lA = DirectX::XMLoadFloat3(&pA.mXmElement);
		DirectX::XMVECTOR lB = DirectX::XMLoadFloat3(&pB.mXmElement);

		DirectX::XMVECTOR lC = DirectX::XMVector3Dot(lA, lB);

		return DirectX::XMVectorGetX(lC);
	}

	SVector3 SVector3::Transform(const SMatrix3& pTransformMatrix) const
	{
		SVector3 lResult;

		DirectX::XMVECTOR lV = DirectX::XMLoadFloat3(&mXmElement);
		DirectX::XMMATRIX lM = DirectX::XMLoadFloat3x3(&pTransformMatrix.mXmElement);

		lV = DirectX::XMVector3Transform(lV, lM);

		DirectX::XMStoreFloat3(&lResult.mXmElement, lV);

		return lResult;
	}

	float SVector3::Length() const
	{
		return sqrtf(mX * mX + mY * mY + mZ * mZ);
	}


}