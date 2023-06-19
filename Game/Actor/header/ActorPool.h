/*
* ActorPool manage the actor memory.
* It allocates and deallocates the actor memory.
* And this class is implemented as singleton.
*/
#pragma once

#include <memory>

using namespace std;

namespace Game
{
	class CActorPool
	{
	public:
		static shared_ptr<CActorPool>			Singleton();

												~CActorPool();
	protected:
												CActorPool();
	};
}
