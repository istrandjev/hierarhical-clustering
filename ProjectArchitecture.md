# Introduction #

This page describe the high level architecture of the project. The project has several main components. Every one of them will be described in the following section.


# Project Components #
## Settings configuration ##
A separate component written in java is used to perform the initial settings configuration of the project.

## Clustering algorithm ##
A component written in c++ that implements the actual clustering. It reads the input from a file and builds the clustering hierarchy according to the configuration. It can use several different [distance measures](AglomerativeHierarchicalClustering#Metrics.md) and [unknown value handlers](UnknownValuesHandling.md).

## Visualization ##
A component written in C++ using OpenGL. As meaningful visualization can be done for up to three dimensional space, the visualization component can be passed a mapping of the actual objects in up to three dimensions. This way only a projection of the actual space will be used to visualize the clustering.