#include "centroid_distance.h" 

#include "cluster.h"
#include "hierarchical_clustering.h"
#include "constants.h"

#include <cmath>

double CentroidDistance::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	double res = 0.0;
	double* left_sum = leftFromCluster.getSumCoordinates();
	double* right_sum = rightFromCluster.getSumCoordinates();
	double* to_sum = toCluster.getSumCoordinates();
	double total_points = double(leftFromCluster.getNumPoints() + rightFromCluster.getNumPoints());
	for (int i =0 ; i < hierarchicalClustering.numDimensions; ++i) {
		double x = (left_sum[i] + right_sum[i])/total_points;
		double ox = to_sum[i]/double(toCluster.getNumPoints());
		res += (x-ox)*(x-ox);
	}
	return sqrt(res);
}

