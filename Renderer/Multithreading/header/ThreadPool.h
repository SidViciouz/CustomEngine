#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "../Multithreading/header/TaskBatch.h"

using namespace std;

namespace Multithreading
{
	class CTaskBatch;

	class CThreadPool
	{

	public:
												CThreadPool(int pNumThreads);
												~CThreadPool();
		void									AddTaskBatch(shared_ptr<CTaskBatch> pTaskBatch);

		static shared_ptr<CThreadPool>			Singleton();
	
	private:

		void									Work();

		queue<pair<shared_ptr<CTask>, shared_ptr<CTaskBatch>>>	mTasks;
		bool									mStop;
		int										mNumThreads;
		vector<thread>							mThreads;
		mutex									mMutex;
		condition_variable						mCV;

	};
}
