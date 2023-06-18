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

	ICharacter::~ICharacter()
	{

	}

	void ICharacter::Update()
	{
		Input();

		CActor::Update();
	}
}