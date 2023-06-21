#include "Actor/header/Player.h"
#include "../Input/header/InputManager.h"
#include "Component/header/PhysicsComponent.h"
#include "../Animation/header/AnimationBlend2D.h"
#include "../Animation/header/Animation.h"

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
		
		shared_ptr<Renderer::CAnimation> lWalkF = make_shared<Renderer::CAnimation>("walkB", "../Model/ALS_N_Walk_F.FBX");
		shared_ptr<Renderer::CAnimation> lRunF = make_shared<Renderer::CAnimation>("runB", "../Model/ALS_N_Run_F.FBX");
		shared_ptr<Renderer::CAnimation> lWalkB = make_shared<Renderer::CAnimation>("walkB", "../Model/ALS_N_Walk_B.FBX");
		shared_ptr<Renderer::CAnimation> lRunB = make_shared<Renderer::CAnimation>("runB", "../Model/ALS_N_Run_B.FBX");
		shared_ptr<Renderer::CAnimation> lWalkPoseF = make_shared<Renderer::CAnimation>("walkposeF", "../Model/ALS_N_WalkPose_F.FBX");
		shared_ptr<Renderer::CAnimation> lRunPoseF = make_shared<Renderer::CAnimation>("runposeF", "../Model/ALS_N_RunPose_F.FBX");
		lRunPoseF->SetScale(0.0416f);

		shared_ptr<Renderer::CAnimationBlend2D> lBlendF = make_shared<Renderer::CAnimationBlend2D>("blendF", mPhysicsComponent->GetVelocity().mX, mPhysicsComponent->GetVelocity().mZ);
		lBlendF->SetAxis(-1,1,-1,1);
		lBlendF->SetAnimation(lWalkPoseF,lWalkF, lRunPoseF,lRunF);
		LoadAnimation("blendF", lBlendF);


		shared_ptr<Renderer::CAnimationBlend2D> lBlendB = make_shared<Renderer::CAnimationBlend2D>("blendB", mPhysicsComponent->GetVelocity().mX, mPhysicsComponent->GetVelocity().mZ);
		lBlendB->SetAxis(-1, 1, -1, 1);
		lBlendB->SetAnimation(lWalkPoseF, lWalkB, lRunPoseF, lRunB);
		LoadAnimation("blendB", lBlendB);

		ResetAnimation("blendF");

		/*
		AddAnimTransition("blendF", "blendB", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() > 0.0f)
				return true;
			}, 0.0f);
		AddAnimTransition("blendB", "blendF", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() == 0.0f)
				return true;
		}, 0.0f);
		
		AddAnimTransition("n_walk_f", "n_run_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() > 2.0f)
				return true;
		}, 0.25f);
		AddAnimTransition("n_run_f", "n_walk_f", [this]()->bool {
			if (mPhysicsComponent->GetVelocity().Length() == 0.0f)
				return true;
		}, 0.1f);
		*/
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
				mPhysicsComponent->GetAcceleration() = lAcceleration * 3.0f;
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