#include "cluster.h"

#include <iostream> 

int Cluster::staticClusterIdx = 0;

Cluster::Cluster(int numDimensions, int minimalPointIndex, double distanceOfCreation)
{
	sumCoordinates = new double[numDimensions];
	clusterIdx = staticClusterIdx++;
	this->distanceOfCreation = distanceOfCreation;
	this->minimalPointIndex = minimalPointIndex;
	cluster1 = NULL;
	cluster2 = NULL;
}

Cluster::~Cluster()
{
	delete sumCoordinates;
}

int Cluster::getNumPoints()const
{
	return numPoints;
}

void Cluster::setNumPoints(int numPoints)
{
	this->numPoints = numPoints;
}
	
double *Cluster::getSumCoordinates()const
{
	return sumCoordinates;
}

void Cluster::setSumCoordinate(double val, int dimension)
{
	sumCoordinates[dimension] = val;
}

int Cluster::getClusterIdx()const
{
	return clusterIdx;
}

double Cluster::getDistanceOfCreation()const
{
	return distanceOfCreation;
}

int Cluster::getMinimalPointIndex() const
{
	return minimalPointIndex;
}

bool Cluster::combinesClusters() const {
	return cluster1 != NULL && cluster2 != NULL;
}

void Cluster::setCluster1(const Cluster* cluster) {
	cluster1 = cluster;
}

void Cluster::setCluster2(const Cluster* cluster) {
	cluster2 = cluster;
}

const Cluster* Cluster::getCluster1() const {
	return cluster1;
}

const Cluster* Cluster::getCluster2() const {
	return cluster2;
}

bool Cluster::operator ==(const Cluster& rhs)const
{
	return clusterIdx == rhs.getClusterIdx();
}

bool Cluster::operator <(const Cluster& rhs)const
{
	return clusterIdx < rhs.getClusterIdx();
}
