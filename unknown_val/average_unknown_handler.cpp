#include "average_unknown_handler.h"
#include "unknown_values_handler.h"

#include <string>
#include <fstream>
#include <vector>
using namespace std;

const string AverageUnkownHandler::HANDLER_IDENTIFIER = "average_unkown_elements";

void AverageUnkownHandler::readValues(const string& fileName, double **&points, int& numPoints, int& numDimensions)const
{
	ifstream in(fileName.c_str());
	in >> numDimensions >> numPoints;

	points = new double* [numPoints];
	for(int i = 0; i < numPoints; i++)
		points[i] = new double[numDimensions];

	vector<vector<bool> > valueKnown(numPoints);
	vector<double> attributeSum(numDimensions, 0.0);
	vector<int> numKnownAttributeValues(numDimensions, 0);

	for(int i = 0; i < numPoints; i++)
	{
		valueKnown[i].resize(numDimensions);
		for(int j = 0; j < numDimensions; j++)
		{
			if(readValue(in, points[i][j]))
			{
				attributeSum[j] += points[i][j];
				numKnownAttributeValues[j]++;
				valueKnown[i][j] = true;
			}
			else
			{
				valueKnown[i][j] = false;
			}
		}
	}

	for(int i = 0; i < numDimensions; i++)
	{
		if(numKnownAttributeValues[i] > 0)
		{
			attributeSum[i] /= (double) numKnownAttributeValues[i];
		}
		else
		{
			attributeSum[i] = 0.0;
		}
	}
	

	for(int i = 0; i < numPoints; i++)
	{
		for(int j = 0; j < numDimensions; j++)
		{
			if(!valueKnown[i][j])
				points[i][j] = attributeSum[j];
		}
	}
}

string AverageUnkownHandler::getName()const
{
	return HANDLER_IDENTIFIER;
}
