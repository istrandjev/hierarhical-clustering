#include "average_distance.h"
#include "hierarchical_clustering.h"

double AverageDistance::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	double res = hierarchicalClustering.dists[leftFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]*
			double(leftFromCluster.getNumPoints());
	res += hierarchicalClustering.dists[rightFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]*
			double(rightFromCluster.getNumPoints());
	res /= double(leftFromCluster.getNumPoints() + rightFromCluster.getNumPoints());
	return res;
}
