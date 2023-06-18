#include "../Multithreading/header/ThreadPool.h"
#include "../Multithreading/header/TaskBatch.h"
#include "../Multithreading/header/Task.h"

namespace Multithreading
{
	CThreadPool::CThreadPool(int pNumThreads)
		: mNumThreads{pNumThreads}, mStop{false}
	{
		//create working thread
		for (int i = 0; i < mNumThreads; ++i)
		{
			mThreads.emplace_back([this]() {
				this->Work();
			});
		}
	}

	CThreadPool::~CThreadPool()
	{

	}

	shared_ptr<CThreadPool> CThreadPool::Singleton()
	{
		static shared_ptr<CThreadPool> mSingletonThreadPool = make_shared<CThreadPool>(8);

		return mSingletonThreadPool;
	}

	void CThreadPool::AddTaskBatch(shared_ptr<CTaskBatch> pTaskBatch)
	{
		while (!pTaskBatch->mTasks.empty())
		{
			{
				unique_lock<mutex> lLock(mMutex);
				mTasks.push({ pTaskBatch->mTasks.front(), pTaskBatch});
				pTaskBatch->mTasks.pop();
				++pTaskBatch->mWorkingTask;
			}
			mCV.notify_one();
		}
	}

	//working thread
	void CThreadPool::Work()
	{
		while (!mStop)
		{
			unique_lock<mutex> lLock(mMutex);
			mCV.wait(lLock, [this]() {
				return !this->mTasks.empty() || mStop;
			});

			pair<shared_ptr<CTask>,shared_ptr<CTaskBatch>> lTaskInfo = mTasks.front();
			shared_ptr<CTask> lTask = lTaskInfo.first;
			mTasks.pop();
			lLock.unlock();

			lTask->Execute();

			{
				unique_lock<mutex> lLock(lTaskInfo.second->mMutex);
				--lTaskInfo.second->mWorkingTask;
			}			
			lTaskInfo.second->mCV.notify_one();
		}
	}
}