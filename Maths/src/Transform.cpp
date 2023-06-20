#include "Transform.h"

namespace Math
{
	CTransform::CTransform()
	{
		mTranslation = { 0,0,0 };
		mOrientation = { 0,0,0,1 };
		mScale = { 1,1,1 };
	}

	CTransform::CTransform(const SVector3& pTranslation, const SQuaternion& pOrientation, const SVector3& pScale)
		: mTranslation{pTranslation}, mOrientation{pOrientation}, mScale{pScale}
	{
		
	}

	const SVector3& CTransform::GetTranslation() const
	{
		return mTranslation;
	}

	SVector3& CTransform::GetTranslation()
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

	SQuaternion& CTransform::GetOrientation()
	{
		return mOrientation;
	}

	void CTransform::SetOrientation(const SQuaternion& pOrientation)
	{
		mOrientation = pOrientation;
	}

	const SVector3& CTransform::GetScale() const
	{
		return mScale;
	}

	SVector3& CTransform::GetScale()
	{
		return mScale;
	}

	void CTransform::SetScale(const SVector3& pScale)
	{
		mScale = pScale;
	}

	CTransform CTransform::Blend(const CTransform& pA, const CTransform& pB, double pAlpha)
	{
		const Math::SVector3 lTranslation = pA.GetTranslation() * (1 - pAlpha) + pB.GetTranslation() * pAlpha;
		const Math::SQuaternion lOrientation = pA.GetOrientation().Slerp(pB.GetOrientation(), pAlpha);
		const Math::SVector3 lScale = pA.GetScale() * (1 - pAlpha) + pB.GetScale() * pAlpha;

		return CTransform{ lTranslation ,lOrientation ,lScale };
	}
}