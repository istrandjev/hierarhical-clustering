

# Introduction #

No use of easily extensible code if one does not know how to make the extensions. This page will describe the details of the implementation and it's purpose is to make it easier to write extensions of the code. After that every extension will be discussed on its own.

# Prerequisite #

We strongly advice you to experiment a bit with the application as it is, before you delve into adding logic of your own. Refer to [experimenting with the algorithms](ExperimentingWithTheAlgorithms.md) page to see how you can do that.

# Details of the implementation #

This section will introduce you to the most major classes of the implementation.

## `HierarchicalClustering` ##
This class is the main class of the implementation of the algorithm. It "does all the magic" as one will say. Its purpose is to calculate the hierarchical tree of clusters using the chosen [distance metric](AglomerativeHierarchicalClustering#Metrics.md). Also as the results of this calculations are used by almost all other components of the system we allow direct access to them (public fields). Here is the semantics of the important fields of this class:

  * **_numPoints_** - integer that store the number of elements subject to clusterization.
  * **_allClusters_** - this is vector of all the generated clusters. It is guaranteed that with the increasing of the step number the indices of the newly created clusters are greater than those of all the previously created clusters. As expected the total number of formed clusters is `numPoints * 2 - 1` which is also the size of this vector.
  * **_clusterPoints_** - this is two dimensional array of integers. Its purpose is to store the cluster to which each point belongs in each step. Thus `clusterPoints[step1][idx1]` tells the index of **_allClusters_** to which the element with `idx1` belongs after step number `step1` has been executed. As the step indices are between 0 and `numPoints - 1` the dimensions of this array are `numPoints x numPoints`.

## `Cluster` ##
This class represents all the details needed to handle the different clusters in the system. An important note to make here is that we do not actually store the cluster points in every single cluster. We rather store just the index of the cluster in `HierarchicalClustering`'s **_allClusters_**. After that we are able to restore the cluster points, if needed, from `HirearchicalClustering`'s **_clusterPoints_**. The most important methods of this class are the following:
  * **_getClusterIdx_()** - returns the index of the cluster in the **_allClusters_** field of the `HirearchicalClustering` instance that created this cluster.
  * **_getNumPoints()_** - returns the number of points that this cluster comprises of.
  * **_getDistanceOfCreation()_** - returns the double 'distance of creation' for the cluster. Every cluster that contains more than one point is formed as result of merging two clusters that happened to be the closest with distance _D_ at certain step. The 'distance of creation' for every cluster of more than one point stores this _D_. For the rest the distance of creation is 0. This method is used for example when calculating the [cophenetic quality measure](AglomerativeHierarchicalClustering#Cophenetic_correlation.md)

## `Distance` ##
This class and its deriving ones define the logic implementing different [distance metrics](AglomerativeHierarchicalClustering#Metrics.md). Every distance class defines several methods:
  * **_initialDistance_** - this method defines the initial distance between the clusters of size 1 in step 0. However the base class provides default implementation setting this distance to the Euclidian distance between the points forming the two clusters. This default implementation should be enough for most cases
  * **_getName_** - this method defines unique identifier of the distance class for the system. This identifier will also be the one listed in the input form [distance metric combo box](ExperimentingWithTheAlgorithms#Distance_metric.md).
  * **_mergedDistance_** - this method calculates the new distance between newly formed cluster and other one. In order to make this method serve all possible usages it receives 4 parameters: the two clusters _leftFromCluster_ and _rightFromCluster_ that are merged to form the new cluster, _toCluster_ - the cluster to which we want to calculate the distance form the newly formed cluster and the ubiquitous `HierarchicalClustering` instance.

If you wish to understand the logic of this class it is probably best to refer to some already existing implementation.

# Extending the existing code #

In this section we will describe the ways in which you can extend the existing code.

## Adding a distance metric ##
Probably the place where experimenting with different approaches is most worthwhile is the implementation of distance metrics. The introduction of new distance metric in the system is fairly simple:
  1. You need to implement new class extending the base Distance and define the derived methods in the desired manner. The name is important - it should be unique for the system and, beware, it is the one that will be used in the input form [distance metric combo box](ExperimentingWithTheAlgorithms#Distance_metric.md).
  1. Add the header of your metric in _all\_distances.h_
  1. After that just modify _distance\_factory.cpp_ adding `distances.push_back(new YourDistance());` to its **_`InitDistances`_** method body, where _`YourDistance`_ is the class of your newly implemented metric.

That's it. Now your metric will appear in the input form and you will be able to conduct tests with it.