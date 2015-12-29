//Description: Titan2 is the visualizer component.
//It reads in the state vector from the integrator
//and displays it both in a 2D plot, and 3D display.

#define SHIP_SCALE 0.1
#define DEG_TO_RAD (3.14159/180.0)
#define RAD_TO_DEG (180.0/3.14159)


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "cWindowManager.h"
#include "cD3D.h"
#include "cModelLoader.h"
#include "common_classes.h"
#include "sm_defines.h"

HMODULE hSharedMem;

typedef int (__cdecl *_read_int_type) (int addr);
typedef double (__cdecl *_read_double_type) (int addr);
typedef char (__cdecl *_read8_type) (int addr);

typedef int (__cdecl *_write_int_type) (int addr, int value);
typedef int (__cdecl *_write_double_type) (int addr, double value);

_read_int_type read_int;
_read_double_type read_double;
_read8_type read8;
_write_int_type write_int;
_write_double_type write_double;

cModelLoader Loader;
cWindowManager WindowManager;
cD3D D3D;

int camera_target = 0;
int component_setup = 0;