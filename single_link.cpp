#include "single_link.h"
#include "cluster.h"
#include "hierarchical_clustering.h"
#include "constants.h"

#include <set>
using namespace std;

double SingleLink::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	return min(hierarchicalClustering.dists[leftFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()], 
		hierarchicalClustering.dists[rightFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]);
}


std::string SingleLink::getName() const {
	return "single_link";
}