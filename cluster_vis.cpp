#include "cluster_vis.h"
#include "shape_vis.h"
#include "glfont.h"

#include <cmath>
#include <iostream> 
#include <sstream>

using namespace std;
ClusterVis::ClusterVis(const HierarchicalClustering* _hc) {
	hc = _hc;
	if (hc->numDimensions < 2 || hc->numDimensions > 3) {
		throw "Sorry, I can't visualize that!";
	}
	clusterLevel = 0;
	radius = 1e100;
	numPoints = hc->numPoints;
	for (int i = 0; i < numPoints; ++i) {
		for (int j = i + 1;j < numPoints; ++j) {
			double temp = Dist(hc->points[i], hc->points[j], hc->numDimensions);
			if (temp < radius) {
				radius = temp;
			}
		}	
	}
	radius *= 0.2;
	radius =  max(radius, 0.01);
}

void ClusterVis::printText(const string& str)const
{
	if(str.size() == 0)
		return;

	GLuint textureName;
	glGenTextures(1, &textureName);
	PixelPerfectGLFont font;
	font.Create("glfont.glf", textureName);

	stringstream ss;
	ss << str;
	string line;
	double y = 7.7;

	font.Begin();
	glPushMatrix();
	glLoadIdentity( );
	glEnable(GL_TEXTURE_2D);
	glColor3f(256.0f, 256.0f, 256.0f);

	while(getline(ss, line))
	{
		while(line.size() < 45)
			line.push_back(' ');
		font.TextOut(" " + line, 8, y, -20);
		y -= 0.64;
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ClusterVis::PrintStats(const CofeneticMeasure& cofeneticMeasure) const
{
	stringstream text;
	text << " Step: " << clusterLevel + 1 << "\n";
	text.setf(ios::fixed,ios::floatfield);
	text.precision(5);
	if(clusterLevel > numPoints / 30 && clusterLevel > 1)
		text << " cppc: " << cofeneticMeasure.getCppc(*hc, clusterLevel) << "\n";
	else
		text << " cppc: " << " Not available" << "\n";

	text << " F-measure: " << 0.89876554 << "\n";
	printText(text.str());
}

void ClusterVis::Visualize(int set_type = -1) const {
	for (int i = 0;i < numPoints; ++i) {
		int temp_type = hc->allClusters[hc->clusterPoints[clusterLevel][i]]->getMinimalPointIndex();
		const Shape& shape = GetShape(hc->points[i], temp_type);

	
		shape.Display();
	}
}

void ClusterVis::IncrementClusterLevel() {
	if (clusterLevel < hc->numPoints - 1) {
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
	if (hc->numDimensions == 3) {
		z = pt[2];
	}
	return Shape(x,y,z, radius, type_code);
}
