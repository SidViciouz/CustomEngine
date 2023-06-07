#include "Matrix4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Transform.h"

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

	SMatrix4 SMatrix4::operator*(const SMatrix4& lOtherMatrix)
	{
		DirectX::XMMATRIX lThis = DirectX::XMLoadFloat4x4(&mXmElement);

		DirectX::XMMATRIX lOther = DirectX::XMLoadFloat4x4(&lOtherMatrix.mXmElement);

		DirectX::XMMATRIX lResult = lThis * lOther;

		SMatrix4 lResultMatrix;

		DirectX::XMStoreFloat4x4(&lResultMatrix.mXmElement, lResult);

		return lResultMatrix;
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


	SMatrix4 SMatrix4::Translation(const SVector3& pTranslation)
	{
		SMatrix4 lMatrix;

		for (int i = 0; i < 3; ++i)
			lMatrix.mElement[3][i] = pTranslation.mElement[i];

		return lMatrix;
	}

	SMatrix4 SMatrix4::Orientation(const SQuaternion& pOrientation)
	{
		SMatrix4 lMatrix;

		float lI = pOrientation.mX;
		float lJ = pOrientation.mY;
		float lK = pOrientation.mZ;
		float lR = pOrientation.mW;

		float lI2 = lI * lI;
		float lJ2 = lJ * lJ;
		float lK2 = lK * lK;

		float lIJ = lI * lJ;
		float lJK = lJ * lK;
		float lIK = lI * lK;
		float lIR = lI * lR;
		float lJR = lJ * lR;
		float lKR = lK * lR;


		lMatrix.m11 = 1 - 2 * (lJ2 + lK2);
		lMatrix.m12 = 2 * (lIJ + lKR);
		lMatrix.m13 = 2 * (lIK + lJR);

		lMatrix.m21 = 2 * (lIJ - lKR);
		lMatrix.m22 = 1 - 2 * (lI2 + lK2);
		lMatrix.m23 = 2 * (lJK + lIR);

		lMatrix.m31 = 2 * (lIK + lJR);
		lMatrix.m32 = 2 * (lJK - lIR);
		lMatrix.m33 = 1 - 2 * (lI2 + lJ2);

		return lMatrix;
	}



	SMatrix4 SMatrix4::Scale(const SVector3& pScale)
	{
		SMatrix4 lMatrix;

		for (int i = 0; i < 3; ++i)
			lMatrix.mElement[i][i] = pScale.mElement[i];

		return lMatrix;
	}



	SMatrix4 SMatrix4::Transform(const CTransform& pTransform)
	{
		return Orientation(pTransform.GetOrientation()) * Scale(pTransform.GetScale()) * Translation(pTransform.GetTranslation());
	}

}