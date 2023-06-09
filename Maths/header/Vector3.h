#pragma once
#include <DirectXMath.h>

namespace Math
{
	struct SVector3
	{
												SVector3();
												SVector3(const float& pX, const float& pY, const float& pZ);

		void									Set(const float& pX, const float& pY, const float& pZ);

		SVector3								operator-();

		SVector3								operator*(const double& pValue) const;

		SVector3								operator+(const SVector3& pOther);

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
