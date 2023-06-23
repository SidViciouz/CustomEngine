#include "Actor/header/Player.h"
#include "../Input/header/InputManager.h"
#include "Component/header/PhysicsComponent.h"
#include "../Animation/header/AnimationBlend2D.h"
#include "../Animation/header/Animation.h"
#include "Game/header/EventManager.h"

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
		
		shared_ptr<Renderer::CAnimation> lWalkF = make_shared<Renderer::CAnimation>("walkF", "../Model/ALS_N_Walk_F.FBX");
		shared_ptr<Renderer::CAnimation> lRunF = make_shared<Renderer::CAnimation>("runB", "../Model/ALS_N_Run_F.FBX");
		shared_ptr<Renderer::CAnimation> lWalkB = make_shared<Renderer::CAnimation>("walkB", "../Model/ALS_N_Walk_B.FBX");
		shared_ptr<Renderer::CAnimation> lRunB = make_shared<Renderer::CAnimation>("runB", "../Model/ALS_N_Run_B.FBX");
		shared_ptr<Renderer::CAnimation> lWalkPoseF = make_shared<Renderer::CAnimation>("walkposeF", "../Model/ALS_N_WalkPose_F.FBX");
		shared_ptr<Renderer::CAnimation> lRunPoseF = make_shared<Renderer::CAnimation>("runposeF", "../Model/ALS_N_RunPose_F.FBX");
		shared_ptr<Renderer::CAnimation> lShooting = make_shared<Renderer::CAnimation>("shooting", "../Model/shooting.FBX");
		lRunPoseF->SetScale(0.0416f);

		LoadAnimation("shooting", lShooting);

		shared_ptr<Renderer::CAnimationBlend2D> lBlendF = make_shared<Renderer::CAnimationBlend2D>("blendF", mPhysicsComponent->GetVelocity().mX, mPhysicsComponent->GetVelocity().mZ);
		lBlendF->SetAxis(-1,1,-1,1);
		lBlendF->SetAnimation(lWalkPoseF,lWalkF, lRunPoseF,lRunF);
		LoadAnimation("blendF", lBlendF);


		shared_ptr<Renderer::CAnimationBlend2D> lBlendB = make_shared<Renderer::CAnimationBlend2D>("blendB", mPhysicsComponent->GetVelocity().mX, mPhysicsComponent->GetVelocity().mZ);
		lBlendB->SetAxis(-1, 1, -1, 1);
		lBlendB->SetAnimation(lWalkPoseF, lWalkB, lRunPoseF, lRunB);
		LoadAnimation("blendB", lBlendB);

		ResetAnimation("n_walk_f");

		/*
		AddAnimTransition("blendF", "blendB", [this]()->bool {
			return true;
		}, 2.0f);
		AddAnimTransition("blendB", "blendF", [this]()->bool {
				return true;
		}, 2.0f);
		*/
		
		AddAnimTransition("n_walk_f", "n_run_f", [this]()->bool {
				return true;
		}, 1.0f);
		AddAnimTransition("n_run_f", "n_walk_f", [this]()->bool {
				return true;
		}, 1.0f);
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


		if (lInputManager->GetKeyDown(0x51))
		{
			mAnimationGraph->PlayAnimation("shooting");
			CEventManager::Singleton()->Notify(this, SEvent{});
		}

		Math::SVector3 lVelocity;

		if (lInputManager->GetKeyPressed(0x41))
		{
			lVelocity += Math::SVector3(-1, 0, 0);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x44))
		{
			lVelocity += Math::SVector3(1, 0, 0);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x57))
		{
			lVelocity += Math::SVector3(0, 0, 1);
			lMoving = true;
		}
		if (lInputManager->GetKeyPressed(0x53))
		{
			lVelocity += Math::SVector3(0, 0, -1);
			lMoving = true;
		}

		if (lVelocity.Length() == 0.0f)
			lMoving = false;

		if (lMoving)
		{
			lVelocity.Normalized();

			mPhysicsComponent->SetVelocity(lVelocity * 3.0f);
		}
		else
		{
			mPhysicsComponent->SetAcceleration(Math::SVector3());
			mPhysicsComponent->SetVelocity(Math::SVector3());
		}
	}

}