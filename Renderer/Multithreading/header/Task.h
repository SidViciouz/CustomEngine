#pragma once

namespace Multithreading
{
	class CTask
	{
	public:
													CTask();
		virtual void								Execute() = 0;
	};
}
