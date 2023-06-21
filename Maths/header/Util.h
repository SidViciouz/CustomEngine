#pragma once


namespace Math
{
	// pAlingment must be power of 2
	unsigned int Alignment(unsigned int pByteSize, unsigned int pAlignment);

	double Lerp(double pA, double pB, double pAlpha);
}