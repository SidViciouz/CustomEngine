#include "../Mesh/header/Object.h"
#include "Vector3.h"

namespace Renderer
{
	CObject::CObject()
	{

	}

	CObject::CObject(const Math::CTransform& pTransform)
	{
		mTransform = pTransform;
	}

	Math::SMatrix4 CObject::GetWorldMatrix() const
	{
		const Math::SVector3& lTranslation = mTransform.GetTranslation();
		const Math::SQuaternion& lOrientation = mTransform.GetOrientation();
		const Math::SVector3& lScale = mTransform.GetScale();

		Math::SMatrix4 lMatrix = Math::SMatrix4::Orientation(lOrientation) * Math::SMatrix4::Scale(lScale) * Math::SMatrix4::Translation(lTranslation);

		return lMatrix;
	}

	void CObject::SetTranslation(const Math::SVector3& pTranslation)
	{
		mTransform.SetTranslation(pTranslation);
	}

	void CObject::SetOrientation(const Math::SQuaternion& pOrientation)
	{
		mTransform.SetOrientation(pOrientation);
	}

	void CObject::SetScale(const Math::SVector3& pScale)
	{
		mTransform.SetScale(pScale);
	}

	const Math::SVector3& CObject::GetTranslation() const
	{
		return mTransform.GetTranslation();
	}

	const Math::SQuaternion& CObject::GetOrientation() const
	{
		return mTransform.GetOrientation();
	}

	const Math::SVector3& CObject::GetScale() const
	{
		return mTransform.GetScale();
	}

	Math::SVector3& CObject::GetTranslation()
	{
		return mTransform.GetTranslation();
	}

	Math::SQuaternion& CObject::GetOrientation()
	{
		return mTransform.GetOrientation();
	}

	Math::SVector3& CObject::GetScale()
	{
		return mTransform.GetScale();
	}

	void CObject::AddTranslation(Math::SVector3 pTranslation)
	{
		mTransform.GetTranslation() += pTranslation;
	}

	void CObject::SetBaseColorResourceHandle(int pResourceHandle)
	{
		mBaseColorResourceHandle = pResourceHandle;
	}

	void CObject::SetMetallicResourceHandle(int pResourceHandle)
	{
		mMetallicResourceHandle = pResourceHandle;
	}

	void CObject::SetNormalResourceHandle(int pResourceHandle)
	{
		mNormalResourceHandle = pResourceHandle;
	}

	void CObject::SetRoughnessResourceHandle(int pResourceHandle)
	{
		mRoughnessResourceHandle = pResourceHandle;
	}

	void CObject::SetObjectHandle(int pObjectHandle)
	{
		mObjectHandle = pObjectHandle;
	}

	int CObject::GetObjectHandle() const
	{
		return mObjectHandle;
	}
}