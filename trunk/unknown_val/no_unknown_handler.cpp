#include "no_unknown_handler.h"
#include "unknown_values_handler.h"

#include <string>
#include <fstream>
using namespace std;

const string NoUnkownHandler::HANDLER_IDENTIFIER = "no_unknown_values";

void NoUnkownHandler::readValues(const string& fileName, double **&points, int& numPoints, int& numDimensions)const
{
	ifstream in(fileName.c_str());
	in >> numDimensions >> numPoints;

	points = new double* [numPoints];
	for(int i = 0; i < numPoints; i++)
		points[i] = new double[numDimensions];

	for(int i = 0; i < numPoints; i++)
		for(int j = 0; j < numDimensions; j++)
			in >> points[i][j];
}

string NoUnkownHandler::getName()const
{
	return HANDLER_IDENTIFIER;
}
