//this library contains functions for accessing shared memory.
#include <Windows.h>
#include <stdio.h>
#include "../common/sm_defines.h"

_read_int_type _read_int;
_read_double_type _read_double;
_write_int_type _write_int;
_write_double_type _write_double;

int init_sm()
{
	HMODULE hSharedMem;

	hSharedMem = NULL;
	hSharedMem = LoadLibraryA("Titan4DLL.dll");

	printf("hSharedMem: %x.\n", hSharedMem);

	if (hSharedMem == NULL)
	{
		printf("Allocation failure.\n");
		return -1;
	}

	_read_int = (_read_int_type)GetProcAddress(hSharedMem, "_read_int");
	_write_int = (_write_int_type)GetProcAddress(hSharedMem, "_write_int");
	_read_double = (_read_double_type)GetProcAddress(hSharedMem, "_read_double");
	_write_double = (_write_double_type)GetProcAddress(hSharedMem, "_write_double");

	//done.
	return 0;
}

//route to DLL functions
int sm_read_int (int addr)
{
	return _read_int(addr);
}

//route to DLL function
double sm_read_double (int addr)
{
	return _read_double(addr);
}

//route to DLL function
int sm_write_int (int addr, int value)
{
	return _write_int(addr, value);
}

//route to DLL function
int sm_write_double (int addr, double value)
{
	return _write_double(addr, value);
}
