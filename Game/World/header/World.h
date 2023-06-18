/*
* world contains actor to spawn
*/
#pragma once

#include <memory>
#include <vector>

using namespace std;

namespace Game
{
	class CActor;

	class CWorld
	{
	public:
		shared_ptr<CWorld>						Create();
												~CWorld();
		
		void									Update();

		void									Add(shared_ptr<CActor> pActor);

		template<class F, class... Args>
		void									ToAllActors(F pFunction,Args... pArgs);

	protected:
												CWorld();

		vector<shared_ptr<CActor>>				mActors;
	};
}
