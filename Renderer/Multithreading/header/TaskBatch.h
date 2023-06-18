#pragma once

#include "../Common/header/Common.h"
#include <mutex>
#include <condition_variable>
#include <queue>

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

		int										mWorkingTask;
		queue<shared_ptr<CTask>>				mTasks;
		mutex									mMutex;
		condition_variable						mCV;
	};
}