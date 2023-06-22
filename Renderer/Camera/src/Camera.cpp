#include "../Camera/header/Camera.h"

namespace Renderer
{
	CCamera::CCamera(const int& pWidth, const int& pHeight)
	{
		mRight.Set(1, 0, 0);
		mUp.Set(0, 1, 0);
		mFront.Set(0, 0, 1);

		mPosition.Set(0.0, 0.0, 0.0f);

		mWidth = pWidth;
		mHeight = pHeight;

		mFov = DirectX::XMConvertToRadians(45.0f);
		mMinPlane = 0.1f;
		mMaxPlane = 1000.0f;
	}
	
	void CCamera::Resize(const int& pWidth, const int& pHeight)
	{
		mWidth = pWidth;
		mHeight = pHeight;
	}

	Math::SMatrix4 CCamera::GetViewMatrix()
	{
		Math::SMatrix4 lView;

		float x = -Math::SVector3::Dot(mPosition, mRight);
		float y = -Math::SVector3::Dot(mPosition, mUp);
		float z = -Math::SVector3::Dot(mPosition, mFront);

		lView.SetColumn(0, mRight);
		lView.SetColumn(1, mUp);
		lView.SetColumn(2, mFront);
		lView.SetRow(3, Math::SVector3(x, y, z));

		return lView;
	}

	Math::SMatrix4 CCamera::GetProjectionMatrix()
	{
		Math::SMatrix4 lProjection;

		DirectX::XMFLOAT4X4 lXmProjection;
		DirectX::XMStoreFloat4x4(&lXmProjection, DirectX::XMMatrixPerspectiveFovLH(mFov, static_cast<float>(mWidth) / static_cast<float>(mHeight), mMinPlane, mMaxPlane));
		lProjection = lXmProjection;

		return lProjection;
	}
	
	const Math::SVector3& CCamera::GetPosition() const
	{
		return mPosition;
	}

	void CCamera::SetPosition(Math::SVector3 pPosition)
	{
		mPosition = pPosition;
	}

	void CCamera::RotateX(float pAngle)
	{
		DirectX::XMMATRIX lRotation = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&mRight.mXmElement),pAngle);

		DirectX::XMStoreFloat3(&mUp.mXmElement, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mUp.mXmElement), lRotation));
		DirectX::XMStoreFloat3(&mFront.mXmElement, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mFront.mXmElement), lRotation));
	}

	void CCamera::RotateY(float pAngle)
	{
		DirectX::XMMATRIX lRotation = DirectX::XMMatrixRotationY(pAngle);

		DirectX::XMStoreFloat3(&mRight.mXmElement, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mRight.mXmElement), lRotation));
		DirectX::XMStoreFloat3(&mUp.mXmElement, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mUp.mXmElement), lRotation));
		DirectX::XMStoreFloat3(&mFront.mXmElement, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&mFront.mXmElement), lRotation));
	}

}