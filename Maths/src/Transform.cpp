#include "Transform.h"

namespace Math
{
	CTransform::CTransform()
	{
		mTranslation = { 0,0,0 };
		mOrientation = { 0,0,0,1 };
		mScale = { 1,1,1 };
	}

	const SVector3& CTransform::GetTranslation() const
	{
		return mTranslation;
	}

	void CTransform::SetTranslation(const SVector3& pTranslation)
	{
		mTranslation = pTranslation;
	}

	const SQuaternion& CTransform::GetOrientation() const
	{
		return mOrientation;
	}

	const SVector3& CTransform::GetScale() const
	{
		return mScale;
	}
}