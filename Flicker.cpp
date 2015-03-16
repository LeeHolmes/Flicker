// Flicker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Flicker.h"
#define MAX_LOADSTRING 100
#define IDT_TIMER1 1

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
BOOL m_PaintWhite = false;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FLICKER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FLICKER);

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
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_FLICKER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_FLICKER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
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
   hInst = hInstance; // Store instance handle in our global variable

	HMONITOR hmon = MonitorFromWindow(hWnd,
									MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	if (!GetMonitorInfo(hmon, &mi)) return NULL;
	hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_POPUP | WS_VISIBLE,
		mi.rcMonitor.left,
		mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top,
		hWnd, NULL, hInst, 0);

   if (!hWnd)
   {
      return FALSE;
   }

   double blinkRate = 1;

   if(__argc > 1)
   {
	   char* rate = __argv[1];
	   blinkRate = atof(rate);
   }
   
   SetTimer(hWnd, IDT_TIMER1, (unsigned int) (1000.0 / blinkRate / 2.0), (TIMERPROC) NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
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
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		HBRUSH paintBrush = NULL;

		if(m_PaintWhite)
			paintBrush = CreateSolidBrush(RGB(255, 255, 255));
		else
			paintBrush = CreateSolidBrush(RGB(0, 0, 0));			
        
		SelectObject(hdc, paintBrush);

		RECT windowRectangle;
		GetWindowRect(hWnd, &windowRectangle);
        Rectangle(hdc, 
			(int) windowRectangle.left, 
			(int) windowRectangle.top,
			(int) windowRectangle.right,
			(int) windowRectangle.bottom);
        DeleteObject(paintBrush);

		EndPaint(hWnd, &ps);
	} break;
	case WM_TIMER:
	{
		switch(wParam)
		{
		case IDT_TIMER1:
			m_PaintWhite = (! m_PaintWhite);
			InvalidateRect(hWnd, NULL, true);

			break;
		}
	}; break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}