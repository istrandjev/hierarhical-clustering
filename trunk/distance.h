#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include "cluster.h"
class HierarchicalClustering;

class Distance
{
public:
	virtual double initialDistance(double p1[], double p2[], int numDimensions);
	virtual double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const = 0;
};
#endif