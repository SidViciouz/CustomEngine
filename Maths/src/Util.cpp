#include "Util.h"

namespace Math
{
	unsigned int Alignment(unsigned int pByteSize, unsigned int pAlignment)
	{
		return (pByteSize + pAlignment - 1) & ~(pAlignment - 1);
	}

	double Lerp(double pA, double pB, double pAlpha)
	{
		return pA * (1.0f - pAlpha) + pB * pAlpha;
	}

}