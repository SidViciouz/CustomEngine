#pragma once

#include "../Common/header/Common.h"

namespace Renderer
{
	class IAnimationBase
	{
	public:
												IAnimationBase(string pName);

		virtual Math::CTransform				EvaluateGlobalTransform(const string& pBoneName, double pTime) = 0;
		virtual Math::CTransform				EvaluateLocalTransform(const string& pBoneName, double pTime) = 0;

		virtual bool							IsBone(const string& pBoneName) const = 0;

		virtual double							GetBeginTime() const = 0;
		virtual double							GetEndTime() const = 0;

		void									SetScale(double pVelocityScale);

	protected:
		string									mName;

		double									mBeginTime;
		double									mEndTime;

		double									mVelocityScale;

	};
}