#include "Actor/header/Character.h"

namespace Game
{
	ICharacter::ICharacter(const char* pPath)
		: CActor{ pPath }
	{
		if (mMesh->HasSkeleton())
		{
			mAnimationGraph = make_shared<Renderer::CAnimationGraph>(mMesh->GetSkeleton());
			mHasAnimationGraph = true;
		}
		else
		{
			mHasAnimationGraph = false;
		}
	}

	ICharacter::ICharacter(shared_ptr<Renderer::CMesh> pMesh)
		: CActor{ pMesh }
	{
		if (mMesh->HasSkeleton())
		{
			mAnimationGraph = make_shared<Renderer::CAnimationGraph>(mMesh->GetSkeleton());
			mHasAnimationGraph = true;
		}
		else
		{
			mHasAnimationGraph = false;
		}
	}

	ICharacter::~ICharacter()
	{

	}

	void ICharacter::Update(double pDeltaTime)
	{
		Input();

		CActor::Update(pDeltaTime);

		//animation multithreading으로 최적화해야함.
		if (mHasAnimationGraph)
			mAnimationGraph->Update(pDeltaTime);
	}

	void ICharacter::LoadAnimation(const string& pName, const char* pPath)
	{
		mAnimationGraph->LoadAnimation(pName, pPath);
	}

	void ICharacter::AddAnimTransition(const string& pFrom, const string& pTo, const std::function<bool(void)>& pCondition, const double& pDuration)
	{
		mAnimationGraph->AddTransition(pFrom, pTo, pCondition, pDuration);
	}

	bool ICharacter::ResetAnimation(const string& pName)
	{
		return mAnimationGraph->Reset(pName);
	}
}