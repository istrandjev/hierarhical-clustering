#include "distance.h"

#include <cmath>

double Distance::initialDistance(double p1[], double p2[], int numDimensions) const
{
	double sum = 0;
	for(int i = 0; i < numDimensions; i++)
		sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	return sqrt(sum);
}
