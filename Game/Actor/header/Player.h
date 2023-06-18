/*
* Player takes input from IO device
*/

#pragma once

#include "Actor/header/Character.h"

namespace Game
{
	class CPlayer : public ICharacter
	{
	public:

		static shared_ptr<CPlayer>				Create(const char* pPath);

		virtual void							Update() override;

												~CPlayer();

	protected:

												CPlayer(const char* pPath);
												CPlayer() = delete;

		virtual void							Input() override;
	};
}