#include "furthest_neighbour.h"
#include "cluster.h"
#include "hierarchical_clustering.h"
#include "constants.h"

double FurthestNeighbour::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	return max(hierarchicalClustering.dists[leftFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()], 
		hierarchicalClustering.dists[rightFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]);
}

std::string FurthestNeighbour::getName() const {
	return "furthest_neighbour";
}
