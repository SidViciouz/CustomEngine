#include "Actor/header/Character.h"

namespace Game
{
	ICharacter::ICharacter(const char* pPath)
		: CActor{pPath}
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
}