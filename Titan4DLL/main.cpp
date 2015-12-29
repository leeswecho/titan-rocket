//Description: Titan4 is the shared memory implementation
//that allows all of the other components to communicate with
//other. It uses the ability of a DLL to define a memory segment
//that is common to all instances of that DLL, to allow those who
//link to the DLL to access shared memory. 
#include <windows.h>

#pragma data_seg(".myseg")
	char Buffer[0x100000] = ""; //"all shared variables must be statically initialized" <-- very important!
#pragma data_seg() 

__declspec(dllexport) char __cdecl _read8(int addr)
{
	if (addr < 0)
		return 0;
	if (addr >= 0x100000)
		return 0;

	return Buffer[addr];
}

__declspec(dllexport) char __cdecl _write8(int addr, char data)
{
	if (addr < 0)
		return -1;
	if (addr >= 0x100000)
		return -1;

	Buffer[addr] = data;

	return 0;
}

__declspec(dllexport) int __cdecl _write_int(int addr, int value)
{
	int* pi = (int*)(Buffer + addr);
	
	*pi = value;

	return 0;
}

__declspec(dllexport) int __cdecl _read_int(int addr)
{
	int* pi = (int*)(Buffer + addr);
	
	return *pi ;

}

__declspec(dllexport)int  __cdecl _write_double(int addr, double value)
{
	double* pf = (double*)(Buffer + addr);
	
	*pf = value;

	return 0;
}

__declspec(dllexport) double __cdecl _read_double(int addr)
{
	double* pf = (double*)(Buffer + addr);
	
	return *pf;

}

int WINAPI DllMain()
{


	return TRUE;
}