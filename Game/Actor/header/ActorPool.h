/*
* ActorPool manage the actor memory.
* It allocates and deallocates the actor memory.
* And this class is implemented as singleton.
*/
#pragma once

#include "Common/header/Common.h"


namespace Renderer
{
	class CMesh;
}

namespace Game
{
	class CActor;

	class CActorPool
	{
	public:
		static shared_ptr<CActorPool>			Singleton();

												~CActorPool();

		// allocate new actor from pool
		shared_ptr<CActor>						NewActor(shared_ptr<Renderer::CMesh> pMesh);
		// release actor into pool
		void									ReleaseActor(shared_ptr<CActor> pActor);

	protected:
												CActorPool();

		vector<shared_ptr<CActor>>				mActorPool;
		stack<shared_ptr<CActor>>				mReleasedActors;

		int										mMaxActorNum;
		int										mActorNum;
	};
}
