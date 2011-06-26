#define _CRT_SECURE_NO_DEPRECATE

#include "cluster_vis.h"
#include "cofenetic.h"
#include "configuration_manager.h"
#include "hierarchical_clustering.h"
#include "image_mover.h"
#include "random.h"
#include "shape_vis.h"
#include "utils.h"

#include <algorithm>
#include <cmath>

#define NAME_SIZE 10
#define REC_SIZE 6
using namespace std;
int score;
#define mpair make_pair
#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;
typedef long double ld;
const ld epsylon = 1e-9;

void doMain() {
	ConfigurationManager cm;
	cm.doConfiguration();
	const Distance* distance = cm.getDistance();
	HierarchicalClustering hc(cm.getInputFileName(), distance);
	hc.calculateHierarchy(distance);
	ClusterVis cluster_vis(&hc);
	ImageMover image_mover;
	{
		FILE *out = fopen("solution.out","w");
		fprintf(out,"%d\n", hc.numPoints);
		for (int i=0;i<hc.numPoints; ++i) {
			fprintf(out,"%d: ", i);
			for (int j=0;j<hc.numPoints;++j){
				fprintf(out,"(");
				for (int k=0;k<hc.numDimensions; ++k){
					fprintf(out, "%.1llf", hc.points[j][k]);
					if (k + 1 != hc.numDimensions) {
						fprintf(out, ", ");
					}
				}
				fprintf(out,")=%d", hc.clusterPoints[i][j]);
			}
			fprintf(out, "\n");
		}
		if(true)
		{
			fprintf(out, "Cofenetic metric: %.8lf\n", cppc(hc));
		}
		fclose(out);
	}
	if (false) {
		vector<vector<int> > actual;
		FILE *in = fopen("classes.txt", "r");
		int n;
		fscanf(in,"%d", &n);
		actual.resize(n);
		for (int i=0;i<n;++i){
			int m;
			fscanf(in,"%d", &m);
			actual[i].resize(m);
			for (int j=0;j<m;++j){
				fscanf(in, "%d", &actual[i][j]);
			}
		}
		fclose(in);
		FILE *out = fopen("compare.out","a");
		fprintf(out, "%.7lf\n", hc.getTotalFMeasure(actual));
	}
	int tp = 0;
	while (true) {
		GlVisualizer::clear_output();
		image_mover.DoMove();
		cluster_vis.Visualize(tp);
		GlVisualizer::animation_pause(true);
		if (GlVisualizer::keys['Z']) {
			cluster_vis.IncrementClusterLevel();
			GlVisualizer::keys['Z']	= false;
		}
		if (GlVisualizer::keys['X']) {
			cluster_vis.DecrementClusterLevel();
			GlVisualizer::keys['X']	= false;
		}
		if (GlVisualizer::keys['V']) {
			cluster_vis.IncrementRadius();
		}
		if (GlVisualizer::keys['B']) {
			cluster_vis.DecrementRadius();
		}

		if (GlVisualizer::keys['N']) {
			tp = (tp+1)%8;
		}
		if (GlVisualizer::keys['M']) {
			tp = (tp+7)%8;
		}
	}
}

void doMy() {
	ImageMover im;
	while(true) {
		GlVisualizer::clear_output();
		//glTranslatef(0.0f, 0.0f, -1.5f);
		im.DoMove();
		Shape s(0,0,0,0.15, 4*8*8*8 + 4);
		s.Display();
		GlVisualizer::animation_pause(true);
	}
}
int WINAPI WinMain(	HINSTANCE	hInstance,				// Instance
			HINSTANCE	hPrevInstance,				// Previous Instance
			LPSTR		lpCmdLine,				// Command Line Parameters
			int		nCmdShow)				// Window Show State
{
	freopen("error.out","w",stderr);
	// Create Our OpenGL Window
	GlVisualizer::Initialize();
	if (!GlVisualizer::CreateGLWindow(L"Hierarchical clustering",1280,800,16))
	{
		return 0;							// Quit If Window Was Not Created
	}
	WCHAR st[40];
	const ui number = 10;
	GetDlgItemText(NULL, 0,st, 40);
	//while(true){
	doMain();
	//}
	// Shutdown
	GlVisualizer::KillGLWindow();								// Kill The Window
	return 0;							// Exit The Program
}
