#ifndef HIERARCHICAL_CLUSTERING_HPP
#define HIERARCHICAL_CLUSTERING_HPP

#include "distances/distance.h"
#include "cluster.h"
#include "binary_heap.h"

#include <string>
#include <set>
#include <vector>
using namespace std;

class HierarchicalClustering
{
public:
	
	int numDimensions;
	int numPoints;
	double** points;
	double** dists;
	int** clusterPoints;
	int* correspondingClusers;

	BinaryHeap *distsHeap;
	set<int> currentClusters;
	vector<Cluster*> allClusters;

	HierarchicalClustering(const string& dataFileName, const Distance* distance);
	~HierarchicalClustering();

	void calculateHierarchy(const Distance* distance);
private:
	void constructElements();
};

#endif //HIERARCHICAL_CLUSTERING_HPP
