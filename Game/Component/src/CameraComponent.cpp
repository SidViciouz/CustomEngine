#include "Component/header/CameraComponent.h"
#include "../Camera/header/Camera.h"
#include "Actor/header/Actor.h"
#include "../Input/header/InputManager.h"

namespace Game
{
	CameraComponent::CameraComponent(shared_ptr<CActor> pActor, shared_ptr<Renderer::CCamera> pCamera)
		: IBaseComponent{ pActor }, mCamera{ pCamera }, mMouseActivated{ false }, mDistance{ 1.0f }, mYaw {0.0f}, mPitch{0.0f}
	{

	}

	CameraComponent::~CameraComponent()
	{

	}

	void CameraComponent::Update(double pDeltaTime)
	{
		IBaseComponent::Update(pDeltaTime);
		
		/*
		Math::SVector3 lTranslation(0, 0, -mDistance);
		Math::SVector3 lRight(1, 0, 0);

		DirectX::XMMATRIX lRotationY = DirectX::XMMatrixRotationY(mYaw);
		DirectX::XMVECTOR lXMTranslation = DirectX::XMLoadFloat3(&lTranslation.mXmElement);
		DirectX::XMVECTOR lXMRight = DirectX::XMLoadFloat3(&lRight.mXmElement);

		lXMTranslation = DirectX::XMVector3TransformNormal(lXMTranslation, lRotationY);
		lXMRight = DirectX::XMVector3TransformNormal(lXMRight, lRotationY);

		DirectX::XMMATRIX lRotationPitch = DirectX::XMMatrixRotationAxis(lXMRight, mPitch);
		lXMTranslation = DirectX::XMVector3TransformNormal(lXMTranslation, lRotationPitch);

		DirectX::XMStoreFloat3(&lTranslation.mXmElement, lXMTranslation);
		mCamera->GetPosition() = mActor->GetTranslation() +lTranslation;
		*/

		/*
		const Math::SVector3 lTranslation = mActor->GetTranslation();
		mCamera->SetPosition(lTranslation);

		const Math::SVector3 lCameraPosition = mCamera->GetPosition();
		const Math::SVector3 lActorPosition = mActor->GetTranslation();

		printf("%f %f %f, %f %f %f\n", lCameraPosition.mElement[0], lCameraPosition.mElement[1], lCameraPosition.mElement[2],
			lActorPosition.mElement[0],lActorPosition.mElement[1],lActorPosition.mElement[2]);
		*/
	}

	void CameraComponent::Input()
	{
		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();


		if (!mMouseActivated && lInputManager->GetKeyDown(VK_LBUTTON))
		{
			mMouseActivated = true;
		}
		else if (mMouseActivated && lInputManager->GetKeyUp(VK_LBUTTON))
		{
			mMouseActivated = false;
		}

		int lCurrentX = lInputManager->GetMouseX();
		int lCurrentY = lInputManager->GetMouseY();

		if (mMouseActivated)
		{
			float mDeltaX = DirectX::XMConvertToRadians(0.25f * static_cast<float>(lCurrentX - mPrevX));
			float mDeltaY = DirectX::XMConvertToRadians(0.25f * static_cast<float>(lCurrentY - mPrevY));

			mCamera->RotateY(mDeltaX);
			mCamera->RotateX(mDeltaY);
		}

		mPrevX = lCurrentX;
		mPrevY = lCurrentY;
	}

	void CameraComponent::SetDistance(float pDistance)
	{
		mDistance = pDistance;
	}

	void CameraComponent::SetYaw(float pYaw)
	{
		mYaw = pYaw;
	}

	void CameraComponent::SetPitch(float pPitch)
	{
		mPitch = pPitch;
	}
}