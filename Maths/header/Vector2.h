#pragma once
#include <DirectXMath.h>

namespace Math
{
	struct SVector2
	{
												SVector2();
												SVector2(const float& pX, const float& pY);

		void									Set(const float& pX, const float& pY);

		SVector2								operator-();

		SVector2								operator*(const double& pValue) const;

		SVector2								operator+(const SVector2& pOther);

		union
		{
			struct
			{
				float mX, mY;
			};

			float mElement[2];

			DirectX::XMFLOAT2 mXmElement;
		};
	};
}
