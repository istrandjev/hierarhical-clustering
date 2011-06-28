#include "cluster_vis.h"
#include "cophenetic.h"
#include "f_measure_calculator.h"
#include "glfont.h"
#include "shape_vis.h"
#include "unknown_val/unknown_handler_factory.h"
#include "unknown_val/unknown_values_handler.h"
#include <cmath>
#include <iostream> 
#include <fstream>
#include <sstream>

using namespace std;
ClusterVis::ClusterVis(const HierarchicalClustering* _hc, const string& remappedFileName) {
	hc = _hc;
	loadPointsFromFile(remappedFileName);
	if (numDimensions < 2 || numDimensions > 3) {
		throw "Sorry, I can't visualize that!";
	}
	clusterLevel = 0;
	radius = 1e100;
	for (int i = 0; i < numPoints; ++i) {
		for (int j = i + 1;j < numPoints; ++j) {
			double temp = Dist(points[i], points[j], numDimensions);
			if (temp < radius) {
				radius = temp;
			}
		}	
	}
	radius *= 0.2;
	radius =  max(radius, 0.01);
}

ClusterVis::~ClusterVis()
{
	for(int i = 0; i < numPoints; i++)
		delete [] points[i];
	delete [] points;
}

void ClusterVis::loadPointsFromFile(const string& remappedFileName)
{
	const UnkownHandler* unknownHandler = UnknownHandlerFactory::getUnkownHandler();
	unknownHandler->readValues(remappedFileName, points, numPoints, numDimensions);
}

void ClusterVis::printText(const string& str)const
{
	if(str.size() == 0)
		return;

	GLuint textureName;
	glGenTextures(1, &textureName);
	PixelPerfectGLFont font;
	font.Create("visualization/glfont.glf", textureName);

	stringstream ss;
	ss << str;
	string line;
	float y = 7.7f;

	font.Begin();
	glPushMatrix();
	glLoadIdentity( );
	glEnable(GL_TEXTURE_2D);
	glColor3f(256.0f, 256.0f, 256.0f);

	while(getline(ss, line))
	{
		while(line.size() < 45) {
			line.push_back(' ');
		}
		font.TextOut(" " + line, 8, (float)y, -20);
		y -= 0.64f;
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ClusterVis::PrintStats(const CopheneticMeasure& copheneticMeasure, const FMeasureCalculator* fmc) const
{
	stringstream text;
	text << " Step: " << clusterLevel + 1 << "\n";
	text.setf(ios::fixed,ios::floatfield);
	text.precision(5);
	if(clusterLevel > numPoints / 30 && clusterLevel > 1) {
		double val = copheneticMeasure.getCppc(*hc, clusterLevel);
		if(val > -1.5)
			text << " cppc: " << val << "\n";
		else
			text << " cppc: " << " Not available" << "\n";
	} else {
		text << " cppc: " << " Not available" << "\n";
	}
	if (fmc != NULL) {
		text << " F-measure: " << fmc->getAverageFMeasure(clusterLevel) << "\n";
	}
	printText(text.str());
}

void ClusterVis::Visualize(int set_type = -1) const {
	for (int i = 0;i < numPoints; ++i) {
		int temp_type = hc->allClusters[hc->clusterPoints[clusterLevel][i]]->getMinimalPointIndex();
		const Shape& shape = GetShape(points[i], temp_type);

	
		shape.Display();
	}
}

void ClusterVis::IncrementClusterLevel() {
	if (clusterLevel < numPoints - 1) {
		++clusterLevel;
	}
}

void ClusterVis::DecrementClusterLevel() {
	if (clusterLevel > 0) {
		--clusterLevel;
	}
}

void ClusterVis::IncrementRadius() {
	radius *= 1.1;
}

void ClusterVis::DecrementRadius() {
	radius *= 0.9;
}

double ClusterVis::Dist(double* pt1, double* pt2, int nd) const {
	double res = 0.0;
	for (int i = 0;i < nd; ++i) {
		res += sqrt((pt1[i] - pt2[i])*(pt1[i] - pt2[i]));
	}
	return res;
}

Shape ClusterVis::GetShape(double* pt, int type_code) const { 
	double x = pt[0];
	double y = pt[1];
	double z = 0.0;
	if (numDimensions == 3) {
		z = pt[2];
	}
	return Shape(x,y,z, radius, type_code);
}
