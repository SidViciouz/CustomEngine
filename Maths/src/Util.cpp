#include "Util.h"

namespace Math
{
	unsigned int Alignment(unsigned int pByteSize, unsigned int pAlignment)
	{
		return (pByteSize + pAlignment - 1) & ~(pAlignment - 1);
	}
}