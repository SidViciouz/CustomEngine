#include "../Multithreading/header/TaskBatch.h"

namespace Multithreading
{
	CTaskBatch::CTaskBatch() :
		mWorkingTask{0}
	{

	}



	CTaskBatch::~CTaskBatch()
	{

	}



	void CTaskBatch::AddTask(shared_ptr<CTask> pTask)
	{
		mTasks.push(pTask);
	}



	void CTaskBatch::Wait()
	{
		unique_lock<mutex> lLock(mMutex);
		if (mWorkingTask != 0)
		{
			mCV.wait(lLock, [this]() {
				return this->mWorkingTask == 0;
			});
		}

		lLock.unlock();
	}
}