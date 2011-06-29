#define _CRT_SECURE_NO_DEPRECATE

#include "f_measure_calculator.h"
#include "visualization/cluster_vis.h"
#include "visualization/shape_vis.h"
#include "visualization/utils.h"
#include "visualization/image_mover.h"
#include "cophenetic.h"
#include "configuration_manager.h"
#include "hierarchical_clustering.h"



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

void intiailizeGraphics() {
	// Create Our OpenGL Window
	GlVisualizer::Initialize();
	if (!GlVisualizer::CreateGLWindow(L"Hierarchical clustering",1280,730,16))
	{
		exit(0);							// Quit If Window Was Not Created
	}
	WCHAR st[40];
	GetDlgItemText(NULL, 0,st, 40);
}

void doMain() {

	ConfigurationManager cm;
	if(!cm.doConfiguration()) {
		return;
	}

	intiailizeGraphics();

	const Distance* distance = cm.getDistance();
	HierarchicalClustering hc(cm.getInputFileName(), distance);
	hc.calculateHierarchy(distance);
	ClusterVis cluster_vis(&hc, cm.getRemapFileName());
	ImageMover image_mover;
	CopheneticMeasure copheneticMeasure;
	FMeasureCalculator fmc;
	if (cm.isSupervised()) {
		fmc.Init(&hc, cm.getClassifiedFileName());
	}
	int tp = 0;
	while (true) {
		GlVisualizer::clear_output();
		image_mover.DoMove();
		if (cm.isSupervised()) {
			cluster_vis.PrintStats(copheneticMeasure, &fmc);
		} else {
			cluster_vis.PrintStats(copheneticMeasure, NULL);
		}
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

	doMain();
	// Shutdown
	GlVisualizer::KillGLWindow();								// Kill The Window
	return 0;							// Exit The Program
}
