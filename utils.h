#ifndef _GRAPHICAL_HELPER_H
#define _GRAPHICAL_HELPER_H
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library

#ifndef WIN32
#include <sys/time.h>
#else
#include <windows.h>
#include <time.h>
#endif

inline long double get_time(){    
#ifndef WIN32
	timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + tv.tv_usec/1000000.0 ;
#else
	return (long double)clock()/CLOCKS_PER_SEC;
#endif
};

void delay(long double ms);

class GlVisualizer {
public:
	static HGLRC hRC;							// Permanent Rendering Context
	static HDC  hDC;							// Private GDI Device Context
	static HWND  hWnd;							// Holds Our Window Handle
	static HINSTANCE hInstance;							// Holds The Instance Of The Application
	static bool keys[256];	// Array Used For The Keyboard Routine
	static bool active;
	static bool fullscreen;
	static int last_x;
	static int last_y;
	static int xrot;
	static int yrot;
	static bool rotated;
	static bool left_mouse;
	static bool right_mouse;
	static GLuint base;
	static void initLight() {
		const GLfloat val = 0.6f;
		const GLfloat val2 = 0.6f;
		GLfloat mat_ambient[] = { val, val, val, 1.0f };
		GLfloat mat_specular[] = { val2, val2, val2, 1.0f };
		GLfloat light_position[] = { 0.0f, 0.0f, 10.0f, 1.0f };
		GLfloat lm_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
	    
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
	}

	static void ReSizeGLScene(GLsizei width, GLsizei height)				// Resize And Initialize The GL Window
	{
		if (height==0)								// Prevent A Divide By Zero By
		{
			height=1;							// Making Height Equal One
		}

		glViewport(0, 0, width, height);					// Reset The Current Viewport
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();							// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
		glLoadIdentity();							// Reset The Modelview Matrix
	}

	static int InitGL()								// All Setup For OpenGL Goes Here
	{
		glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
		glShadeModel(GL_SMOOTH);						// Enables Smooth Shading
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);					
		glClearDepth(1.0f);							// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations
		initLight();
		BuildFont();
		return 1;								// Initialization Went OK
	}

	static bool Pause()
	{
		MSG msg;
		while(1)
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
			{
				if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
				{
					return true;
				}
				else
				{
					TranslateMessage(&msg);				// Translate The Message
					DispatchMessage(&msg);
				}
				if(msg.message==WM_KEYDOWN)
					break;
			}
		}
		return false;
	}
	static void KillGLWindow()							// Properly Kill The Window
	{
		if (fullscreen)								// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(true);						// Show Mouse Pointer
		}
		if (hRC)								// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
					MessageBox(NULL,L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			if (!wglDeleteContext(hRC))					// Are We Able To Delete The RC?
			{
					MessageBox(NULL,L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			hRC=NULL;							// Set RC To NULL
		}
		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,L"Release Device Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hDC=NULL;							// Set DC To NULL
		}
		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,L"Could Not Release hWnd.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hWnd=NULL;							// Set hWnd To NULL
		}
		if (!UnregisterClass(L"OpenGL",hInstance))				// Are We Able To Unregister Class
		{
			MessageBox(NULL,L"Could Not Unregister Class.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hInstance=NULL;							// Set hInstance To NULL
		}
		KillFont();
	}
	static void Initialize() {
		hRC = NULL;
		hDC = NULL;
		hWnd = NULL;
		active = true;
		fullscreen = false;
		memset(keys, 0,sizeof(keys));
	}
	static BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits)
	{
		GLuint		PixelFormat;						// Holds The Results After Searching For A Match
		WNDCLASS	wc;							// Windows Class Structure
		DWORD		dwExStyle;						// Window Extended Style
		DWORD		dwStyle;						// Window Style
		RECT WindowRect;							// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;						// Set Left Value To 0
		WindowRect.right=(long)width;						// Set Right Value To Requested Width
		WindowRect.top=(long)0;							// Set Top Value To 0
		WindowRect.bottom=(long)height;						// Set Bottom Value To Requested Height
		hInstance		= GetModuleHandle(NULL);			// Grab An Instance For Our Window
		wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
		wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
		wc.cbClsExtra		= 0;						// No Extra Window Data
		wc.cbWndExtra		= 0;						// No Extra Window Data
		wc.hInstance		= hInstance;					// Set The Instance
		wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;						// No Background Required For GL
		wc.lpszMenuName		= NULL;						// We Don't Want A Menu
		wc.lpszClassName	= L"OpenGL";					// Set The Class Name
		if (!RegisterClass(&wc))						// Attempt To Register The Window Class
		{
			MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Exit And Return FALSE
		}
		if (fullscreen)								// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;					// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;			// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;			// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bits;				// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
				if (MessageBox(NULL,L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",L"NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
						fullscreen=FALSE;				// Select Windowed Mode (Fullscreen=FALSE)
				}
				else
				{
						// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL,L"Program Will Now Close.",L"ERROR",MB_OK|MB_ICONSTOP);
					return FALSE;					// Exit And Return FALSE
				}
			}
		}
		if (fullscreen)								// Are We Still In Fullscreen Mode?
		{
			dwExStyle=WS_EX_APPWINDOW;					// Window Extended Style
			dwStyle=WS_POPUP;						// Windows Style
			ShowCursor(FALSE);						// Hide Mouse Pointer
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
		}
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
		if (!(hWnd=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
						L"OpenGL",				// Class Name
						title,					// Window Title
						WS_CLIPSIBLINGS |			// Required Window Style
						WS_CLIPCHILDREN |			// Required Window Style
						dwStyle,				// Selected Window Style
						0, 0,					// Window Position
						WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
						WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
						NULL,					// No Parent Window
						NULL,					// No Menu
						hInstance,				// Instance
						NULL)))					// Don't Pass Anything To WM_CREATE
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Window Creation Error.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
			1,								// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,						// Must Support Double Buffering
			PFD_TYPE_RGBA,							// Request An RGBA Format
			bits,								// Select Our Color Depth
			0, 0, 0, 0, 0, 0,						// Color Bits Ignored
			0,								// No Alpha Buffer
			0,								// Shift Bit Ignored
			0,								// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored
			16,								// 16Bit Z-Buffer (Depth Buffer)
			0,								// No Stencil Buffer
			0,								// No Auxiliary Buffer
			PFD_MAIN_PLANE,							// Main Drawing Layer
			0,								// Reserved
			0, 0, 0								// Layer Masks Ignored
		};
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Can't Find A Suitable PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Can't Set The PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		if (!(hRC=wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Can't Create A GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		if(!wglMakeCurrent(hDC,hRC))						// Try To Activate The Rendering Context
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Can't Activate The GL Rendering Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
		ShowWindow(hWnd,SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);								// Sets Keyboard Focus To The Window
		ReSizeGLScene(width, height);						// Set Up Our Perspective GL Screen
		if (!InitGL())								// Initialize Our Newly Created GL Window
		{
			KillGLWindow();							// Reset The Display
			MessageBox(NULL,L"Initialization Failed.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;							// Return FALSE
		}
			return TRUE;								// Success
	}

	static LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
					UINT	uMsg,					// Message For This Window
					WPARAM	wParam,					// Additional Message Information
					LPARAM	lParam)					// Additional Message Information
	{
		switch (uMsg)								// Check For Windows Messages
		{
			case WM_ACTIVATE:						// Watch For Window Activate Message
			{
				if (!HIWORD(wParam))					// Check Minimization State
				{
					active=TRUE;					// Program Is Active
				}
				else
				{
					active=FALSE;					// Program Is No Longer Active
				}

				return 0;						// Return To The Message Loop
			}
			case WM_SYSCOMMAND:						// Intercept System Commands
			{
				switch (wParam)						// Check System Calls
				{
					case SC_SCREENSAVE:				// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;					// Prevent From Happening
				}
				break;							// Exit
			}
			case WM_CLOSE:							// Did We Receive A Close Message?
			{
				PostQuitMessage(0);					// Send A Quit Message
				return 0;						// Jump Back
			}
			case WM_KEYDOWN:						// Is A Key Being Held Down?
			{
				keys[wParam] = TRUE;					// If So, Mark It As TRUE
				return 0;						// Jump Back
			}
			case WM_KEYUP:							// Has A Key Been Released?
			{
				keys[wParam] = FALSE;					// If So, Mark It As FALSE
				return 0;						// Jump Back
			}
			case WM_SIZE:							// Resize The OpenGL Window
			{
				ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width, HiWord=Height
				return 0;						// Jump Back
			}
			case WM_MOUSEMOVE:
			{

				// Retrieve mouse screen position
				int x=(short)LOWORD(lParam);
				int y=(short)HIWORD(lParam);
				if (last_x == -1){
					last_x = x;
					last_y =  y;
				} else {
					xrot = x - last_x;
					yrot = y - last_y;
					rotated = true;
					last_x = x;
					last_y = y;
				}
			}
		}
		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}


	static void BuildFont(GLvoid)					// Build Our Bitmap Font
	{
		HFONT	font;						// Windows Font ID
		HFONT	oldfont;					// Used For Good House Keeping

		base = glGenLists(96);					// Storage For 96 Characters ( NEW )
		font = CreateFont(	-24,
			0,
			0,
			0,
			FW_BOLD,
			FALSE,				// Italic
			FALSE,				// Underline
			FALSE,				// Strikeout
			ANSI_CHARSET,			// Character Set Identifier
			OUT_TT_PRECIS,			// Output Precision
			CLIP_DEFAULT_PRECIS,		// Clipping Precision
			ANTIALIASED_QUALITY,		// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
			L"Times New Roman");			// Font Name
		oldfont = (HFONT)SelectObject(hDC, font);		// Selects The Font We Want
		wglUseFontBitmaps(hDC, 32, 96, base);			// Builds 96 Characters Starting At Character 32
		SelectObject(hDC, oldfont);				// Selects The Font We Want
		DeleteObject(font);					// Delete The Font
	}

	static void KillFont(GLvoid)						// Delete The Font List
	{
 		glDeleteLists(base, 96);				// Delete All 96 Characters ( NEW )
	}

	static GLvoid glPrint(GLfloat x, GLfloat y, const char *fmt, ...)
	{
		if( glIsList( base ) == GL_FALSE ) {
			// Error Report?
			throw "shit";
			return;
		}
		
		glRasterPos2f( x, y );
		
		char text[256];
		va_list ap;
		
		if(fmt==NULL) return;
		
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
		
		glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32);
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}

	static void animation_pause(bool changed){
		if (changed){
			SwapBuffers(hDC);			// Swap Buffers (Double Buffering)
		}
		//memset(keys, 0, sizeof(keys));
		MSG	msg;								// Windows Message Structure
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
					KillGLWindow();
					exit((int)(msg.wParam));			// If So done=TRUE
			}
			else							// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		if (keys[VK_ESCAPE])				// Was ESC Pressed?
		{
			KillGLWindow();
			exit((int)(msg.wParam));	
		}
		else								// If There Are No Messages
		{
			if (active)						// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					KillGLWindow();
					exit((int)(msg.wParam));	
				}
				if (!changed){
					delay(0.01);
				} else {
					delay(0.01);
				}
			}
			if (keys[VK_F1])					// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;				// If So Make Key FALSE
				KillGLWindow();					// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow(L"Caliper algorithm",1280,800,16))
				{
					return;				// Quit If Window Was Not Created
				}
			}
		}
	}

	static void clear_output(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
		glLoadIdentity();
	}


/*void drawSphere(ld x, ld y, ld z, ld r) {
	glTranslatef((GLfloat)x,(GLfloat)y,(GLfloat)z);
	for (ld angle = 0.0; angle < pi*2.0; angle+=angle_step*0.1) {
		glRotatef((GLfloat)angle, 1.0f, 0.0f, 0.0f);
		drawCircle(r);
	}
	glLoadIdentity();
}*/
};

//void drawShape(ld x, ld y, ld z, ld r, int shape_type){
//	//drawSphere(x,y,z,r, 20,20);
//	glPushMatrix();
//	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
//	//auxSolidSphere(r);
//	switch(shape_type){
//		case 0: auxSolidTorus(r*0.2, r);break;
//		case 1:	auxSolidBox(r, r, r);break;
//		case 2: auxSolidSphere(r);break;
//		case 3: auxSolidIcosahedron(r);break;
//		case 4: auxSolidDodecahedron(r);break;
//		case 5: auxSolidOctahedron(r);break;
//		case 6: auxSolidTetrahedron(r);break;
//		case 7: auxSolidCone(r,r*1.5);break;
//	}
//	glPopMatrix();
//}
#endif  // _GRAPHICAL_HELPER_H