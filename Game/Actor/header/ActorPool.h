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
		template<typename T, typename std::enable_if<std::is_base_of<CActor, T>::value, int>::type = 0>
		shared_ptr<T>							NewActor(shared_ptr<Renderer::CMesh> pMesh)
		{
			if (!mReleasedActors.empty())
			{
				shared_ptr<T> lTop = dynamic_pointer_cast<T>(mReleasedActors.top());
				mReleasedActors.pop();

				return lTop;

			}
			else if (mActorNum < mMaxActorNum)
			{
				mActorPool.push_back(T::Create(pMesh));

				++mActorNum;

				return dynamic_pointer_cast<T>(mActorPool.back());
			}
			else
			{
				return nullptr;
			}
		}
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
