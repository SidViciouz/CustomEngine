#pragma once

#include <DirectXMath.h>

namespace Math
{
	struct SQuaternion
	{
												SQuaternion();
												SQuaternion(const float& pX, const float& pY, const float& pZ, const float& pW);
		
		static float							Dot(const SQuaternion& pA, const SQuaternion& pB);

		SQuaternion								operator-() const;

		template<typename T>
		SQuaternion								operator*(const T& pValue) const;

		SQuaternion								operator+(const SQuaternion& pOther);

		void									Normalized();

		SQuaternion								Slerp(SQuaternion pOther,const double& pAlpha) const;

		union
		{
			struct
			{
				float mX, mY, mZ, mW;
			};

			float mElement[4];

			DirectX::XMFLOAT4 mXmElement;
		};

	};
}
