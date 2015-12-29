#include "cWindowManager.h"
//*******************************************************//
//*******************************************************//
int cWindowManager::init(HINSTANCE _hAppInstance)
{
	hAppInstance = _hAppInstance;
	bRunFlag = TRUE;

	for (int lc1 = 0; lc1 < MAX_WINDOWS; lc1++)
	{
		hWindows[lc1] = 0;
	}

	return 0;
}
//*******************************************************//
//*******************************************************//
int cWindowManager::create_window(int iWindow, LPWSTR ClassName, LPWSTR Name, WNDPROC _WndCallBackProc)
{
	wndclass1.lpszClassName = ClassName;
	wndclass1.hInstance = hAppInstance;
	wndclass1.hCursor = LoadCursor(hAppInstance, (LPCTSTR)IDC_ARROW);
	wndclass1.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass1.lpfnWndProc = _WndCallBackProc;
	wndclass1.lpszMenuName = NULL;
	wndclass1.style = CS_DBLCLKS;
	wndclass1.hIcon = LoadIcon(hAppInstance, (LPCTSTR)IDI_ICON1);
	wndclass1.cbClsExtra = 0;
	wndclass1.cbWndExtra = 0;

	RegisterClass(&wndclass1);

	hWindows[iWindow] = CreateWindow(ClassName,
						Name,
						WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
						0,
						0,
						640,
						480,
						NULL,
						NULL,
						hAppInstance,
						NULL);

	return 0;

}
//*******************************************************//
//*******************************************************//