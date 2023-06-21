#include "Component/header/CameraComponent.h"
#include "../Camera/header/Camera.h"
#include "Actor/header/Actor.h"

namespace Game
{
	CameraComponent::CameraComponent(shared_ptr<CActor> pActor, shared_ptr<Renderer::CCamera> pCamera)
		: IBaseComponent{pActor}, mCamera{pCamera}
	{

	}

	CameraComponent::~CameraComponent()
	{

	}

	void CameraComponent::Update(double pDeltaTime)
	{
		mCamera->GetPosition() = mActor->GetTranslation() + mTranslation;
	}

	void CameraComponent::SetLocalTranslation(Math::SVector3 pTranslation)
	{
		mTranslation = pTranslation;
	}

	void CameraComponent::SetLocalOrientation(Math::SQuaternion pOrientation)
	{
		mOrientation = pOrientation;
	}
}