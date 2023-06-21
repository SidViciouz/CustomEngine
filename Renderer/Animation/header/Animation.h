#pragma once

#include "../Common/header/Common.h"
#include "../Animation/header/AnimationBase.h"

namespace Math
{
	struct SVector2;
}

namespace Renderer
{
	class CSkeleton;

	class CAnimation : public IAnimationBase
	{
	public:
												CAnimation(string pName, const char* pPath);

		virtual Math::CTransform				EvaluateGlobalTransform(const string& pBoneName, double pTime) override;
		virtual Math::CTransform				EvaluateLocalTransform(const string& pBoneName, double pTime) override;

		virtual bool							IsBone(const string& pBoneName) const override;

		virtual double							GetBeginTime() const override;
		virtual double							GetEndTime() const override;

	protected:
		shared_ptr<CSkeleton>					mSkeleton;
	};
}