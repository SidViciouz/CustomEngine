#pragma once

#include "World/header/World.h"
#include <wrl.h>

namespace Renderer
{
	class CRenderer;
}

namespace Game
{
	class CActor;
	class CActorPool;

	class CGame
	{
	public:
												CGame();
												~CGame();
		void									Initialize(HINSTANCE pHInstance);

		virtual void							Begin();
		void									Loop();

	protected:

		//set actor to world and register it to renderer
		void									RegisterActor(shared_ptr<CActor> pActor);
		void									UnregisterActor(shared_ptr<CActor> pActor);

		shared_ptr<CWorld>						mWorld;
		shared_ptr<Renderer::CRenderer>			mRenderer;

		//ActorPool
	};
	
}
