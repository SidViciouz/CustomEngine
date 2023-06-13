#pragma once

#include <DirectXMath.h>

namespace Math
{
	struct SVector3;

	/*
	* row major¿Ã¥Ÿ.
	*/
	struct SMatrix3
	{
		SMatrix3();
		SMatrix3(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3);

		void									operator=(const DirectX::XMFLOAT3X3& pXmMatrix);
		SMatrix3								operator*(const SMatrix3& lOtherMatrix);

		void									SetIdentity();
		void									SetRow(int pRowIndex, const SVector3& pRow);
		void									SetColumn(int pColumnIndex, const SVector3& pColumn);

		union
		{
			struct
			{
				float m11, m12, m13,
					m21, m22, m23,
					m31, m32, m33;
			};

			float mElement[3][3];

			DirectX::XMFLOAT3X3 mXmElement;
		};

	};
}
