/*
* timer class is used to count time.
* it can be used not only for game loop, but also game objects
*/

#pragma once

namespace Game
{
	class CTimer
	{
	public:
												CTimer();
												~CTimer() {}

		void									Reset();

		void									Tick();
		
		double									GetDeltaTime() const;
		double									GetTotalTime() const;

	private:
		double									mDeltaTime;
		double									mTotalTime;
		double									mSecondPerCount;

		long long								mBaseCount;
		long long								mPreviousCount;
		long long								mCurrentCount;

	};
}
