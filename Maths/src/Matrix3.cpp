#include "Matrix3.h"
#include "Vector3.h"

namespace Math
{
	SMatrix3::SMatrix3()
	{
		SetIdentity();
	}

	SMatrix3::SMatrix3(const SVector3& pRow1, const SVector3& pRow2, const SVector3& pRow3)
	{
		SetIdentity();

		SetRow(0, pRow1);
		SetRow(1, pRow2);
		SetRow(2, pRow3);
	}

	void SMatrix3::operator=(const DirectX::XMFLOAT3X3& pXmMatrix)
	{
		for (int lRow = 0; lRow < 3; ++lRow)
			for (int lColumn = 0; lColumn < 3; ++lColumn)
				mElement[lRow][lColumn] = pXmMatrix(lRow, lColumn);
	}

	SMatrix3 SMatrix3::operator*(const SMatrix3& lOtherMatrix)
	{
		DirectX::XMMATRIX lThis = DirectX::XMLoadFloat3x3(&mXmElement);

		DirectX::XMMATRIX lOther = DirectX::XMLoadFloat3x3(&lOtherMatrix.mXmElement);

		DirectX::XMMATRIX lResult = lThis * lOther;

		SMatrix3 lResultMatrix;

		DirectX::XMStoreFloat3x3(&lResultMatrix.mXmElement, lResult);

		return lResultMatrix;
	}

	void SMatrix3::SetIdentity()
	{
		for (int lRow = 0; lRow < 3; ++lRow)
			for (int lColumn = 0; lColumn < 3; ++lColumn)
			{
				if (lRow == lColumn)
					mElement[lRow][lColumn] = 1;
				else
					mElement[lRow][lColumn] = 0;
			}
	}

	void SMatrix3::SetRow(int pRowIndex, const SVector3& pRow)
	{
		for (int i = 0; i < 3; ++i)
			mElement[pRowIndex][i] = pRow.mElement[i];
	}

	void SMatrix3::SetColumn(int pColumnIndex, const SVector3& pColumn)
	{
		for (int i = 0; i < 3; ++i)
			mElement[i][pColumnIndex] = pColumn.mElement[i];
	}
}