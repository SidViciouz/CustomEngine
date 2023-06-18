/*
* Enemy takes input from BehaviorTree
*/

#pragma once

#include "Actor/header/Character.h"

namespace Game
{
	class CEnemy : public ICharacter
	{
	public:

		static shared_ptr<CEnemy>				Create(const char* pPath);

		virtual void							Update() override;

												~CEnemy();

	protected:

												CEnemy(const char* pPath);
												CEnemy() = delete;

		virtual void							Input() override;
	};
}