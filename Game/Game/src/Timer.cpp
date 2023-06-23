#include "Game/header/Timer.h"
#include <Windows.h>

namespace Game
{
	CTimer::CTimer()
		: mBaseCount{ 0 }, mPreviousCount{ 0 }, mCurrentCount{ 0 }, mDeltaTime{ 0 }
	{
		__int64 lFrequency;

		QueryPerformanceFrequency((LARGE_INTEGER*)&lFrequency);

		mSecondPerCount = 1.0f / static_cast<double>(lFrequency);
	}

	void CTimer::Reset()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mBaseCount);

		mPreviousCount = mBaseCount;
	}

	void CTimer::Tick()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCount);


		mDeltaTime = (mCurrentCount - mPreviousCount) * mSecondPerCount;
		mTotalTime = (mCurrentCount - mBaseCount) * mSecondPerCount;

		if (mDeltaTime < 0.0f)
			mDeltaTime = 0.0f;

		mPreviousCount = mCurrentCount;
	}

	double CTimer::GetDeltaTime() const
	{
		return mDeltaTime;
	}

	double CTimer::GetTotalTime() const
	{
		return mTotalTime;
	}

}