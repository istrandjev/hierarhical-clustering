#include "nearest_neighbour.h"
#include "cluster.h"
#include "hierarchical_clustering.h"
#include "constants.h"

#include <set>
using namespace std;

double NearestNeighbour::mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
	const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const
{
	return min(hierarchicalClustering.dists[leftFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()], 
		hierarchicalClustering.dists[rightFromCluster.getMinimalPointIndex()][toCluster.getMinimalPointIndex()]);
}


std::string NearestNeighbour::getName() const {
	return "nearest_neighbour";
}