/*
* Player takes input from IO device
*/

#pragma once

#include "Actor/header/Character.h"

namespace Renderer
{
	class CMesh;
}

namespace Game
{
	class CPhysicsComponent;

	class CPlayer : public ICharacter
	{
	public:

		static shared_ptr<CPlayer>				Create(shared_ptr<Renderer::CMesh> pMesh);

		virtual void							Update(double pDeltaTime) override;

												~CPlayer();

	protected:

												CPlayer(shared_ptr<Renderer::CMesh> pMesh);
												CPlayer() = delete;

		virtual void							Input() override;

		shared_ptr<CPhysicsComponent>			mPhysicsComponent;
	};
}