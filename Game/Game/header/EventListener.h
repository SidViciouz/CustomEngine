/*
* event listener listens events from event manager. and do specified action.
*/

#pragma once

#include "Common/header/Common.h"
#include "../Maths/header/Vector3.h"

namespace Game
{
	struct SEvent
	{
		Math::SVector3							mPosition;
	};

	class IEventListener
	{
	public:
												IEventListener() {}
												~IEventListener() {}

		virtual void							Listen(const SEvent pEvent) = 0;

	protected:

	};
}
