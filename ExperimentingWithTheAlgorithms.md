# Introduction #

The code we have developed is intended to be very intuitive and easy to use especially from end-user point of view. However we provide this page to explain briefly how you should interpret the results and how to tune in the parameters of the algorithm as you wish them to be.


# Prerequisite #

Of course, before you start experimenting you need to set up the project. If you have not yet done so please refer to [Setup the project](SetupTheProject.md) page.

# Input form #

When you launch the project you are presented with user friendly form that should allow you to switch all the parameters of the algorithms to the values you desire. The following sections will describe the semantics of everyone of them.

## Data input files ##

The data input files provide the elements that are to be clustered. There are couple of data input files: one of them describes the "real" elements along with all their attributes and the other one is the so-called remapped input. In the remapped file the elements are expected to be represented with at most 3 continuous attributes. The remapped file data is used in the visualization - every element is represented with a point with coordinates taken from the remapped file. When you select the real data input file the remapped file is set by default to the same file to save double browsing in case of 2 or 3 dimensional input data.

Both files are text files and are in the same format:
```
<num_dimensions> <num_points>
num_points lines of num_dimensions doubles describing the next element
```

For example if you wish to use as an input a unit square in 2D the input will be like:
```
2 4
0.0 0.0
0.0 1.0
1.0 1.0
1.0 0.0
```

## Distance metric ##

In the input form there is a drop down from which you can choose the distance metric to use. We have implemented the ones described in [this page](AglomerativeHierarchicalClustering#Metrics.md), but we allow for implementation of user metrics. The semantics of this field is obvious.

## Classification available switch ##

If set this switch states that there is an additional information available for the input data presented, i.e. the classification of the input elements is known and the data is used for algorithm verification. This can be used by different algorithm like the [F-metric measure](AglomerativeHierarchicalClustering#F_Measure.md) so that they will provide additional data during the execution of the algorithm.

When you check the checkbox additional button will appear in which you should provide the file containing the known classification data. This is text file and its format is the following:
```
<number_of_classes>
for each class:
  <number_of_points>
  [line with space-separated indexes of the elements in the class]
```
The indexes of the elements should match the sequence of the elements given in the data input files. The indexes range from 0 to `n - 1`, where n is the number of elements subject to clusterization. Every element should be classified in exactly one class. Thus if you have two clusters of 4 points, an example classification data file is the following:
```
2
4
0 2 4 6
4
1 3 5 7
```

## Unknown values handler ##
There is a separate drop-down from which you can choose the way the unknown attribute values will be handled. One can always provide unknown value for some of the attributes of the elements in the input data files. An unknown value is represented with "?" rather than real number for the corresponding attribute. Example file containing 3 2D points, one of the attributes of which is unknown:

```
2 3
0.0 0.0
? 1.0
1.0 1.0
```

The algorithms to handle unknown values are described in detail [here](UnknownValuesHandling.md). However here we will list the implemented options along with all peculiarities of the implementation:
  * no\_unknown\_handler - This handler will not allow any unknown values in neither the real input data file, nor the remapped input file. If unknown value is provided exception is thrown.
  * ignore\_unknown\_handler - This handler ignores all elements containing unknown attributes. This, however, reduces the number of elements to process and visualize. One have to take care when using this handler - if the remapped data file and the real one have difference in the elements with unknown elements the remapping will result in unpredictable results.
  * average\_unknown\_handler - This handler uses the average of all known values of certain attribute as substitute of the unknown values. Thus all attributes become real numbers and the handling can continues as if there were no unknown values. If for certain attribute no value is known a default average of zero is assigned to it.

# Visualization controls #
After you have provided all the parameters needed for the algorithm to run you can press the button execute.

When this happens the actual visualization of the algorithm is shown on the screen. Every element is presented with a point in the 3D space with points of same cluster represented with space figures of same shape and color and points of different cluster represented with different figures whenever possible (5000 different figures are provided).

You can observe the state of the current step (the initial step is zeroth, i.e. the one in which no merging had happened) with various controls:
  * You can rotate the image with the mouse
  * You can move inwards (direction of z<sup>+</sup>) with 'q'
  * You can move outwards (direction of z<sup>-</sup>) with 'a'
  * You can rotate the image around x axis via 's' or 'w'
  * You can rotate the image around y axis via 'e' or 'd'
  * You can rotate the image around z axis via 'r' or 'f'
  * The arrows ←, →, ↓, ↑ execute the expected movements in x and y directions
  * You can enlarge the figures representing the points with 'v'
  * You can make smaller the figures representing the points with 'b'

Also:
  * You can enlarge the figures representing the points with 'v'
  * You can shrink the figures representing the points with 'b'
  * You can switch to the next step, if such exist, with 'z'
  * You can switch back to the previous step, if such exist, with 'x'
  * You can stop the visualization and thus terminate the program with 'esc'

In each step there is a text field in which the values of all applicable quality measures are shown, together with the step number.