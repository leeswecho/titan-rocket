#include <windows.h>
#define IDI_ICON1 101
#define MAX_WINDOWS	10

class cWindowManager
{
public:
	WNDCLASS wndclass1;
	HWND hWindows[MAX_WINDOWS];
	HINSTANCE hAppInstance;
	bool bRunFlag;

	int init(HINSTANCE _hAppInstance);
	int create_window(int iWindow, LPWSTR ClassName, LPWSTR Name, WNDPROC _WndCallBackProc);

private:



};