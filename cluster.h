#ifndef CLUSTER_HPP
#define CLUSTER_HPP

class Cluster
{
private:
	int clusterIdx;
	double *sumCoordinates;
	int numPoints;
	double distanceOfCreation;
	int minimalPointIndex;
	const Cluster* cluster1;
	const Cluster* cluster2;
	static int staticClusterIdx;
public:
	Cluster(int numDimensions, int minimalPointIndex, double distanceOfCreation = 0.0);
	~Cluster();
	int getNumPoints()const;
	void setNumPoints(int numPoints);
	double *getSumCoordinates()const;
	void setSumCoordinate(double val, int dimension);
	int getClusterIdx()const;
	int getMinimalPointIndex()const;
	double getDistanceOfCreation()const;
	
	bool combinesClusters() const;

	void setCluster1(const Cluster* cluster);
	void setCluster2(const Cluster* cluster);
	const Cluster* getCluster1() const;
	const Cluster* getCluster2() const;

	bool operator ==(const Cluster& rhs)const;
	bool operator <(const Cluster& rhs)const;
};

#endif //CLUSTER_HPP