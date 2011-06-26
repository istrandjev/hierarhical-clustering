#include "hierarchical_clustering.h"
#include "distance.h"
#include "cluster.h"
#include "closest_cluster.h"
#include "constants.h"

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

HierarchicalClustering::HierarchicalClustering(const string& dataFileName, Distance* distance)
{
	ifstream in(dataFileName.c_str());
	in >> numDimensions >> numPoints;

	constructElements();
	for(int i = 0; i < numPoints; i++)
		for(int j = 0; j < numDimensions; j++)
			in >> points[i][j];

	for(int i = 0; i < numPoints; i++)
	{
		for(int j = i + 1; j < numPoints; j++)
		{
			dists[i][j] = distance->initialDistance(points[i], points[j], numDimensions);
		}
	}

	for(int i = 0; i < numPoints; i++)
	{
		Cluster* newCluster = new Cluster(numDimensions, i);
		for (int j = 0;j < numDimensions; ++j) {
			newCluster->setSumCoordinate(points[i][j], j);
		}
		newCluster->setNumPoints(1);
		clusterPoints[0][i] = newCluster->getClusterIdx();
		allClusters.push_back(newCluster);
		currentClusters.insert(i);
	}

	distsHeap = new BinaryHeap(dists, numPoints);
}

void HierarchicalClustering::constructElements()
{
	points = new double* [numPoints];
	for(int i = 0; i < numPoints; i++)
		points[i] = new double[numDimensions];

	dists = new double* [numPoints];
	for(int i = 0; i < numPoints; i++)
	{
		dists[i] = new double[numPoints];
		for(int j = 0; j < numPoints; j++)
			dists[i][j] = INFINITY;
	}

	clusterPoints = new int* [numPoints];
	for(int i = 0; i < numPoints; i++)
	{
		clusterPoints[i] = new int[numPoints];
	}

	correspondingClusers = new int[numPoints];
	for(int i = 0; i < numPoints; i++)
		correspondingClusers[i] = i;
}

HierarchicalClustering::~HierarchicalClustering()
{
	delete distsHeap;
	for(int i = 0; i < numPoints; i++)
	{
		delete [] points[i];
		delete [] dists[i];
		delete [] clusterPoints[i];
	}

	delete [] points;
	delete [] dists;
	delete [] clusterPoints;

	for(size_t i =  0; i < allClusters.size(); i++)
		delete allClusters[i];

	delete [] correspondingClusers;
}

void HierarchicalClustering::calculateHierarchy(Distance* distance)
{
	for(int step = 1; step < numPoints; step++)
	{
		int fromIdx = correspondingClusers[distsHeap->minDistX()];
		int toIdx = correspondingClusers[distsHeap->minDistY()];
		double minDist = distsHeap->minDist();

		Cluster* fromCluster = allClusters[fromIdx];
		Cluster* toCluster = allClusters[toIdx];

		int minPointIdx = min(fromCluster->getMinimalPointIndex(), toCluster->getMinimalPointIndex());
		Cluster* newCluster = new Cluster(numDimensions, minPointIdx, minDist);

		vector<int> mergedPoints[2];
		for(int i = 0; i < numPoints; i++)
		{
			if(clusterPoints[step - 1][i] == fromCluster->getClusterIdx())
			{
				clusterPoints[step][i] = newCluster->getClusterIdx();
				mergedPoints[0].push_back(i);
			} else if(clusterPoints[step - 1][i] == toCluster->getClusterIdx())
			{
				clusterPoints[step][i] = newCluster->getClusterIdx();
				mergedPoints[1].push_back(i);
			} 
			else
			{
				clusterPoints[step][i] = clusterPoints[step - 1][i];
			}
		}

		for(size_t i = 0; i < mergedPoints[0].size(); i++)
		{
			for(size_t j = 0; j < mergedPoints[1].size(); j++)
			{
				if(mergedPoints[0][i] < mergedPoints[1][j])
					distsHeap->removeElement(mergedPoints[0][i], mergedPoints[1][j]);
				else
					distsHeap->removeElement(mergedPoints[1][j], mergedPoints[0][i]);
			}
		}

		currentClusters.erase(fromCluster->getClusterIdx());
		currentClusters.erase(toCluster->getClusterIdx());

		double newDist;
		int otherClusterPoint;
		for(set<int>::iterator itr = currentClusters.begin(); itr != currentClusters.end(); ++itr)
		{
			otherClusterPoint = allClusters[*itr]->getMinimalPointIndex();
			newDist = distance->mergedDistance(*fromCluster, *toCluster, *allClusters[*itr], *this);
			if(otherClusterPoint < newCluster->getMinimalPointIndex())
			{
				distsHeap->modifyKey(otherClusterPoint, newCluster->getMinimalPointIndex(), newDist);
			}
			else
			{
				distsHeap->modifyKey(newCluster->getMinimalPointIndex(), otherClusterPoint, newDist);
			}
		}
		
		double * sumOfCoordinates1 = fromCluster->getSumCoordinates();
		double * sumOfCoordinates2 = toCluster->getSumCoordinates();
		for(int i = 0; i < numDimensions; i++)
		{
			newCluster->setSumCoordinate(sumOfCoordinates1[i] + sumOfCoordinates2[i], i);
		}

		newCluster->setNumPoints(fromCluster->getNumPoints() + toCluster->getNumPoints());
		newCluster->setCluster1(fromCluster);
		newCluster->setCluster2(toCluster);
		allClusters.push_back(newCluster);

		currentClusters.insert(newCluster->getClusterIdx());

		for(size_t i = 0; i < mergedPoints[0].size(); i++)
			correspondingClusers[mergedPoints[0][i]] = newCluster->getClusterIdx();
		for(size_t i = 0; i < mergedPoints[1].size(); i++)
			correspondingClusers[mergedPoints[1][i]] = newCluster->getClusterIdx();

	}
}

double HierarchicalClustering::getTotalFMeasure(const vector<vector<int> >& classes) {
	double total_f_measure = 0.0;
	vector<int> actual_classes(this->numPoints, -1);
	for (size_t i = 0;i < classes.size(); ++i) {
		for (size_t j = 0;j < classes[i].size(); ++j){
			actual_classes[classes[i][j]] = i;
		}
	}

	for (size_t i = 0; i < classes.size(); ++i) {
		double class_f_measure = 0.0;
		class_f_measure = max(class_f_measure, getClassFMeasure(i, classes[i].size(), actual_classes));
		total_f_measure += (double(classes[i].size())*class_f_measure)/double(this->numPoints);
	}
	return total_f_measure;
}

double HierarchicalClustering::getClassFMeasure(int class_index, int class_size, const vector<int>& actual_classes) {
	double f_measure = 0.0;
	vector<int> num_objects(allClusters.size(), 0);
	for (size_t i = 0; i < this->allClusters.size(); ++i){
		const Cluster* cluster = this->allClusters[i];
		if (!cluster->combinesClusters()) {
			if (actual_classes[cluster->getMinimalPointIndex()] == class_index) {
				num_objects[cluster->getClusterIdx()] = 1;
			}
		} else {
			num_objects[cluster->getClusterIdx()] = num_objects[cluster->getCluster1()->getClusterIdx()] +
					num_objects[cluster->getCluster2()->getClusterIdx()];
		}
	}
	for (size_t i = 0; i < this->allClusters.size(); ++i) {
		double temp_precision = double(num_objects[i])/double(this->allClusters[i]->getNumPoints());
		double temp_recall = double(num_objects[i])/double(class_size);
		if (temp_precision < 1e-9 || temp_recall <  1e-9) {
			continue;
		}
		double temp_f_measure = (2.0*temp_precision*temp_recall)/ (temp_precision + temp_recall);
		f_measure = max(f_measure, temp_f_measure);
	}
	return f_measure;
}
