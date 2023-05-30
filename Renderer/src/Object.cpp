#include "Object.h"
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
}