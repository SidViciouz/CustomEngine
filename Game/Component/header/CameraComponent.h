#pragma once

#include "BaseComponent.h"
#include "../Maths/header/Quaternion.h"
#include "../Maths/header/Vector3.h"

namespace Renderer
{
	class CCamera;
}

namespace Game
{
	class CameraComponent : public IBaseComponent
	{
	public:
												CameraComponent(shared_ptr<CActor> pActor, shared_ptr<Renderer::CCamera> pCamera);
												~CameraComponent();

		virtual void							Update(double pDeltaTime) override;

		virtual void							Input() override;

		void									SetDistance(float pDistance);
		void									SetYaw(float pYaw);
		void									SetPitch(float pPitch);

	protected:
		shared_ptr<Renderer::CCamera>			mCamera;
		
		float									mYaw;
		float									mPitch;
		float									mDistance;

		bool									mMouseActivated;
		int										mPrevX;
		int										mPrevY;
	};
}
