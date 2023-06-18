#include "Actor/header/Player.h"
#include "../Input/header/InputManager.h"

namespace Game
{
	shared_ptr<CPlayer> CPlayer::Create(const char* pPath)
	{
		return shared_ptr<CPlayer>(new CPlayer(pPath));
	}

	CPlayer::CPlayer(const char* pPath)
		: ICharacter{pPath}
	{

	}

	CPlayer::~CPlayer()
	{

	}

	void CPlayer::Update()
	{
		ICharacter::Update();
	}

	void CPlayer::Input()
	{
		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();

		if (lInputManager->GetKeyPressed(0x41))
		{
			mObject->AddTranslation(Math::SVector3(-0.01f, 0, 0));
		}
		if (lInputManager->GetKeyPressed(0x44))
		{
			mObject->AddTranslation(Math::SVector3(0.01f, 0, 0));
		}
		if (lInputManager->GetKeyPressed(0x57))
		{
			mObject->AddTranslation(Math::SVector3(0, 0, 0.01f));
		}
		if (lInputManager->GetKeyPressed(0x53))
		{
			mObject->AddTranslation(Math::SVector3(0, 0, -0.01f));
		}
	}

}