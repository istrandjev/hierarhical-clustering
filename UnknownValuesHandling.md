# Introduction #
This page describes what are unknown attribute values and the different approaches to handle then in the context of data mining.
# Unknown values #
It is not  rare case an element attribute's value to be unavailable. This can happen either because it has never been measured for some reason, or because it has been last in the archives. This is why the clustering algorithms should be able to handle such cases and still produce reasonable clustering.

As there are several different approaches to the unknown value problem we will discuss them in the following sections.

## Skip unknown ##
The easiest to understand and most naive approach is to just skip all elements with unknown attribute values. Thus the remaining elements will be perfectly described and all well-known clustering algorithms can be applied.

## Use aggregate ##
Another approach is to use statistical data from the known values of the same attribute in the other elements and deduce the expected value in the unknown cases. Some classical approaches are to use the average for continuous values and the mode for nominal attributes.

## Delayed unknown calculation ##

The approaches described above make initial preprocessing of the given data and from then on the calculations go on like if there where no unknown values. Other approaches do not assign a single value to every unknown attribute, but rather deduce it case-based. Such approaches are for example the max and min approach. When it comes to calculating the distance between two elements with one of them containing unknown attribute value the coordinate distance along this attribute is considered equal to the maximal (or minimal respectively) known attribute distance between these two elements. However this approach is not very suitable for visualization (no single value is assigned unknown attributes). Also special care should be taken, when it comes to calculating centroids with this approach.