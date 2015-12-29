#include "cModelLoader.h"
//***********************************************//
//***********************************************//
int cModelLoader::init()
{
	iNumUniqueVertexes = 0;
	iNumFinalVertexes = 0;
	iNumUniqueTextureCoords = 0;
	iNumUniqueNormals = 0;

	return 0;
}
//***********************************************//
//***********************************************//
int cModelLoader::exit()
{
	iNumUniqueVertexes = 0;
	iNumFinalVertexes = 0;
	iNumUniqueTextureCoords = 0;
	iNumUniqueNormals = 0;
	return 0;
}
//***********************************************//
//***********************************************//
int cModelLoader::load_obj_model(char* filename, double scale)
{
	
	double COORD_SCALE = scale;

	iNumUniqueVertexes = 0;
	iNumUniqueTextureCoords = 0;
	iNumUniqueNormals = 0;
	iNumFinalVertexes = 0;

	FILE* InputFile;
	char Buffer[1000];
	bool bTri = false;

	int delims[14] = {1000, 1000, 1000, 1000, 1000, 1000, 1000,1000, 1000, 1000, 1000, 1000, 1000, 1000};
	char* substrings[15];

	for (int lc1 = 0; lc1 < 15; lc1++)
		substrings[lc1] = NULL;



	InputFile = fopen(filename, "r");

	if (InputFile == NULL)
		return -1;

	char* pret; 

	while (TRUE)
	{
		pret = fgets(Buffer, 1000, InputFile);	

		if (pret == NULL)
			break;

		if ((Buffer[0] == 'f') ||
			(Buffer[0] == 'v'))
		{
			substrings[0] = Buffer;
			//go through buffer
			int iNumDelimsFound = 0;
			int iOrigStrlen = strlen(Buffer);
			for (int lc1 = 0; lc1 < iOrigStrlen; lc1++)
			{	
				if (Buffer[lc1] < 32)
					Buffer[lc1] = 0;

				if ((Buffer[lc1] == ' ') || (Buffer[lc1] == '/'))
				{
					delims[iNumDelimsFound] = lc1;
					Buffer[lc1] = 0;
					iNumDelimsFound++;
					substrings[iNumDelimsFound] = Buffer + lc1 + 1;
				}
			}

			//now add
			if (Buffer[0] == 'v')
			if (Buffer[1] == 0)
			{

				//the x direction is reversed because when I did the model I had
				// positive x as going backwards and I don't want that here
				//note: as a result of this, I had to reverse the normals as well
				//to compensate for this
				UniqueVertexes[iNumUniqueVertexes].x = strtod(substrings[1], NULL);
				UniqueVertexes[iNumUniqueVertexes].y = strtod(substrings[2], NULL);
				UniqueVertexes[iNumUniqueVertexes].z = strtod(substrings[3], NULL);

				//scale coords
				UniqueVertexes[iNumUniqueVertexes].x *= COORD_SCALE;
				UniqueVertexes[iNumUniqueVertexes].y *= COORD_SCALE;
				UniqueVertexes[iNumUniqueVertexes].z *= COORD_SCALE;

				//TEMP: set color
				UniqueVertexes[iNumUniqueVertexes].colour = 0xffffffff;

				//	printf("VRT f %f %f %f \n", 
				//				UniqueVertexes[iNumUniqueVertexes].x,
				//				UniqueVertexes[iNumUniqueVertexes].y,
				//				UniqueVertexes[iNumUniqueVertexes].z);
				iNumUniqueVertexes++;
			}
			if (Buffer[0] == 'v')
			if (Buffer[1] == 't')
			{
				UniqueVertexes[iNumUniqueTextureCoords].tu = strtod(substrings[1], NULL);
				UniqueVertexes[iNumUniqueTextureCoords].tv = strtod(substrings[2], NULL);

				iNumUniqueTextureCoords++;
			}
			if (Buffer[0] == 'v')
			if (Buffer[1] == 'n')
			{
				UniqueVertexes[iNumUniqueNormals].nx = strtod(substrings[1], NULL);
				UniqueVertexes[iNumUniqueNormals].ny = strtod(substrings[2], NULL);
				UniqueVertexes[iNumUniqueNormals].nz = strtod(substrings[3], NULL);
				iNumUniqueNormals++;
			}
			if (Buffer[0] == 'f')
			{
				int iNumFields = 3;

				for (int lc5 = 4; lc5 < 13; lc5++)
				{
					if (substrings[lc5] != NULL)
					if (substrings[lc5][0] >= 32)
						iNumFields = lc5;
				}

				//plain triangle
				if (iNumFields == 3)
				{
					int v_index1 = atoi(substrings[1]) - 1;
					int v_index2 = atoi(substrings[2]) - 1;
					int v_index3 = atoi(substrings[3]) - 1;

					vertex normal = compute_normal(UniqueVertexes[v_index1], UniqueVertexes[v_index2], UniqueVertexes[v_index3]);

					FinalVertexes[iNumFinalVertexes] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes], &normal);
					FinalVertexes[iNumFinalVertexes+1] = UniqueVertexes[v_index2];
					copy_normal(&FinalVertexes[iNumFinalVertexes+1], &normal);
					FinalVertexes[iNumFinalVertexes+2] = UniqueVertexes[v_index3];
					copy_normal(&FinalVertexes[iNumFinalVertexes+2], &normal);
					iNumFinalVertexes += 3;

				}	
				//plain quad
				if (iNumFields == 4)
				{
					int v_index1 = atoi(substrings[1]) - 1;
					int v_index2 = atoi(substrings[2]) - 1;
					int v_index3 = atoi(substrings[3]) - 1;
					int v_index4 = atoi(substrings[4]) - 1;

					vertex normal1 = compute_normal(UniqueVertexes[v_index1], UniqueVertexes[v_index2], UniqueVertexes[v_index3]);

					FinalVertexes[iNumFinalVertexes] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes], &normal1);
					FinalVertexes[iNumFinalVertexes+1] = UniqueVertexes[v_index2];
					copy_normal(&FinalVertexes[iNumFinalVertexes+1], &normal1);
					FinalVertexes[iNumFinalVertexes+2] = UniqueVertexes[v_index3];
					copy_normal(&FinalVertexes[iNumFinalVertexes+2], &normal1);

					vertex normal2 = compute_normal(UniqueVertexes[v_index3], UniqueVertexes[v_index4], UniqueVertexes[v_index1]);

					FinalVertexes[iNumFinalVertexes+3] = UniqueVertexes[v_index3];
					copy_normal(&FinalVertexes[iNumFinalVertexes+3], &normal1);
					FinalVertexes[iNumFinalVertexes+4] = UniqueVertexes[v_index4];
					copy_normal(&FinalVertexes[iNumFinalVertexes+4], &normal1);
					FinalVertexes[iNumFinalVertexes+5] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes+5], &normal1);

					iNumFinalVertexes += 6;
				}
				//textured triangle
				if (iNumFields == 6)
				{
					int v_index1 = atoi(substrings[2]) - 1;
					int v_index2 = atoi(substrings[1]) - 1;
					int v_index3 = atoi(substrings[4]) - 1;
					int v_index4 = atoi(substrings[3]) - 1;
					int v_index5 = atoi(substrings[6]) - 1;
					int v_index6 = atoi(substrings[5]) - 1;

					vertex normal = compute_normal(UniqueVertexes[v_index1], UniqueVertexes[v_index3], UniqueVertexes[v_index5]);

					FinalVertexes[iNumFinalVertexes] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes], &normal);
					FinalVertexes[iNumFinalVertexes+1] = UniqueVertexes[v_index3];
					copy_normal(&FinalVertexes[iNumFinalVertexes+1], &normal);
					FinalVertexes[iNumFinalVertexes+2] = UniqueVertexes[v_index5];
					copy_normal(&FinalVertexes[iNumFinalVertexes+2], &normal);

					FinalVertexes[iNumFinalVertexes].tu = UniqueVertexes[v_index2].tu;
					FinalVertexes[iNumFinalVertexes].tv = UniqueVertexes[v_index2].tv;

					FinalVertexes[iNumFinalVertexes+1].tu = UniqueVertexes[v_index4].tu;
					FinalVertexes[iNumFinalVertexes+1].tv = UniqueVertexes[v_index4].tv;

					FinalVertexes[iNumFinalVertexes+2].tu = UniqueVertexes[v_index6].tu;
					FinalVertexes[iNumFinalVertexes+2].tv = UniqueVertexes[v_index6].tv;

					iNumFinalVertexes += 3;
				}	
				//textured quad
				if (iNumFields == 8)
				{
					int v_index1 = atoi(substrings[2]) - 1;
					int v_index2 = atoi(substrings[1]) - 1;
					int v_index3 = atoi(substrings[4]) - 1;
					int v_index4 = atoi(substrings[3]) - 1;
					int v_index5 = atoi(substrings[6]) - 1;
					int v_index6 = atoi(substrings[5]) - 1;
					int v_index7 = atoi(substrings[8]) - 1;
					int v_index8 = atoi(substrings[7]) - 1;

					vertex normal1 = compute_normal(UniqueVertexes[v_index1], UniqueVertexes[v_index3], UniqueVertexes[v_index5]);

					FinalVertexes[iNumFinalVertexes] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes], &normal1);
					FinalVertexes[iNumFinalVertexes+1] = UniqueVertexes[v_index3];
					copy_normal(&FinalVertexes[iNumFinalVertexes+1], &normal1);
					FinalVertexes[iNumFinalVertexes+2] = UniqueVertexes[v_index5];
					copy_normal(&FinalVertexes[iNumFinalVertexes+2], &normal1);

					FinalVertexes[iNumFinalVertexes].tu = UniqueVertexes[v_index2].tu;
					FinalVertexes[iNumFinalVertexes].tv = UniqueVertexes[v_index2].tv;

					FinalVertexes[iNumFinalVertexes+1].tu = UniqueVertexes[v_index4].tu;
					FinalVertexes[iNumFinalVertexes+1].tv = UniqueVertexes[v_index4].tv;

					FinalVertexes[iNumFinalVertexes+2].tu = UniqueVertexes[v_index6].tu;
					FinalVertexes[iNumFinalVertexes+2].tv = UniqueVertexes[v_index6].tv;

					vertex normal2 = compute_normal(UniqueVertexes[v_index5], UniqueVertexes[v_index7], UniqueVertexes[v_index1]);

					FinalVertexes[iNumFinalVertexes+3] = UniqueVertexes[v_index5];
					copy_normal(&FinalVertexes[iNumFinalVertexes+3], &normal1);
					FinalVertexes[iNumFinalVertexes+4] = UniqueVertexes[v_index7];
					copy_normal(&FinalVertexes[iNumFinalVertexes+4], &normal1);
					FinalVertexes[iNumFinalVertexes+5] = UniqueVertexes[v_index1];
					copy_normal(&FinalVertexes[iNumFinalVertexes+5], &normal1);

					FinalVertexes[iNumFinalVertexes+3].tu = UniqueVertexes[v_index6].tu;
					FinalVertexes[iNumFinalVertexes+3].tv = UniqueVertexes[v_index6].tv;

					FinalVertexes[iNumFinalVertexes+4].tu = UniqueVertexes[v_index8].tu;
					FinalVertexes[iNumFinalVertexes+4].tv = UniqueVertexes[v_index8].tv;

					FinalVertexes[iNumFinalVertexes+5].tu = UniqueVertexes[v_index2].tu;
					FinalVertexes[iNumFinalVertexes+5].tv = UniqueVertexes[v_index2].tv;

					iNumFinalVertexes += 6;
				}
			}

		}
		
	}

	fclose(InputFile);


	return 0;
}
//***********************************************//
//***********************************************//





