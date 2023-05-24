#pragma once

#include <DirectXMath.h>

namespace Math
{
	struct SVector3;

	/*
	* row major¿Ã¥Ÿ.
	*/
	struct SMatrix4
	{
												SMatrix4();
												SMatrix4(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3);
												SMatrix4(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3, const SVector3& pRow4);

		void									operator=(const DirectX::XMFLOAT4X4& pXmMatrix);
		void									SetIdentity();
		void									SetRow(int pRowIndex, const SVector3& pRow);
		void									SetColumn(int pColumnIndex, const SVector3& pColumn);

		union
		{
			struct
			{
				float m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44;
			};

			float mElement[4][4];

			DirectX::XMFLOAT4X4 mXmElement;
		};

	};
}
