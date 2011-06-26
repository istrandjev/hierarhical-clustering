#ifndef HIERARCHICAL_CLUSTERING_HPP
#define HIERARCHICAL_CLUSTERING_HPP

#include "distance.h"
#include "cluster.h"
#include "closest_cluster.h"
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

	HierarchicalClustering(const string& dataFileName, Distance* distance);
	~HierarchicalClustering();

	void calculateHierarchy(Distance* distance);
	double getTotalFMeasure(const vector<vector<int> >& classes);
private:
	double getClassFMeasure(int class_index, int class_size, const vector<int>& actual_classes);
	void constructElements();
};

#endif //HIERARCHICAL_CLUSTERING_HPP