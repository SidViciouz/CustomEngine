/*
* event manager get notified events from object and dispatch it to listeners.
*/
#pragma once

#include "Game/header/EventListener.h"
#include "Common/header/Common.h"

namespace Game
{
	class CEventManager
	{
	public:

		static shared_ptr<CEventManager>		Singleton();
												~CEventManager() {}

		void									AddListener(shared_ptr<IEventListener> pListener);
		void									RemoveListener(shared_ptr<IEventListener> pListener);

		void									Notify(IEventListener* pSender, SEvent pEvent);

	protected:
												CEventManager() {}
		
		vector<shared_ptr<IEventListener>>		mEventListeners;
	};
}