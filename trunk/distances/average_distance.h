#ifndef AVERAGE_DISTANCE_H_
#define AVERAGE_DISTANCE_H_
#include "distance.h"

class HierarchicalClustering;
class Cluster;

class AverageDistance : public Distance
{
public:
	virtual double mergedDistance(const Cluster& leftFromCluster, const Cluster& rightFromCluster, 
		const Cluster& toCluster, HierarchicalClustering& hierarchicalClustering) const;

	virtual std::string getName() const;
};
#endif  // AVERAGE_DISTANCE_H_
