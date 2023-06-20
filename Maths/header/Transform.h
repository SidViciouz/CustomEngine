#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Math
{
	class CTransform
	{
	public:
												CTransform();

		SVector3&								GetTranslation();
		const SVector3&							GetTranslation() const;
		void									SetTranslation(const SVector3& pTranslation);

		SQuaternion&							GetOrientation();
		const SQuaternion&						GetOrientation() const;
		void									SetOrientation(const SQuaternion& pOrientation);

		SVector3&								GetScale();
		const SVector3&							GetScale() const;
		void									SetScale(const SVector3& pScale);

	protected:
		SVector3								mTranslation;
		SQuaternion								mOrientation;
		SVector3								mScale;
	};
}
