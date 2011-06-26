#ifndef FURTHEST_NEIGHBOUR_HPP
#define FURTHEST_NEIGHBOUR_HPP

#include "distance.h"

class HierarchicalClustering;
class Cluster;

class FurthestNeighbour: public Distance
{
public:
	double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const;
};
#endif //FURTHEST_NEIGHBOUR_HPP
