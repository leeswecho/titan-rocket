#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "common_classes.h"

#define MAX_VERTEXES	200000

class cModelLoader
{

public:
	int init();
	int exit();

	int load_obj_model(char* filename, double scale);

	vertex UniqueVertexes[MAX_VERTEXES];
	vertex FinalVertexes[MAX_VERTEXES];

	int iNumUniqueVertexes;
	int iNumUniqueTextureCoords;
	int iNumUniqueNormals;
	int iNumFinalVertexes;

private:


};