/*
* event listener listens events from event manager. and do specified action.
*/

#pragma once

#include "Common/header/Common.h"

namespace Game
{
	struct SEvent
	{
		int										mType;
	};

	class IEventListener
	{
	public:
												IEventListener() {}
												~IEventListener() {}

		virtual void							Listen(SEvent pEvent) = 0;

	protected:

	};
}
