/*
* this class is interface for all component.
*/

#pragma once

#include <memory>

using namespace std;

namespace Game
{
	class CActor;

	class IBaseComponent
	{
	public:
												IBaseComponent(shared_ptr<CActor> pActor);
												~IBaseComponent();

		virtual void							Update(double pDeltaTime) = 0;

	protected:

		shared_ptr<CActor>						mActor;
	};
}
