#pragma once

#include <memory>

using namespace std;

#define MAX_KEY_SIZE 100

namespace Input
{
	class CInputManager
	{
	public:
												CInputManager();

		static shared_ptr<CInputManager>		Singleton();

		void									Reset();

		void									SetKeyDown(int pHandle);
		void									SetKeyUp(int pHandle);

		bool									GetKeyDown(int pHandle);
		bool									GetKeyUp(int pHandle);
		bool									GetKeyPressed(int pHandle);

	private:

		bool									mKeyDown[MAX_KEY_SIZE] = { false, };
		bool									mKeyUp[MAX_KEY_SIZE] = { false, };
		bool									mKeyPressed[MAX_KEY_SIZE] = { false, };
		
	};
}
