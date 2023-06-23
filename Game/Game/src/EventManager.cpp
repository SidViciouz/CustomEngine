#include "Game/header/EventManager.h"

namespace Game
{
	shared_ptr<CEventManager> CEventManager::Singleton()
	{
		static shared_ptr<CEventManager> lEventManagerSingleton = shared_ptr<CEventManager>(new CEventManager());

		return lEventManagerSingleton;
	}


	void CEventManager::AddListener(shared_ptr<IEventListener> pListener)
	{
		mEventListeners.push_back(pListener);
	}

	void CEventManager::RemoveListener(shared_ptr<IEventListener> pListener)
	{
		for (int i=0; i<mEventListeners.size(); ++i)
		{
			if (mEventListeners[i].get() == pListener.get())
			{
				mEventListeners.erase(mEventListeners.begin() + i);
				return;
			}
		}
	}

	void CEventManager::Notify(IEventListener* pSender, SEvent pEvent)
	{
		for (int i = 0; i < mEventListeners.size(); ++i)
		{
			if (mEventListeners[i].get() == pSender)
				continue;

			mEventListeners[i]->Listen(pEvent);
		}
	}
}