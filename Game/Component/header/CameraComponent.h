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

		void									SetLocalTranslation(Math::SVector3 pTranslation);
		void									SetLocalOrientation(Math::SQuaternion pOrientation);

	protected:
		shared_ptr<Renderer::CCamera>			mCamera;
		
		Math::SVector3							mTranslation;
		Math::SQuaternion						mOrientation;
	};
}
