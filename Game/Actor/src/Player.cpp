#include "Actor/header/Player.h"
#include "../Input/header/InputManager.h"
#include "Physics/header/PhysicsComponent.h"

namespace Game
{
	shared_ptr<CPlayer> CPlayer::Create(shared_ptr<Renderer::CMesh> pMesh)
	{
		return shared_ptr<CPlayer>(new CPlayer(pMesh));
	}

	CPlayer::CPlayer(shared_ptr<Renderer::CMesh> pMesh)
		: ICharacter{ pMesh }
	{
		mPhysicsComponent = make_shared<CPhysicsComponent>(shared_ptr<CActor>(this));
		AddComponent(mPhysicsComponent);
	}

	CPlayer::~CPlayer()
	{

	}

	void CPlayer::Update(double pDeltaTime)
	{
		ICharacter::Update(pDeltaTime);
	}

	void CPlayer::Input()
	{
		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();

		if (lInputManager->GetKeyPressed(0x41))
		{
			mPhysicsComponent->GetAcceleration() += Math::SVector3(-0.01f, 0, 0);
		}
		if (lInputManager->GetKeyPressed(0x44))
		{
			mPhysicsComponent->GetAcceleration() += Math::SVector3(0.01f, 0, 0);
		}
		if (lInputManager->GetKeyPressed(0x57))
		{
			mPhysicsComponent->GetAcceleration() += Math::SVector3(0, 0, 0.01f);
		}
		if (lInputManager->GetKeyPressed(0x53))
		{
			mPhysicsComponent->GetAcceleration() += Math::SVector3(0, 0, -0.01f);
		}
	}

}