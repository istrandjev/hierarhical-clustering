#ifndef NEAREST_NEIGHBOUR_HPP
#define NEAREST_NEIGHBOUR_HPP

#include "distance.h"

class Cluster;
class HierarchicalClustering;

class NearestNeighbour: public Distance
{
public:
	double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const;
};
#endif //NEAREST_NEIGHBOUR_HPP