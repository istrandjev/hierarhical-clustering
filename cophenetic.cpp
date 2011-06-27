#include "cophenetic.h"
#include "hierarchical_clustering.h"

#include <cmath>
double CopheneticMeasure::realDistance(double p1[], double p2[], int numDimensions)const
{
	double sum = 0;
	for(int i = 0; i < numDimensions; i++)
		sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	return sqrt(sum);
}

double CopheneticMeasure::copheneticDistance(const HierarchicalClustering& hc, int p1Idx, int p2Idx, int vizStep)const
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

double CopheneticMeasure::getCppc(const HierarchicalClustering& hierarchicalClustering, int vizStep)const
{
	double **realDistances;
	double **copheneticDistances;
	int n = hierarchicalClustering.numPoints;
	realDistances = new double*[n];
	copheneticDistances = new double*[n];
	for(int i = 0; i < n; i++)
	{
		realDistances[i] = new double[n];
		copheneticDistances[i] = new double[n];
	}

	double realDistanceAvg = 0.0;
	double copheneticDistAvg = 0.0;
	int numDists = 0;
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			copheneticDistances[i][j] = copheneticDistance(hierarchicalClustering, i, j, vizStep);
			if(copheneticDistances[i][j] < 0)
				continue;
			copheneticDistAvg += copheneticDistances[i][j];

			realDistances[i][j] = realDistance(hierarchicalClustering.points[i],
										hierarchicalClustering.points[j], 
										hierarchicalClustering.numDimensions);
			realDistanceAvg += realDistances[i][j];		
			numDists++;
		}
	}
	realDistanceAvg /= (double)numDists; 
	copheneticDistAvg /= (double)numDists; 

	double nom = 0.0;
	double denSum1 = 0.0;
	double denSum2 = 0.0;
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			if(copheneticDistances[i][j] < 0)
				continue;
			nom += (realDistances[i][j] - realDistanceAvg) * (copheneticDistances[i][j] - copheneticDistAvg);
			denSum1 += (realDistances[i][j] - realDistanceAvg) * (realDistances[i][j] - realDistanceAvg);
			denSum2 += (copheneticDistances[i][j] - copheneticDistAvg) * (copheneticDistances[i][j] - copheneticDistAvg);
		}
	}

	for(int i = 0; i < n; i++)
	{
		delete [] realDistances[i];
		delete [] copheneticDistances[i];
	}
	delete [] realDistances;
	delete [] copheneticDistances;

	double val = nom / sqrt(denSum1 * denSum2);
	//val *= (double) (2.0 * numDists) / (double)(n * (n - 1));
	return val;
}
