#include "Actor/header/Enemy.h"

namespace Game
{
	shared_ptr<CEnemy> CEnemy::Create(const char* pPath)
	{
		return shared_ptr<CEnemy>(new CEnemy(pPath));
	}

	CEnemy::CEnemy(const char* pPath)
		: ICharacter{ pPath }
	{

	}

	CEnemy::~CEnemy()
	{

	}

	void CEnemy::Update(double pDeltaTime)
	{
		ICharacter::Update(pDeltaTime);
	}

	void CEnemy::Input()
	{
		
	}

}