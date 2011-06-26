#ifndef COFENETIC_HPP
#define COFENETIC_HPP

#include "hierarchical_clustering.h"



class CofeneticMeasure
{
private:
	double value;
	double realDistance(double p1[], double p2[], int numDimensions)const;
	double cofDistance(const HierarchicalClustering& hc, int p1Idx, int p2Idx, int vizStep)const;
public:
	double getCppc(const HierarchicalClustering& hierarchicalClustering, int vizStep)const;
};

#endif //COFENETIC_HPP
