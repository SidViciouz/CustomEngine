#pragma once
#include "../../Maths/header/Transform.h"
#include "../../Maths/header/Matrix4.h"

namespace Math
{
	struct SVector3;
}

namespace Renderer
{

	class CObject
	{
	public:
												CObject();
												CObject(const Math::CTransform& pTransform);

		Math::SMatrix4							GetWorldMatrix() const;

		void									SetTranslation(const Math::SVector3& pTranslation);
		void									SetOrientation(const Math::SQuaternion& pOrientation);
		void									SetScale(const Math::SVector3& pScale);

		const Math::SVector3&					GetTranslation() const;
		const Math::SQuaternion&				GetOrientation() const;
		const Math::SVector3&					GetScale() const;

		Math::SVector3&							GetTranslation();
		Math::SQuaternion&						GetOrientation();
		Math::SVector3&							GetScale();

		void									AddTranslation(Math::SVector3 pTranslation);

		void									SetBaseColorResourceHandle(int pResourceHandle);
		void									SetMetallicResourceHandle(int pResourceHandle);
		void									SetNormalResourceHandle(int pResourceHandle);
		void									SetRoughnessResourceHandle(int pResourceHandle);

		void									SetObjectHandle(int pObjectHandle);
		int										GetObjectHandle() const;

	protected:
		//transform, ...
		Math::CTransform						mTransform;

		int										mBaseColorResourceHandle = -1;
		int										mMetallicResourceHandle = -1;
		int										mNormalResourceHandle = -1;
		int										mRoughnessResourceHandle = -1;

		// handle used by renderer
		int										mObjectHandle;
	};
}
