/*
* physics component has physical properties like velocity, acceleration and so on.
*/
#pragma once

#include "Component/header/BaseComponent.h"
#include "../Maths/header/Vector3.h"

namespace Game
{
	class CActor;

	class CPhysicsComponent : public IBaseComponent
	{
	public:
												CPhysicsComponent(shared_ptr<CActor> pActor);
												~CPhysicsComponent();

		virtual void							Update(double pDeltaTime) override;

		virtual void							Input() override;

		Math::SVector3&							GetVelocity();
		Math::SVector3&							GetAcceleration();

		const Math::SVector3&					GetVelocity() const;
		const Math::SVector3&					GetAcceleration() const;

		void									SetVelocity(const Math::SVector3& pVelocity);
		void									SetAcceleration(const Math::SVector3& pAcceleration);

	protected:

		Math::SVector3							mVelocity;
		Math::SVector3							mAcceleration;
	};
}
