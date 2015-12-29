#pragma once

#include <windows.h>
#include <stdio.h>
#include "sm_defines.h"

class cSharedMem
{
public:
	_read_int_type read_int;
	_read_double_type read_double;
	_write_int_type write_int;
	_write_double_type write_double;

	HMODULE hSharedMem;

	cSharedMem();

	~cSharedMem();

private:
};