#include "cofenetic.h"
#include "hierarchical_clustering.h"

#include <cmath>
double CofeneticMeasure::realDistance(double p1[], double p2[], int numDimensions)const
{
	double sum = 0;
	for(int i = 0; i < numDimensions; i++)
		sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	return sqrt(sum);
}

double CofeneticMeasure::cofDistance(const HierarchicalClustering& hc, int p1Idx, int p2Idx, int vizStep)const
{
	int cur = 0;

	int step = 1;
	while(step * 2 < hc.numPoints)
		step *= 2;

	while(step > 0)
	{
		cur += step;
		if(cur >= hc.numPoints)
		{
			cur -= step;
			step /= 2;
			continue;
		}
		if(hc.clusterPoints[cur][p1Idx] == hc.clusterPoints[cur][p2Idx])
			cur -= step;
		step /= 2;
	}

	cur++;
	if(cur > vizStep)
		return -1;
	else
		return hc.allClusters[hc.clusterPoints[cur][p1Idx]]->getDistanceOfCreation();
}

double CofeneticMeasure::getCppc(const HierarchicalClustering& hierarchicalClustering, int vizStep)const
{
	double **realDistances;
	double **cofeneticDistances;
	int n = hierarchicalClustering.numPoints;
	realDistances = new double*[n];
	cofeneticDistances = new double*[n];
	for(int i = 0; i < n; i++)
	{
		realDistances[i] = new double[n];
		cofeneticDistances[i] = new double[n];
	}

	double realDistanceAvg = 0.0;
	double cofeneticDistAvg = 0.0;
	int numDists = 0;
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			cofeneticDistances[i][j] = cofDistance(hierarchicalClustering, i, j, vizStep);
			if(cofeneticDistances[i][j] < 0)
				continue;
			cofeneticDistAvg += cofeneticDistances[i][j];

			realDistances[i][j] = realDistance(hierarchicalClustering.points[i],
										hierarchicalClustering.points[j], 
										hierarchicalClustering.numDimensions);
			realDistanceAvg += realDistances[i][j];		
			numDists++;
		}
	}
	realDistanceAvg /= (double)numDists; 
	cofeneticDistAvg /= (double)numDists; 

	double nom = 0.0;
	double denSum1 = 0.0;
	double denSum2 = 0.0;
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			if(cofeneticDistances[i][j] < 0)
				continue;
			nom += (realDistances[i][j] - realDistanceAvg) * (cofeneticDistances[i][j] - cofeneticDistAvg);
			denSum1 += (realDistances[i][j] - realDistanceAvg) * (realDistances[i][j] - realDistanceAvg);
			denSum2 += (cofeneticDistances[i][j] - cofeneticDistAvg) * (cofeneticDistances[i][j] - cofeneticDistAvg);
		}
	}

	for(int i = 0; i < n; i++)
	{
		delete [] realDistances[i];
		delete [] cofeneticDistances[i];
	}
	delete [] realDistances;
	delete [] cofeneticDistances;

	double val = nom / sqrt(denSum1 * denSum2);
	//val *= (double) (2.0 * numDists) / (double)(n * (n - 1));
	return val;
}
