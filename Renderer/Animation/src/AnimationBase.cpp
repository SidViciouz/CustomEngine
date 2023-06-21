#include "../Animation/header/AnimationBase.h"

namespace Renderer
{
	IAnimationBase::IAnimationBase(string pName)
		: mName{pName}, mVelocityScale{1.0f}
	{

	}

	void IAnimationBase::SetScale(double pVelocityScale)
	{
		mVelocityScale = pVelocityScale;
	}

}