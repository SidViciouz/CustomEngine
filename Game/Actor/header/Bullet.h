#pragma once

#include "Actor.h"
#include "Game/header/Timer.h"
#include "Common/header/Common.h"

namespace Game
{
	class CPhysicsComponent;

	class CBullet : public CActor
	{
	public:
		static shared_ptr<CBullet>				Create(shared_ptr<Renderer::CMesh> pMesh);

												~CBullet();

		virtual void							Reset(shared_ptr<Renderer::CMesh> pMesh) override;

		virtual void							Update(double pDeltaTime);
		virtual void							Listen(const SEvent pEvent) override;

	protected:
												CBullet(shared_ptr<Renderer::CMesh> pMesh);

		void									Destroy();

		CTimer									mTimer;
		shared_ptr<CPhysicsComponent>			mPhysicsComponent;
	};
}
