#ifndef CLUSTER_VIS_H_
#define CLUSTER_VIS_H_

#include "hierarchical_clustering.h"

class Shape;

class ClusterVis {
public:
	ClusterVis(const HierarchicalClustering* _hc);

	void Visualize(int set_type) const;
	void IncrementClusterLevel();
	void DecrementClusterLevel();
	void IncrementRadius();
	void DecrementRadius();

private:
	double Dist(double* pt1, double* pt2, int nd) const;
	Shape GetShape(double* pt, int type_code) const;

private:
	const HierarchicalClustering* hc;	
	int clusterLevel;
	int numPoints;
	double radius;
	
};
#endif  // CLUSTER_VIS_H_