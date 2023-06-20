#include "../Animation/header/AnimationBlend2D.h"
#include "../../Maths/header/Vector2.h"
#include "../../Maths/header/Transform.h"
#include "../Animation/header/Animation.h"

namespace Renderer
{
	CAnimationBlend2D::CAnimationBlend2D(string pName, const float& pX, const float& pY)
		: IAnimationBase{pName}, mConditionX{pX}, mConditionY{pY}
	{

	}

	void CAnimationBlend2D::SetAnimation(shared_ptr<CAnimation> pAnimation, Math::SVector2 pValue)
	{
		mAnimCoords.push_back({ pAnimation ,pValue });
	}

	Math::CTransform CAnimationBlend2D::EvaluateGlobalTransform(const string& pBoneName, double pTime)
	{
		return Math::CTransform::Blend(mAnimCoords[0].first->EvaluateGlobalTransform(pBoneName, pTime), mAnimCoords[1].first->EvaluateGlobalTransform(pBoneName, pTime),0.5f);
	}

	Math::CTransform CAnimationBlend2D::EvaluateLocalTransform(const string& pBoneName, double pTime)
	{
		return Math::CTransform::Blend(mAnimCoords[0].first->EvaluateLocalTransform(pBoneName, pTime), mAnimCoords[1].first->EvaluateLocalTransform(pBoneName, pTime), 0.5f);
	}

	bool CAnimationBlend2D::IsBone(const string& pBoneName) const
	{
		for (auto& lAnimCoords : mAnimCoords)
		{
			if (!lAnimCoords.first->IsBone(pBoneName))
				return false;
		}

		return true;
	}
	
	const double& CAnimationBlend2D::GetBeginTime() const
	{
		return 0;
	}

	const double& CAnimationBlend2D::GetEndTime() const
	{
		return 1;
	}
}