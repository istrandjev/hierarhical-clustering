#include "f_measure_calculator.h"
#include "hierarchical_clustering.h"
#include <fstream>

FMeasureCalculator::FMeasureCalculator(
		const HierarchicalClustering* hirarchical_clustering,
		const std::string& classes_file) : hc(hirarchical_clustering) {
	const int notset_value = -1;
	actualClasses.resize(hc->allClusters.size(), notset_value);
	std::ifstream in(classes_file.c_str());
	if (!in) {
		throw new std::exception("Could not open settings file");
	}
	
	size_t number_of_classes;
	in >> number_of_classes;
	classSizes.resize(number_of_classes);
	
	size_t total_classes_size = 0;
	for (size_t class_index = 0;class_index < number_of_classes; ++class_index) {
		size_t class_size;
		in >> class_size;
		total_classes_size += class_size;
		classSizes[class_index] = class_size;

		for (size_t j = 0; j < class_size; ++j) {
			size_t point_index;
			in >> point_index;
			if (actualClasses[point_index] != notset_value) {
				throw new std::exception("Classes file contains a poitn index more then once!");
			}
			if (point_index < 0 || point_index >= actualClasses.size()) {
				throw new std::exception("Classes file contains invalid index!");
			}
			actualClasses[point_index] = class_index;
		}
	}

	if (total_classes_size != hc->numPoints) {
		throw new std::exception("Not all points are assigned to a class!");
	}
	computeFMeasures();
}

double FMeasureCalculator::getFMeasure(size_t level, size_t class_index) const {
	if (level >= fMeasuresPerLevel.size() || level < 0) {
		throw new std::exception("Wrong level given to getFMeasure!");
	}
	if (class_index >= fMeasuresPerLevel[level].size() || class_index < 0) {
		throw new std::exception("Wrong class index given to getFMeasure!");
	}
	return fMeasuresPerLevel[level][class_index];
}

double FMeasureCalculator::getAverageFMeasure(size_t level) const {
	if (level >= fMeasuresPerLevel.size() || level < 0) {
		throw new std::exception("Wrong level given to getAverageFMeasure!");
	}
	double result = 0.0;
	for (size_t class_index = 0; class_index < getClassesCount(); ++class_index) {
		result += getFMeasure(level, class_index);
	}
	result /= double(getClassesCount());
	return result;
}

void FMeasureCalculator::computeFMeasures() {
	fMeasuresPerLevel.resize(hc->numPoints);
	for (size_t level_index = 0; level_index < fMeasuresPerLevel.size(); ++level_index) {
		fMeasuresPerLevel[level_index].resize(getClassesCount(), 0.0);
	}
	for (size_t class_index = 0; class_index < getClassesCount(); ++class_index) {
		computeFMeasuresForClass(class_index);
	}
}

void FMeasureCalculator::computeFMeasuresForClass(size_t class_index) {
	double f_measure = 0.0;
	vector<int> num_objects(hc->allClusters.size(), 0);
	for (size_t i = 0; i < hc->allClusters.size(); ++i){
		const Cluster* cluster = hc->allClusters[i];
		if (!cluster->combinesClusters()) {
			if (actualClasses[cluster->getMinimalPointIndex()] == class_index) {
				num_objects[cluster->getClusterIdx()] = 1;
			}
		} else {
			num_objects[cluster->getClusterIdx()] = num_objects[cluster->getCluster1()->getClusterIdx()] +
					num_objects[cluster->getCluster2()->getClusterIdx()];
		}
	}
	for (size_t cluster_index = 0; cluster_index < hc->allClusters.size(); ++cluster_index) {
		double temp_precision = double(num_objects[cluster_index])/
				double(hc->allClusters[cluster_index]->getNumPoints());
		double temp_recall = double(num_objects[cluster_index])/
				double(classSizes[class_index]);
		if (temp_precision < 1e-9 || temp_recall <  1e-9) {
			continue;
		}
		double temp_f_measure = (2.0*temp_precision*temp_recall)/ (temp_precision + temp_recall);
		fMeasuresPerLevel[getClusterLevel(cluster_index)][class_index] = 
				max(fMeasuresPerLevel[getClusterLevel(cluster_index)][class_index], temp_f_measure);
	}
	for (size_t level = 1; level < fMeasuresPerLevel.size(); ++level) {
		fMeasuresPerLevel[level][class_index] = max(fMeasuresPerLevel[level][class_index], 
				fMeasuresPerLevel[level-1][class_index]);
	}
}

int FMeasureCalculator::getClusterLevel(int cluster_index) const {
	if (cluster_index < hc->numPoints) {
		return 0;
	}
	return cluster_index - hc->numPoints + 1;
}

size_t FMeasureCalculator::getClassesCount() const {
	return classSizes.size();
}