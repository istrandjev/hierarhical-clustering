#include "complete_link.h"
#include "cluster.h"
#include "hierarchical_clustering.h"
#include "constants.h"

double CompleteLink::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	return max(hierarchicalClustering.dists[leftFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()], 
		hierarchicalClustering.dists[rightFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]);
}

std::string CompleteLink::getName() const {
	return "complete_link";
}
