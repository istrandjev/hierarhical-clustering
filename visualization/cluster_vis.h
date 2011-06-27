#ifndef CLUSTER_VIS_H_
#define CLUSTER_VIS_H_

#include <hierarchical_clustering.h>

class Shape;
class FMeasureCalculator;
class CopheneticMeasure;

class ClusterVis {
public:
	ClusterVis(const HierarchicalClustering* _hc, const string& remappedFileName);
	~ClusterVis();
	void Visualize(int set_type) const;
	void PrintStats(const CopheneticMeasure& copheneticMeasure, const FMeasureCalculator* fmc) const;
	void IncrementClusterLevel();
	void DecrementClusterLevel();
	void IncrementRadius();
	void DecrementRadius();

private:
	void loadPointsFromFile(const string& remappedFileName);
	double Dist(double* pt1, double* pt2, int nd) const;
	Shape GetShape(double* pt, int type_code) const;
	void printText(const string& str)const;

private:
	const HierarchicalClustering* hc;	
	int clusterLevel;
	double radius;

	double **points;
	int numDimensions;
	int numPoints;

	
};
#endif  // CLUSTER_VIS_H_
