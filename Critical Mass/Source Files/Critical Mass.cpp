// Critical Mass.cpp : Defines the entry point for the application.
//

#include "../Header Files/stdafx.h"
#include "../Header Files/Critical Mass.h"
#include "../Header Files/Game.h"
#include "../Header Files/ObjectManager.h"
#include "../Header Files/ObjectFactory.h"
#include "../Header Files/Renderer.h"
#include "../Header Files/GL/wglew.h"
#include "../Header Files/DebugConsole.h"
#include "../Header Files/Input.h"

#pragma region Old Stuff
/*
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPTSTR    lpCmdLine,
int       nCmdShow)
{
UNREFERENCED_PARAMETER(hPrevInstance);
UNREFERENCED_PARAMETER(lpCmdLine);

// TODO: Place code here.
MSG msg;
HACCEL hAccelTable;

// Initialize global strings
LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
LoadString(hInstance, IDC_CRITICALMASS, szWindowClass, MAX_LOADSTRING);
MyRegisterClass(hInstance);

// Perform application initialization:
if (!InitInstance (hInstance, nCmdShow))
{
return FALSE;
}

hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRITICALMASS));

// Main message loop:
while (GetMessage(&msg, NULL, 0, 0))
{
if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
}

return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
WNDCLASSEX wcex;

wcex.cbSize = sizeof(WNDCLASSEX);

wcex.style			= CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc	= WndProc;
wcex.cbClsExtra		= 0;
wcex.cbWndExtra		= 0;
wcex.hInstance		= hInstance;
wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CRITICALMASS));
wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CRITICALMASS);
wcex.lpszClassName	= szWindowClass;
wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
HWND hWnd;

hInst = hInstance; // Store instance handle in our global variable

hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

if (!hWnd)
{
return FALSE;
}

ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);

return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int wmId, wmEvent;
PAINTSTRUCT ps;
HDC hdc;

switch (message)
{
case WM_COMMAND:
wmId    = LOWORD(wParam);
wmEvent = HIWORD(wParam);
// Parse the menu selections:
switch (wmId)
{
case IDM_ABOUT:
DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
break;
case IDM_EXIT:
DestroyWindow(hWnd);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
break;
case WM_PAINT:
hdc = BeginPaint(hWnd, &ps);
// TODO: Add any drawing code here...
EndPaint(hWnd, &ps);
break;
case WM_DESTROY:
PostQuitMessage(0);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
UNREFERENCED_PARAMETER(lParam);
switch (message)
{
case WM_INITDIALOG:
return (INT_PTR)TRUE;

case WM_COMMAND:
if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
{
EndDialog(hDlg, LOWORD(wParam));
return (INT_PTR)TRUE;
}
break;
}
return (INT_PTR)FALSE;
}
*/
#pragma endregion

CGame		game;
CInput		input;

// Globals
// These may be shared across modules
HINSTANCE	ghInstance = NULL;		// Application instance
HWND		ghMainWnd = NULL;		// Main Window (window manager)
DEVMODE		gDevMode;

HDC	  g_hDC = NULL;		// GDI Device context
HGLRC g_hRC = NULL;	    // OpenGL Rendering context

const wchar_t szGameName[] = L"Critical Mass";

////CJM
//HDC* GetHDC ( void) { return &g_hDC; }

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Program Entry Point
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	// Do any program wide Initialization here
	// Change display settings 


	// Create Main Window. 
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL; //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("OpenGLWin32Window"); // Should be unique
	wcex.hIconSm		= NULL;

	if(RegisterClassEx(&wcex) == 0)
		return -1;

	// Select window styles
	UINT uiStyle,uiStyleX;
	//uiStyle = WS_OVERLAPPEDWINDOW;
	uiStyle = WS_POPUPWINDOW | WS_CAPTION;  // (Temporarily?) make the window non-resizable.
	uiStyleX = NULL;

	// Create the main 3D window
	ghMainWnd = CreateWindowEx(uiStyleX, wcex.lpszClassName, szGameName, uiStyle,
		0, 0, game.GetScreenWidth(), game.GetScreenHeight(), NULL, NULL, hInstance, NULL);

	// Get DC and set up Pixel Format Descriptor
	HDC hDC = GetDC(ghMainWnd);

	//Dummy Descriptor... Old school OpenGL initialization
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),		// Size of struct
		1,									// Version of struct
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
		PFD_TYPE_RGBA,	            // Always put this
		32,				            // Always put this, size of the color buffer
		0,0,0,0,0,0,0,0,0,0,0,0,0,  // You have no control over
		16,					        // Depth of the Z buffer
		8,				            // Stencil buffer
		0,0,0,0,0,0 };	            // Either obsolete, or we don't care about

		// Set Dummy format so we can get access WGL extensions
		SetPixelFormat(hDC, 1, &pfd);
		g_hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, g_hRC);

		// Checck for errors
		if(!g_hRC  || !hDC)
			return -1;

		// This is needed to initialize the wglChoosePixelFormatARB
		// function.
		glewInit();

		// Now that we have checked extensions delete window and create new one with correct pixel format
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(g_hRC);
		ReleaseDC(ghMainWnd,hDC);

		if( game.GetFullScreen() == GL_TRUE )
		{
			gDevMode.dmPelsHeight = game.GetScreenHeight();
			gDevMode.dmPelsWidth = game.GetScreenWidth();
			gDevMode.dmSize = sizeof(DEVMODE);
			gDevMode.dmBitsPerPel = 32;
			gDevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if(ChangeDisplaySettings(&gDevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				MessageBox(NULL, TEXT("Cannot change to selected desktop resolution."),
					NULL, MB_OK | MB_ICONSTOP);
				return -1;
			}

			uiStyle = WS_POPUP;
			uiStyleX = WS_EX_APPWINDOW;

		}

		// Create New Window
		// Create the window again
		ghMainWnd = CreateWindowEx(	uiStyleX,     // Extended style
			wcex.lpszClassName, 
			szGameName, 
			uiStyle,// window stlye
			10,       // window position, x
			10,       // window position, y
			game.GetScreenWidth(),  // width
			game.GetScreenHeight(),   // height
			NULL,           // Parent window
			NULL,           // menu
			hInstance,    // instance
			NULL);          // pass this to WM_CREATE

		g_hDC = GetDC( ghMainWnd );

		int nPixCount = 0;
		int nPixelFormat = -1;

		// Specify the important attributes we care about
		int pixAttribs[] = { WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
			WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
			//WGL_ACCELERATION_ARB,   1, // must be HW accelerated
			WGL_COLOR_BITS_ARB,     24, // 8 bits of each R, G and B
			WGL_DEPTH_BITS_ARB,     8, // 16 bits of depth precision for window
			WGL_DOUBLE_BUFFER_ARB,	 GL_TRUE, // Double buffered context
			//WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on
			//WGL_SAMPLES_ARB,        game.GetSamples(), // # of samples for MSAA
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB, // pf should be RGBA type
			WGL_STENCIL_BITS_ARB, 8,
			0}; // NULL termination

		// Ask OpenGL to find the most relevant format matching our attribs
		// Only get one format back.
		wglChoosePixelFormatARB(g_hDC, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*)&nPixCount);

		if(nPixelFormat != -1)
		{
			// Check for MSAA
			int attrib[] = { WGL_SAMPLES_ARB };
			int nResults = 0;
			wglGetPixelFormatAttribivARB(g_hDC, nPixelFormat, 0, 1, attrib, &nResults);

			// Got a format, now set it as the current one
			SetPixelFormat( g_hDC, nPixelFormat, &pfd );

			GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,  3,
				WGL_CONTEXT_MINOR_VERSION_ARB,  0,
				0 };

			g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
			if (g_hRC == NULL)
			{
				MessageBoxA(NULL, "Could not create an OpenGL 3.0 context.", NULL, MB_OK);
				return -1;
			}

			wglMakeCurrent( g_hDC, g_hRC );
		}

		if (!ghMainWnd)
			return -1;

		////////////////////////////////////////////////////////////////////////////////
		// Initialize the game
		////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
		SetupDebugConsole();
#endif
		game.Initialize( hInstance, ghMainWnd, g_hDC);

		// Try to adjust the window so that its client area is 1024 x 768
		RECT clientArea;
		GetClientRect( ghMainWnd, &clientArea );
		clientArea.right = clientArea.left + 1024;
		clientArea.bottom = clientArea.top + 768;
		AdjustWindowRect( &clientArea, uiStyle, FALSE );

		// Resize the the main render target so that it fits in the client area.
		GetClientRect( ghMainWnd, &clientArea );
		int clientWidth = clientArea.right - clientArea.left;
		int clientHeight = clientArea.bottom - clientArea.top;
		game.ResizeWindow( clientWidth, clientHeight );

		// Make sure window manager stays hidden
		ShowWindow(ghMainWnd, SW_SHOW);
		UpdateWindow(ghMainWnd);

		//game.Initialize( hInstance, ghMainWnd, g_hDC );


		//input.Initialize(hInstance,ghMainWnd,

		//m_Input.Initialize( _hInstance, ghMainWnd, GetScreenWidth(), GetScreenHeight() );


		// Message Pump - Use the form that goes idle and waits for 
		// messages, not continually running.
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0)) 
		{
			if(!TranslateMessage(&msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Do any program wide shutdown here
		// Restore Display Settings
		if( game.GetFullScreen() == GL_TRUE )
			ChangeDisplaySettings(NULL, 0);

		// Return termination code
		return msg.wParam;
}
// Main window message procedure. This is the window manager for
// the application
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam)
{

	switch (message) 
	{
		// Window is created, the application is starting
	case WM_CREATE:
		{

		}
		break;

	case WM_PAINT:
		{
			if(g_hDC == NULL) {
				ValidateRect(hWnd, NULL);
				break;
			}

			// Input and update might want to get moved out of here, but it
			// works just fine for now.
			if(game.IsClosing())
			{
				PostMessage( hWnd, WM_DESTROY, 0, 0 );
				break;
			}
			game.Play();
			SwapBuffers(g_hDC);
			//if( !game.GetAnimated() )
			//	ValidateRect( hWnd, NULL );
		}
		break;

	case WM_SIZE:
		if(g_hDC != NULL)
			game.ResizeWindow( LOWORD(lParam), HIWORD(lParam) );
		break;

		// Window is destroyed, it's the end of the application
	case WM_DESTROY:
		// Cleanup
		if(g_hDC != NULL)
			game.Shutdown();

		wglMakeCurrent(g_hDC, NULL);
		wglDeleteContext(g_hRC);
		ReleaseDC(hWnd, g_hDC);
		PostQuitMessage(0);
		break;

		// Window is either full screen, or not visible
	case WM_ACTIVATE:
		{
			// Only handled when full screen mode
			if( game.GetFullScreen() )
			{
				WINDOWPLACEMENT wndPlacement;
				wndPlacement.length = sizeof(WINDOWPLACEMENT);
				wndPlacement.flags = WPF_RESTORETOMAXIMIZED;
				wndPlacement.ptMaxPosition.x = 0;
				wndPlacement.ptMaxPosition.y = 0;
				wndPlacement.ptMinPosition.x = 0;
				wndPlacement.ptMinPosition.y = 0;
				wndPlacement.rcNormalPosition.bottom = gDevMode.dmPelsHeight;
				wndPlacement.rcNormalPosition.left = 0;
				wndPlacement.rcNormalPosition.top = 0;
				wndPlacement.rcNormalPosition.right = gDevMode.dmPelsWidth;

				if(LOWORD(wParam) == WA_INACTIVE)
				{
					wndPlacement.showCmd = SW_SHOWMINNOACTIVE;
					SetWindowPlacement(hWnd, &wndPlacement);
					//ShowCursor(TRUE);
				}
				else
				{
					wndPlacement.showCmd = SW_RESTORE;
					SetWindowPlacement(hWnd, &wndPlacement);
					//ShowCursor(FALSE);
				}
			}
		}
		break;


	//	// Handle keyboard input
	//case WM_CHAR:
	//	// Close program on ESC key press
	//	if(wParam == 27)
	//		DestroyWindow(hWnd);
	//	break;

	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
