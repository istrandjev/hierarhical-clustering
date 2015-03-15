

# Introduction #

As the project is aimed at the implementation of aglomerative hierarchical clustering algorithms it is required to explain what stands behind this term. We will use this page to very briefly introduce you to this approach to the clustering task.


# Aglomerative Hierarchical Clustering #

The aglomerative hierarchical clustering is an approach to the clustering task. In it we start with every element of the set of interest represented in its own cluster. After that we perform a sequence of steps that gradually merge clusters.

The original version of the algorithm merges clusters until we reach the state in which only one containing all the elements is present. However, one can always implement some logic that stops the merging after certain condition is met.

Each step of the merging merges exactly two of the clusters present in the previous step. We choose the couple of clusters to merge based on certain metric of distance between the clusters: we always merge the couple of clusters for which the distance is minimal. If there is more than one such couple arbitrary one is chosen (Note: the way the arbitrary couple is chosen can affect the results of the algorithm in the general case).

Very important for this algorithm is that we are not interested only in the final result of the algorithm. As already mentioned, this is usually just one cluster. In fact we construct a tree of all the clusters after each step: we merge couple of clusters in a single new one. After that when it comes to performing operations with the constructed clustering we perform them on the whole tree, rather than just on the final element.

As the aglomerative hierarchical clustering algorithms store the whole cluster tree and its construction is not an easy operation (the best algorithms in performance perform in O(n^2 lg n) time, where n is the number of elements subject to clustering), this algorithm is usually combined with faster clustering algorithm that aim to to base aglomeration and reduce the number of subjects.

The most used metrics for distance between clusters are described in the following section.

# Metrics #

This section will describe the most common metrics used to compute the distance between clusters in the step of finding the two closest clusters in the implementation of the algorithm.

## Single link ##

The single link distance between two clusters C1 and C2 is computed as
```
min dist(A, b) { A in C1, B in C2}
```
Where dist(A, B) denotes Euclidian distance between two points.

In other words we consider the single link distance between the two clusters to be the distance between their two closest elements.

The formal definition given here seems to be applicable only for elements with all their attributes being continuous, but the notion of distance can also be extended for nominal attributes.

## Complete link ##
The complete link distance between two clusters C1 and C2 is computed as
```
max dist(A, b) { A in C1, B in C2}
```
Where dist(A, B) denotes Euclidian distance between two points.

In other words we consider the complete link distance between the two clusters to be the distance between their two furthest elements.

The formal definition given here seems to be applicable only for elements with all their attributes being continuous, but the notion of distance can also be extended for nominal attributes.

## Average distance ##
The average distance between two clusters C1 and C2 is in fact the average of the Euclidian distance:
```
(1 / (n1 * n2)) * sum(dist(A, b) { A in C1, B in C2})
```

Where dist(A, B) denotes Euclidian distance between two points, n1 is the number of elements in C1 and n2 is the number of elements in C2.

## Centroid distance ##
The centroid distance between two clusters is the Euclidian distance between the centroids of the two clusters.

# Measures of quality #

Very important in every heuristic algorithm (like the one at hand) is to determine the quality of its results. For the reason of measuring the quality of aglomerative hierarchical clustering several measures are defined. They will be described briefly in the following sections.

## Cophenetic correlation ##
The cophenetic correlation measures the correlation between the matrix of real Euclidian distances between every two elements of the subject set and the so-called cophenetic distances between them.

The cophenetic distance between two points in the case of aglomerative hierarchical clustering algorithm is defined as follows: the distance between the two clusters that were merged to put the two points in a single new cluster. As we continue the aglomeration until we get to single cluster it is guaranteed that such a step of merging exists for every couple of points. Note that the cophenetic distance depends on the [metric](AglomerativeHierarchicalClustering#Metrics.md) we have chosen to use.

Now as we have defined the cophenetic distance we can define the matrix of cophenetic distances of the aglomerative hierarchical clustering: this is a matrix whose element cp`[`i`]``[`j`]` equals the cophenetic distance between elements with indices i and j.

The matrix of distances is defined as follows: d`[`i`]``[`j`]` equals the Euclidian distance between elements with indices i and j.

Let cp\_avg is the average of all the cophenetic distances cp`[`i`]``[`j`]` for which j > i. Analogously let d\_avg is the average of all real distances d`[`i`]``[`j`]` for which j > i.

The cophenetic correlation of the aglomerative hierarchical clustering is defined as follows:

cppc = sum<sub>i<j</sub>((d`[`i`]``[`j`]` - d\_avg) `*` (cp`[`i`]``[`j`]` - cp\_avg)) / sqrt(sum<sub>i<j</sub>((d`[`i`]``[`j`]` - d\_avg)<sup>2</sup>) + sum<sub>i<j</sub>((cp`[`i`]``[`j`]` - cp\_avg)<sup>2</sup>))

It can be proven that the value of this metric is in absolute value not greater than 1. The closer the values are to 1 the better the algorithm quality is considered to be.

We have tried in our implementation to calculate the value of this quality measure on each step considering only the already linked couples of elements in the formula, but the results usefulness is controversial.

## F Measure ##
### Basics ###
F measure is a metric used to evaluate the quality of clustering when the actual class to each object belongs is known in advance. Therefore it is a supervised measure of clustering quality.
### Input ###
The input for this measure are the objects to be clustered themselves and
the actual class to which each object belongs.
### Estimating the F measure ###
Estimating the F measure for a hierarchical clustering consists in the following steps:
  1. for each cluster and each class estimate the F-measure of the class for this cluster
  1. for each class choose the maximum among it's F-measures and use it as the F measure of the class
This measure can also be used when only a part of the hierarchy has been built. In that case simply use only the constructed clusters.
### Estimate the F measure for a class ###
In order to estimate the F measure of a given class for a given cluster, one needs to use two other metrics the **recall** and the **precision** of the class for the given cluster.
  * recall(Class, Cluster) - the recall of a class for a given cluster is defined as the proportion of objects of the given class assigned to the given cluster. I.e. if all the objects in the given class are |Class| and the number of objects assigned to Cluster that in fact are from Class is |CC|, than recall(Class, Cluster) = |CC|/|Class|
  * precision(Class, Cluster) - the precision of a class for a given cluster is defined as the proportion of objects within Cluster that belong to Class. I.e. if we denote the number of objects in Cluster by |Cluster|  and the number of objects assigned to Cluster that in fact belong to Class by |CC| than  precision(Class, Cluster) = |CC|/|Cluster|
Having in mind the above formulas the F measure of Class for Cluster is defined as the harmonic mean of the recall and precision of Class for Cluster:
```
FMeasure(Class,Cluster)=2*recall(Class,Cluster)*precision(Class,Cluster)/(recall(Class,Cluster)+precision(Class,Cluster))
```