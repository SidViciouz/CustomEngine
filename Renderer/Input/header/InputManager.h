#pragma once

#include <memory>

using namespace std;

namespace Input
{
	class CInputManager
	{
	public:
												CInputManager();

		static shared_ptr<CInputManager>		Create();

		static shared_ptr<CInputManager>		Get();

	private:

		static shared_ptr<CInputManager>		mThis;
	};
}
