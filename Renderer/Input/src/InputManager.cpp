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

	void CInputManager::Reset()
	{
		memset(mKeyDown, false, MAX_KEY_SIZE);
		memset(mKeyUp, false, MAX_KEY_SIZE);
	}

	void CInputManager::SetKeyDown(int pHandle)
	{
		if (pHandle >= MAX_KEY_SIZE)
			return;

		if (mKeyPressed[pHandle])
			return;

		mKeyDown[pHandle] = true;
		mKeyPressed[pHandle] = true;
	}

	void CInputManager::SetKeyUp(int pHandle)
	{
		if (pHandle >= MAX_KEY_SIZE)
			return;

		mKeyUp[pHandle] = true;
		mKeyPressed[pHandle] = false;
	}

	bool CInputManager::GetKeyDown(int pHandle)
	{
		return mKeyDown[pHandle];
	}

	bool CInputManager::GetKeyUp(int pHandle)
	{
		return mKeyUp[pHandle];
	}

	bool CInputManager::GetKeyPressed(int pHandle)
	{
		return mKeyPressed[pHandle];
	}
}