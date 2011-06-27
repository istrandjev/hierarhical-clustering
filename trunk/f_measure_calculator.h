#ifndef F_MEASURE_CALCULATOR_H
#define F_MEASURE_CALCULATOR_H

#include <string>
#include <vector>

#include "hierarchical_clustering.h"

class FMeasureCalculator {
public:
	FMeasureCalculator(const HierarchicalClustering* hirarchical_clustering, const std::string& classes_file);
	double getFMeasure(size_t level, size_t class_index) const;
	double getAverageFMeasure(size_t level) const;
private:
	void computeFMeasures(); 
	void computeFMeasuresForClass(size_t class_index);
	int getClusterLevel(int cluster_index) const;
	size_t getClassesCount() const;
private:
	const HierarchicalClustering* hc;
	std::vector<std::vector<double> > fMeasuresPerLevel;
	std::vector<int> actualClasses;
	std::vector<size_t> classSizes;
};
#endif  // F_MEASURE_CALCULATOR_H