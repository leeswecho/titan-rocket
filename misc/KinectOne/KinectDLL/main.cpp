#include <windows.h>
#include <MMSystem.h>

//currently the powerpoint is relying on this function to retrieve the 
//current gesture state.
//I have currently stubbed this out with calls to the keyboard instead,
//such that pushing H results in a "prev" code (1)
//and pushing J results in a "next" code (2)
//for real Kinect, replace with appropriate gesture determination.
__declspec(dllexport) int __stdcall GetActiveGesture()
{
	if (GetAsyncKeyState((char)'H') != 0)
	{
		return 1;
	}
	if (GetAsyncKeyState((char)'J') != 0)
	{
		return 2;
	}
	return 0;

}


int WINAPI DllMain()
{


	return TRUE;
}