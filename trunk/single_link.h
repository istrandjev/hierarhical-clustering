#ifndef NEAREST_NEIGHBOUR_HPP
#define NEAREST_NEIGHBOUR_HPP

#include "distance.h"

class Cluster;
class HierarchicalClustering;

class SingleLink: public Distance
{
public:
	virtual double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const;

	virtual std::string getName() const;
};
#endif //NEAREST_NEIGHBOUR_HPP
