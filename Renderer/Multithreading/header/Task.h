#pragma once

namespace Multithreading
{
	class CTaskBatch;

	class CTask
	{
	public:
													CTask();
		virtual void								Execute() = 0;
	};
}
