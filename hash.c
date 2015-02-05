//#include "bloom.h"
#include <math.h>
#include <stdlib.h>
#include "hash.h"

double genUniformRandom(double rangeStart, double rangeEnd)
{
	double r;
	do
	{
		r = rangeStart + ((rangeEnd - rangeStart) * rand() / (double) RAND_MAX);
	} while (r < rangeStart || r > rangeEnd);
	return r;
}

double genGaussianRandom()
{
	// Use Box-Muller transform to generate a point from normal
	// distribution.
	double x1, x2, z;
	do
	{
		x1 = genUniformRandom(0.0, 1.0);
	} while (x1 == 0); // cannot take log of 0.
	x2 = genUniformRandom(0.0, 1.0);
	z = sqrt(-2.0 * log(x1)) * cos(2.0 * PI * x2);
	return z;
}
int genRandomInt(int rangeStart, int rangeEnd)
{
	int r;
	do
	{
		r = rangeStart
				+ (int) ((rangeEnd - rangeStart + 1.0) * rand()
						/ (RAND_MAX + 1.0));
	} while (r < rangeStart || r > rangeEnd);
	return r;
}
