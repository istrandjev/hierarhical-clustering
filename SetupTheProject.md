# Introduction #

This project is small, but requires some tools in order to be able to run on local environment. Regretfully, currently the code is platform dependent and run only under Windows. Also the project is currently developed as Visual Studio project, but we intend to prepare Makefile, so that it will be runnable via g++.


# Downloading the required tools #

In order to be able to run the project you require several tools.

## Subversion ##

You will require subversion in order to be able to checkout the code from this code. You can download command-line subversion from [here](http://www.sliksvn.com/en/download) or even use [Tortoise Svn](http://tortoisesvn.tigris.org/) which provides a bit more user friendly interface for manipulation from the GUI of the OS. Each of this two places should describe the way to use the corresponding tools well enough.

After you have downloaded the tool you have chosen download the code of the project:
```
svn checkout http://hierarhical-clustering.googlecode.com/svn/trunk/ hierarhical-clustering-read-only
```

## Java ##

You will need to have java in order to be able to run the interface of the project. You probably already have at least jre, but if you don't you can easily find the newest one in the net. Please make sure java.exe is in your system's PATH otherwise there is a risk the interface still won't be able to run.

## Visual Studio ##
Regretfully, the project is still dependent on Microsoft Visual Studio, though we intend to develop Makefile, which will allow for building with g++. MS VS is not freeware, but still there are versions as Express Edition which are offered for free. The express edition can be found for example [here](http://www.microsoft.com/visualstudio/en-us/products/2010-editions/visual-cpp-express).

## Add OpenGL libraries ##
TODO

## Run the project ##

Now you should have everything you need to run the project. Just double click the .sln file from the files checked from the repository. After you do that the project will open in Visual Studio. All you need to do now is to press ctrl + f5, which will build the project and run it for you.

You should now be good for experimenting.