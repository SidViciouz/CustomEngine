#include "Physics/header/PhysicsComponent.h"
#include "Actor/header/Actor.h"

namespace Game
{
	CPhysicsComponent::CPhysicsComponent(shared_ptr<CActor> pActor) 
		: IBaseComponent{pActor}
	{

	}

	CPhysicsComponent::~CPhysicsComponent()
	{

	}

	void CPhysicsComponent::Update(double pDeltaTime)
	{
		Math::SVector3& lTranslation = mActor->GetTranslation();
		
		mVelocity += (mAcceleration * pDeltaTime);

		lTranslation += (mVelocity * pDeltaTime);
	}

	Math::SVector3& CPhysicsComponent::GetVelocity()
	{
		return mVelocity;
	}

	Math::SVector3& CPhysicsComponent::GetAcceleration()
	{
		return mAcceleration;
	}

	const Math::SVector3& CPhysicsComponent::GetVelocity() const
	{
		return mVelocity;
	}

	const Math::SVector3& CPhysicsComponent::GetAcceleration() const
	{
		return mAcceleration;
	}

	void CPhysicsComponent::SetVelocity(const Math::SVector3& pVelocity)
	{
		mVelocity = pVelocity;
	}

	void CPhysicsComponent::SetAcceleration(const Math::SVector3& pAcceleration)
	{
		mAcceleration = pAcceleration;
	}
}