#include "hierarchical_clustering.h"
#include "distances/distance.h"
#include "unknown_val/unknown_handler_factory.h"
#include "unknown_val/unknown_values_handler.h"
#include "cluster.h"
#include "constants.h"

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

HierarchicalClustering::HierarchicalClustering(const string& dataFileName, const Distance* distance)
{
	const UnkownHandler* unknownHandler = UnknownHandlerFactory::getUnkownHandler();
	unknownHandler->readValues(dataFileName, points, numPoints, numDimensions);

	constructElements();

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

void HierarchicalClustering::calculateHierarchy(const Distance* distance)
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

