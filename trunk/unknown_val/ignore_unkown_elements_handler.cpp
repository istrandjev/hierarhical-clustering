#include "ignore_unknown_elements_handler.h"
#include "unknown_values_handler.h"

#include <string>
#include <fstream>
#include <vector>
using namespace std;

const string IgnoreUnkownHandler::HANDLER_IDENTIFIER = "ignore_unkown_elements";

void IgnoreUnkownHandler::readValues(const string& fileName, double **&points, int& numPoints, int& numDimensions)const
{
	ifstream in(fileName.c_str());
	in >> numDimensions >> numPoints;

	vector<vector<double> > v;
	for(int i = 0; i < numPoints; i++)
	{
		vector<double> temp(numDimensions);
		bool elementKnown = true;
		for(int j = 0; j < numDimensions; j++)
		{
			if(!readValue(in, temp[j]))
			{
				elementKnown = false;
			}
		}
		if(elementKnown)
			v.push_back(temp);
	}

	numPoints = v.size();
	points = new double* [numPoints];
	for(int i = 0; i < numPoints; i++)
		points[i] = new double[numDimensions];

	for(int i = 0; i < numPoints; i++)
		for(int j = 0; j < numDimensions; j++)
			points[i][j] = v[i][j];
}

string IgnoreUnkownHandler::getName()const
{
	return HANDLER_IDENTIFIER;
}
