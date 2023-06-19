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
	class CPlayer : public ICharacter
	{
	public:

		static shared_ptr<CPlayer>				Create(const char* pPath);
		static shared_ptr<CPlayer>				Create(shared_ptr<Renderer::CMesh> pMesh);

		virtual void							Update(double pDeltaTime) override;

												~CPlayer();

	protected:

												CPlayer(const char* pPath);
												CPlayer(shared_ptr<Renderer::CMesh> pMesh);
												CPlayer() = delete;

		virtual void							Input() override;
	};
}