#ifndef CENTROID_DISTANCE_H_
#define CENTROID_DISTANCE_H_

#include "distance.h"

class Cluster;
class HierarchicalClustering;

class CentroidDistance : public Distance
{
public:
	double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const;
};

#endif  // CENTROID_DISTANCE_H_
