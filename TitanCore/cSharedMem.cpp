#include "cSharedMem.h"
//*****************************************************************//
//*****************************************************************//
cSharedMem::cSharedMem()
{
	hSharedMem = NULL;
	hSharedMem = LoadLibraryA("Titan4DLL.dll");

	printf("hSharedMem: %x.\n", hSharedMem);

	if (hSharedMem == NULL)
	{
		printf("Allocation failure.\n");
		return;
	}

	read_int = (_read_int_type)GetProcAddress(hSharedMem, "_read_int");
	write_int = (_write_int_type)GetProcAddress(hSharedMem, "_write_int");
	read_double = (_read_double_type)GetProcAddress(hSharedMem, "_read_double");
	write_double = (_write_double_type)GetProcAddress(hSharedMem, "_write_double");

	//DEBUG
	printf("read_int: %x\n", read_int);

}
//*****************************************************************//
//*****************************************************************//
cSharedMem::~cSharedMem()
{
	FreeLibrary(hSharedMem);
}
//*****************************************************************//
//*****************************************************************//
