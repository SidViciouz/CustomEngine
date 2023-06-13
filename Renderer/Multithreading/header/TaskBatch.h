#pragma once

#include "../Common/header/Common.h"

namespace Multithreading
{
	class CTask;

	class CTaskBatch
	{
	public:
												CTaskBatch();
												~CTaskBatch();
		void									AddTask(shared_ptr<CTask> pTask);
		void									Wait();

	private:
		vector<shared_ptr<CTask>>				mTasks;
	};
}