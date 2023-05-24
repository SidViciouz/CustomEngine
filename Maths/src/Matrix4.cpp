#include "Matrix4.h"
#include "Vector3.h"

namespace Math
{
	SMatrix4::SMatrix4()
	{
		SetIdentity();
	}

	SMatrix4::SMatrix4(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3)
	{
		SetIdentity();

		SetRow(0, pRow1);
		SetRow(1, pRow2);
		SetRow(2, pRow3);
	}

	SMatrix4::SMatrix4(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3, const SVector3& pRow4)
	{
		SetIdentity();

		SetRow(0, pRow1);
		SetRow(1, pRow2);
		SetRow(2, pRow3);
		SetRow(3, pRow4);
	}

	void SMatrix4::operator=(const DirectX::XMFLOAT4X4& pXmMatrix)
	{
		for (int lRow = 0; lRow < 4; ++lRow)
			for (int lColumn = 0; lColumn < 4; ++lColumn)
				mElement[lRow][lColumn] = pXmMatrix(lRow, lColumn);
	}

	void SMatrix4::SetIdentity()
	{
		for (int lRow = 0; lRow < 4; ++lRow)
			for (int lColumn = 0; lColumn < 4; ++lColumn)
			{
				if (lRow == lColumn)
					mElement[lRow][lColumn] = 1;
				else
					mElement[lRow][lColumn] = 0;
			}
	}

	void SMatrix4::SetRow(int pRowIndex, const SVector3& pRow)
	{
		for (int i = 0; i < 3; ++i)
			mElement[pRowIndex][i] = pRow.mElement[i];
	}

	void SMatrix4::SetColumn(int pColumnIndex, const SVector3& pColumn)
	{
		for (int i = 0; i < 3; ++i)
			mElement[i][pColumnIndex] = pColumn.mElement[i];
	}

}