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

	class CTimer;

	class CGame
	{
	public:
		static shared_ptr<CGame>				Singleton();
												~CGame();
		void									Initialize(HINSTANCE pHInstance);

		virtual void							Begin();
		void									Loop();

		//set actor to world and register it to renderer
		void									RegisterActor(shared_ptr<CActor> pActor);
		void									UnregisterActor(shared_ptr<CActor> pActor);

	protected:
												CGame();


		shared_ptr<CWorld>						mWorld;
		shared_ptr<Renderer::CRenderer>			mRenderer;
		shared_ptr<CTimer>						mTimer;
	};
}
