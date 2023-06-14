#include "../Input/header/InputManager.h"


namespace Input
{
	shared_ptr<CInputManager> CInputManager::mThis;

	CInputManager::CInputManager()
	{

	}

	shared_ptr<CInputManager> CInputManager::Create()
	{
		if (mThis == nullptr)
			mThis = make_shared<CInputManager>();

		return mThis;
	}

	shared_ptr<CInputManager> CInputManager::Get()
	{
		return mThis;
	}
}