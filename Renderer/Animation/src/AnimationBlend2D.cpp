#include "../Animation/header/AnimationBlend2D.h"
#include "../../Maths/header/Vector2.h"

namespace Renderer
{
	CAnimationBlend2D::CAnimationBlend2D(string pName)
		: IAnimationBase{pName}
	{

	}

	void CAnimationBlend2D::SetAnimation(shared_ptr<CAnimation> pAnimation, Math::SVector2 pValue)
	{
		mAnimCoords.push_back({ pAnimation ,pValue });
	}

	shared_ptr<CSkeleton> CAnimationBlend2D::GetSkeleton()
	{

	}

	const double& CAnimationBlend2D::GetBeginTime() const
	{

	}

	const double& CAnimationBlend2D::GetEndTime() const
	{

	}
}