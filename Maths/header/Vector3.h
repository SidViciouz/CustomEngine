#pragma once
#include <DirectXMath.h>

namespace Math
{
	class SMatrix3;

	struct SVector3
	{
												SVector3();
												SVector3(const float& pX, const float& pY, const float& pZ);

		void									Set(const float& pX, const float& pY, const float& pZ);

		SVector3								operator-();

		SVector3								operator*(const double& pValue) const;

		SVector3								operator+(const SVector3& pOther);

		SVector3								operator-(const SVector3& pOther);

		SVector3								Normalize() const;

		static SVector3							Cross(const SVector3& pA, const SVector3& pB);

		static float							Dot(const SVector3& pA, const SVector3& pB);

		SVector3								Transform(const SMatrix3& pTransformMatrix) const;

		float									Length() const;

	union
	{
		struct
		{
			float mX, mY, mZ;
		};

		float mElement[3];

		DirectX::XMFLOAT3 mXmElement;
	};
	};
}
