#include "utils.h"

HGLRC GlVisualizer::hRC = NULL;							// Permanent Rendering Context
HDC GlVisualizer::hDC = NULL;							// Private GDI Device Context
HWND GlVisualizer::hWnd = NULL;							// Holds Our Window Handle
HINSTANCE GlVisualizer::hInstance;							// Holds The Instance Of The Application
bool GlVisualizer::keys[256];	// Array Used For The Keyboard Routine
bool GlVisualizer::active = true;
bool GlVisualizer::fullscreen = false;
int GlVisualizer::last_x = -1;
int GlVisualizer::last_y = -1;
int GlVisualizer::xrot = 0;
int GlVisualizer::yrot = 0;
bool GlVisualizer::rotated = false;
bool GlVisualizer::left_mouse = false;
bool GlVisualizer::right_mouse = false;
GLuint GlVisualizer::base = 0;

void delay(long double ms)
{
	long double start;
	start=get_time();
	while((get_time()-start)<(double)ms);
}