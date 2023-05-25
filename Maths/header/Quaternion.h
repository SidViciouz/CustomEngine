#pragma once

#include <DirectXMath.h>

namespace Math
{
	struct SQuaternion
	{
												SQuaternion();
												SQuaternion(const float& pX, const float& pY, const float& pZ, const float& pW);
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
