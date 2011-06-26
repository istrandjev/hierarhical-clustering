#include "cluster_vis.h"
#include "shape_vis.h"

#include <cmath>

ClusterVis::ClusterVis(const HierarchicalClustering* _hc) {
	hc = _hc;
	if (hc->numDimensions < 2 || hc->numDimensions > 3) {
		throw "Sorry, I can't visualize that!";
	}
	clusterLevel = 0;
	radius = 1e100;
	numPoints = hc->numPoints;
	for (int i = 0; i < numPoints; ++i) {
		for (int j = i + 1;j < numPoints; ++j) {
			double temp = Dist(hc->points[i], hc->points[j], hc->numDimensions);
			if (temp < radius) {
				radius = temp;
			}
		}	
	}
	radius *= 0.2;
	radius =  max(radius, 0.01);
}


void ClusterVis::Visualize(int set_type = -1) const {
	for (int i = 0;i < numPoints; ++i) {
		int temp_type = hc->allClusters[hc->clusterPoints[clusterLevel][i]]->getMinimalPointIndex();
		const Shape& shape = GetShape(hc->points[i], temp_type);
		shape.Display();
	}
}

void ClusterVis::IncrementClusterLevel() {
	if (clusterLevel < hc->numPoints - 1) {
		++clusterLevel;
	}
}

void ClusterVis::DecrementClusterLevel() {
	if (clusterLevel > 0) {
		--clusterLevel;
	}
}

void ClusterVis::IncrementRadius() {
	radius *= 1.1;
}

void ClusterVis::DecrementRadius() {
	radius *= 0.9;
}

double ClusterVis::Dist(double* pt1, double* pt2, int nd) const {
	double res = 0.0;
	for (int i = 0;i < nd; ++i) {
		res += sqrt((pt1[i] - pt2[i])*(pt1[i] - pt2[i]));
	}
	return res;
}

Shape ClusterVis::GetShape(double* pt, int type_code) const { 
	double x = pt[0];
	double y = pt[1];
	double z = 0.0;
	if (hc->numDimensions == 3) {
		z = pt[2];
	}
	return Shape(x,y,z, radius, type_code);
}
