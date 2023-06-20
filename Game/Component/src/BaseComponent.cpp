#include "Component/header/BaseComponent.h"

namespace Game
{
	IBaseComponent::IBaseComponent(shared_ptr<CActor> pActor)
		: mActor{pActor}
	{

	}

	IBaseComponent::~IBaseComponent()
	{

	}
}