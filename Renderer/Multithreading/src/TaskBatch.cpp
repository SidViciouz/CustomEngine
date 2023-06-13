#include "../Multithreading/header/TaskBatch.h"

namespace Multithreading
{
	CTaskBatch::CTaskBatch()
	{

	}



	CTaskBatch::~CTaskBatch()
	{

	}



	void CTaskBatch::AddTask(shared_ptr<CTask> pTask)
	{
		mTasks.push_back(pTask);
	}



	void CTaskBatch::Wait()
	{

	}
}