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

		//set animation
		LoadAnimation("n_walkpose_f", "../Model/ALS_N_WalkPose_F.FBX");
		LoadAnimation("n_walk_f", "../Model/ALS_N_Walk_F.FBX");
		LoadAnimation("n_run_f", "../Model/ALS_N_Run_F.FBX");
		LoadAnimation("cls_walk_f", "../Model/ALS_CLF_Walk_F.FBX");
		ResetAnimation("n_walk_f");
		AddAnimTransition("n_walkpose_f", "n_walk_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() > 0.0f)
				return true;
			}, 0.25f);
		AddAnimTransition("n_walk_f", "n_walkpose_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() == 0.0f)
				return true;
		}, 0.1f);
		
		AddAnimTransition("n_walk_f", "n_run_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() > 2.0f)
				return true;
		}, 0.25f);
		AddAnimTransition("n_run_f", "n_walk_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() == 0.0f)
				return true;
		}, 0.1f);
		
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
		bool lMoving = false;

		shared_ptr<Input::CInputManager> lInputManager = Input::CInputManager::Singleton();

		Math::SVector3 lAcceleration;

		if (lInputManager->GetKeyPressed(0x41))
		{
			lAcceleration += Math::SVector3(-1, 0, 0);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x44))
		{
			lAcceleration += Math::SVector3(1, 0, 0);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x57))
		{
			lAcceleration += Math::SVector3(0, 0, 1);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x53))
		{
			lAcceleration += Math::SVector3(0, 0, -1);
			lMoving = true;
		}

		if (lMoving)
		{
			lAcceleration.Normalized();

			if (mPhysicsComponent->GetVelocity().Length() < 2.0f)
			{
				mPhysicsComponent->GetAcceleration() = lAcceleration * 2.0f;
			}
			else
			{
				mPhysicsComponent->GetAcceleration() = Math::SVector3();
			}

		}
		else
		{
			mPhysicsComponent->GetAcceleration() = Math::SVector3();
			mPhysicsComponent->GetVelocity() = Math::SVector3();
		}
	}

}