//Description: Titan3 simply generates model files which are used by
//the visualizer (Titan2). The files are in Wavefront .obj format, 
//and old, common, and simple format. A file is generated for each
//section of the rocket that could potentially move independently.
#include <stdio.h>
#include <math.h>

FILE* outfile;

struct vertex{
   float x, y, z;
   float nx, ny, nz;
   unsigned long colour;
   float tu, tv;
};

class vec
{
	public:
	double x;
	double y;
	double z;

	vec()
	{
		x = 0;
		y = 0; 
		z = 0;
	}
	vec(double _x, double _y, double _z) 
	{
		x = _x;
		y = _y;
		z = _z;
	}

	vec operator+(vec A)
	{
		return vec(x+A.x, y+A.y,z+A.z);
    }

	vec operator-(vec A)
	{
		return vec(x-A.x, y-A.y,z-A.z);
    }

	vec operator*(double r)
	{
		return vec(x*r, y*r, z*r);
	}

	vec operator|(vec A)
	{
		return vec(y*A.z - z*A.y, z*A.x - x*A.z, x*A.y - y*A.x);
	}

	double mag()
	{
		return sqrt(x*x + y*y + z*z);
	}

	void normalize()
	{
		double magnitude = sqrt(x*x + y*y + z*z);

		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;

	}

	private:
};

int divlat;
int divlong;
double inclat;
double inclong;
double scale;
int iNumVertexes;

#define RESOLUTION 40
#define PI 3.14159265358

#define STAGE1_NOZZLE_SCALE 0.8382/93.5     //0.8382 meters per 93.5 pixels
#define ADDITIONAL_SCALE 0.5

vertex stage1_nozzle_vertices_inner[21][RESOLUTION];
vertex stage1_nozzle_vertices_outer[21][RESOLUTION];

vertex stage1_main_vertices[20][RESOLUTION];

vertex stage1_pipes_vertices[5][4][RESOLUTION];

vertex stage1_launch_mount_vertices[3][RESOLUTION];

vertex interstage_vertices_inner[2][RESOLUTION];
vertex interstage_vertices_outer[2][RESOLUTION];

vertex stage2_main_vertices[20][RESOLUTION];

double get_texture_coord_position(double longitude_deg)
{
	double long_rad = longitude_deg * (3.14159 / 180.0);
	double ratio = long_rad / 6.283135;

	//now this maps from 0 to 1, but we actually want to start at 0.5 to 1.5 and wrap around
	ratio += 0.5;

	//if (ratio > 1.1)
	//	ratio -= 1;

	return ratio;
}

int build_globe()
{

	divlat = 60;
	divlong = 60;
	inclat = 90.0 / divlat;
	inclong = 360.0 / divlong;
	scale = 1.0;

	iNumVertexes = divlat*divlong*8;
	vertex* vertices = new vertex[divlat*divlong*8 + 400];

	int ct = 0;
	double rlat1 = 0;
	double rlong1 = 0;
	double rlat2 = 0;
	double rlong2 = 0;

	int r3 = 0;
	int r2 = 1;
	int r1 = 2;
	int r0 = 3;

	printf("ct: %d rlat1: %f\n", ct, rlat1);

	for (double longitude = 0; longitude < 360; longitude += inclong )
	for (double latitude = 0; latitude < 90; latitude += inclat)
	{
		printf("ct: %d longitude %f latitude %f\n", ct, longitude, latitude);

		rlat1 = latitude * (3.14159 / 180.0);
		rlat2 = (latitude + inclat) * (3.14159 / 180.0);
		rlong1 = longitude * (3.14159 / 180.0);
		rlong2 = (longitude + inclong) * (3.14159 / 180.0);
		
		if (rlat1 > 1.5707)
			rlat1 = 1.5707;
		if (rlat2 > 1.5707)
			rlat2 = 1.5707;

		//longitude = 0 --> x = 1, y = 0 (prime meridian)
		//longitude = 90 deg E --> x = 0, y = 1
		vertices[ct+3].x = scale*cos(rlong1)*cos(rlat1);
		vertices[ct+3].y = scale*sin(rlong1)*cos(rlat1); //RH to LH?
		vertices[ct+3].z = scale*sin(rlat1);
		vertices[ct+3].tu = get_texture_coord_position(longitude); //rlong1 / 6.283135;
		vertices[ct+3].tv = 1.0 - ((rlat1 / 3.141593) + 0.5);

		vertices[ct+2].x = scale*cos(rlong2)*cos(rlat1);
		vertices[ct+2].y = scale*sin(rlong2)*cos(rlat1);  //RH to LH?
		vertices[ct+2].z = scale*sin(rlat1);
		vertices[ct+2].tu = get_texture_coord_position(longitude + inclong); //rlong2 / 6.283135;
		vertices[ct+2].tv = 1.0 - ((rlat1 / 3.141593) + 0.5);

		vertices[ct+1].x = scale*cos(rlong2)*cos(rlat2);
		vertices[ct+1].y = scale*sin(rlong2)*cos(rlat2);  //RH to LH?
		vertices[ct+1].z = scale*sin(rlat2);
		vertices[ct+1].tu = get_texture_coord_position(longitude + inclong); // rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - ((rlat2 / 3.141593) + 0.5);

		vertices[ct+0].x = scale*cos(rlong1)*cos(rlat2);
		vertices[ct+0].y = scale*sin(rlong1)*cos(rlat2);  //RH to LH?
		vertices[ct+0].z = scale*sin(rlat2);
		vertices[ct+0].tu = get_texture_coord_position(longitude); // //rlong1 / 6.283135;
		vertices[ct+0].tv = 1.0 - ((rlat2 / 3.141593) + 0.5);

		ct += 4;

		vertices[ct+3].x = scale*cos(rlong1)*cos(rlat1);
		vertices[ct+3].y = scale*sin(rlong1)*cos(rlat1);  //RH to LH?
		vertices[ct+3].z = -1*scale*sin(rlat1);
		vertices[ct+3].tu = get_texture_coord_position(longitude); // rlong1 / 6.283135;
		vertices[ct+3].tv = 1.0 - (((rlat1 / 3.141593)*-1) + 0.5);

		vertices[ct+2].x = scale*cos(rlong1)*cos(rlat2);
		vertices[ct+2].y = scale*sin(rlong1)*cos(rlat2);  //RH to LH?
		vertices[ct+2].z = -1*scale*sin(rlat2);
		vertices[ct+2].tu = get_texture_coord_position(longitude); //rlong1 / 6.283135;
		vertices[ct+2].tv = 1.0 - (((rlat2 / 3.141593)*-1) + 0.5);

		vertices[ct+1].x = scale*cos(rlong2)*cos(rlat2);
		vertices[ct+1].y = scale*sin(rlong2)*cos(rlat2);  //RH to LH?
		vertices[ct+1].z = -1*scale*sin(rlat2);
		vertices[ct+1].tu = get_texture_coord_position(longitude + inclong); //rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - (((rlat2 / 3.141593)*-1) + 0.5);

		vertices[ct+0].x = scale*cos(rlong2)*cos(rlat1);
		vertices[ct+0].y = scale*sin(rlong2)*cos(rlat1);  //RH to LH?
		vertices[ct+0].z = -1*scale*sin(rlat1);
		vertices[ct+0].tu = get_texture_coord_position(longitude + inclong); //rlong2 / 6.283135;
		vertices[ct+0].tv = 1.0 - (((rlat1 / 3.141593)*-1) + 0.5);

		ct += 4;
	}
	outfile = fopen("globe.objx", "w");
	//write vertexes
	for (int lc1 = 0; lc1 < iNumVertexes; lc1++)
	{
		//to convert RH to LH, try reversing all the x's
		vertices[lc1].x *= -1;
		fprintf(outfile, "v %f %f %f\n", vertices[lc1].x, vertices[lc1].y, vertices[lc1].z);
	}

	//write texture coords
	for (int lc1 = 0; lc1 < iNumVertexes; lc1++)
	{
		fprintf(outfile, "vt %f %f\n", vertices[lc1].tu, vertices[lc1].tv);
	}

	//write faces
	for (int lc1 = 0; lc1 < iNumVertexes; lc1+=4)
	{
		fprintf(outfile, "f %d %d %d %d\n", lc1+1, lc1+2, lc1+3, lc1+4);
	}


	delete [] vertices;

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int stage1_main_vertex_ring(int index, double xcenter, double ycenter, double zcenter, double radius)
{
	double increment = (2*PI) / RESOLUTION;

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		stage1_main_vertices[index][lc1].x = xcenter;
		stage1_main_vertices[index][lc1].y = ycenter + radius*sin(increment*lc1);
		stage1_main_vertices[index][lc1].z = zcenter + radius*cos(increment*lc1);

		stage1_main_vertices[index][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_main_vertices[index][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_main_vertices[index][lc1].z *= STAGE1_NOZZLE_SCALE;

		stage1_main_vertices[index][lc1].tu = 0;
		stage1_main_vertices[index][lc1].tv = 0;
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int stage1_nozzle_vertex_ring(int index, double xcenter, double ycenter, double zcenter, double radius)
{
	double increment = (2*PI) / RESOLUTION;

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		stage1_nozzle_vertices_inner[index][lc1].x = xcenter;
		stage1_nozzle_vertices_inner[index][lc1].y = ycenter + radius*sin(increment*lc1);
		stage1_nozzle_vertices_inner[index][lc1].z = zcenter + radius*cos(increment*lc1);

		stage1_nozzle_vertices_inner[index][lc1].x *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;
		stage1_nozzle_vertices_inner[index][lc1].y *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;
		stage1_nozzle_vertices_inner[index][lc1].z *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;

		stage1_nozzle_vertices_inner[index][lc1].tu = 0;
		stage1_nozzle_vertices_inner[index][lc1].tv = 0;

		stage1_nozzle_vertices_outer[index][lc1].x = xcenter;
		stage1_nozzle_vertices_outer[index][lc1].y = ycenter + (radius*1.005)*sin(increment*lc1);
		stage1_nozzle_vertices_outer[index][lc1].z = zcenter + (radius*1.005)*cos(increment*lc1);

		stage1_nozzle_vertices_outer[index][lc1].x *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;
		stage1_nozzle_vertices_outer[index][lc1].y *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;
		stage1_nozzle_vertices_outer[index][lc1].z *= STAGE1_NOZZLE_SCALE * ADDITIONAL_SCALE;

		stage1_nozzle_vertices_outer[index][lc1].tu = 0;
		stage1_nozzle_vertices_outer[index][lc1].tv = 0;
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_nozzle_main_vertices()
{

	for (int lc1 = 0; lc1 < 21; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", stage1_nozzle_vertices_inner[lc1][lc2].x, stage1_nozzle_vertices_inner[lc1][lc2].y, stage1_nozzle_vertices_inner[lc1][lc2].z);
	}
	for (int lc1 = 0; lc1 < 21; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", stage1_nozzle_vertices_outer[lc1][lc2].x, stage1_nozzle_vertices_outer[lc1][lc2].y, stage1_nozzle_vertices_outer[lc1][lc2].z);
	}

	return 20*2*RESOLUTION;
}
//*********************************************************//
//*********************************************************//
int print_stage1_nozzle_main_faces(int start)
{
	int i1;
	int i2;
	int i3;
	int i4;

	int gap = 21*RESOLUTION;

	for (int lc1 = 0; lc1 < 20; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		i1 = lc1*RESOLUTION + lc2;
		i2 = lc1*RESOLUTION + lc2 + 1;
		i3 = (lc1+1)*RESOLUTION + lc2 + 1;
		i4 = (lc1+1)*RESOLUTION + lc2;
		if (lc2 == (RESOLUTION-1))
		{
			i2 -= RESOLUTION;
			i3 -= RESOLUTION;
		}
		fprintf(outfile, "f %d %d %d %d\n", i4+start+1, i3+start+1, i2+start+1, i1+start+1);
		fprintf(outfile, "f %d %d %d %d\n", i1+start+1+gap, i2+start+1+gap, i3+start+1+gap, i4+start+1+gap);
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_stage1_nozzle()
{
	stage1_nozzle_vertex_ring(0, 500-500, 0, 0, 0.1);
	stage1_nozzle_vertex_ring(1, 500-500, 0, 0, 55);
	stage1_nozzle_vertex_ring(2, 432-500, 0, 0, 55);
	stage1_nozzle_vertex_ring(3, 420-500, 0, 0, 53);
	stage1_nozzle_vertex_ring(4, 409-500, 0, 0, 49);
	stage1_nozzle_vertex_ring(5, 401-500, 0, 0, 45);
	stage1_nozzle_vertex_ring(6, 393-500, 0, 0, 41);
	stage1_nozzle_vertex_ring(7, 385-500, 0, 0, 37);
	stage1_nozzle_vertex_ring(8, 379-500, 0, 0, 34);
	stage1_nozzle_vertex_ring(9, 372-500, 0, 0, 32);
	stage1_nozzle_vertex_ring(10, 365-500, 0, 0, 33);
	stage1_nozzle_vertex_ring(11, 356-500, 0, 0, 38);
	stage1_nozzle_vertex_ring(12, 348-500, 0, 0, 42);
	stage1_nozzle_vertex_ring(13, 337-500, 0, 0, 48);
	stage1_nozzle_vertex_ring(14, 317-500, 0, 0, 57);
	stage1_nozzle_vertex_ring(15, 300-500, 0, 0, 64);
	stage1_nozzle_vertex_ring(16, 276-500, 0, 0, 73);
	stage1_nozzle_vertex_ring(17, 249-500, 0, 0, 81);
	stage1_nozzle_vertex_ring(18, 218-500, 0, 0, 88);
	stage1_nozzle_vertex_ring(19, 190-500, 0, 0, 92);
	stage1_nozzle_vertex_ring(20, 175-500, 0, 0, 93);


	outfile = fopen("stage1_nozzle.objx", "w");

		print_stage1_nozzle_main_vertices();



		print_stage1_nozzle_main_faces(0);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_stage2_nozzle()
{

	stage1_nozzle_vertex_ring(0, 371-371, 0, 0, 0.1);
	stage1_nozzle_vertex_ring(1, 371-371, 0, 0, 274-256);
	stage1_nozzle_vertex_ring(2, 333-371, 0, 0, 274-256);
	stage1_nozzle_vertex_ring(3, 324-371, 0, 0, 270-256);
	stage1_nozzle_vertex_ring(4, 319-371, 0, 0, 267-256);
	stage1_nozzle_vertex_ring(5, 313-371, 0, 0, 267-256);
	stage1_nozzle_vertex_ring(6, 307-371, 0, 0, 270-256);
	stage1_nozzle_vertex_ring(7, 299-371, 0, 0, 276-256);
	stage1_nozzle_vertex_ring(8, 288-371, 0, 0, 281-256);
	stage1_nozzle_vertex_ring(9, 273-371, 0, 0, 288-256);
	stage1_nozzle_vertex_ring(10, 260-371, 0, 0, 293-256);
	stage1_nozzle_vertex_ring(11, 247-371, 0, 0, 297-256);
	stage1_nozzle_vertex_ring(12, 236-371, 0, 0, 301-256);
	stage1_nozzle_vertex_ring(13, 224-371, 0, 0, 304-256);
	stage1_nozzle_vertex_ring(14, 212-371, 0, 0, 307-256);
	stage1_nozzle_vertex_ring(15, 204-371, 0, 0, 309-256);
	stage1_nozzle_vertex_ring(16, 195-371, 0, 0, 311-256);
	stage1_nozzle_vertex_ring(17, 187-371, 0, 0, 313-256);
	stage1_nozzle_vertex_ring(18, 173-371, 0, 0, 316-256);
	stage1_nozzle_vertex_ring(19, 163-371, 0, 0, 318-256);
	stage1_nozzle_vertex_ring(20, 162.9-371, 0, 0, 318-256);

	outfile = fopen("stage2_nozzle.objx", "w");

		print_stage1_nozzle_main_vertices();
		print_stage1_nozzle_main_faces(0);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int stage1_launch_mount_vertex_ring(int index, double xcenter, double ycenter, double zcenter, double radius)
{
	double increment = (2*PI) / RESOLUTION;

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		stage1_launch_mount_vertices[index][lc1].x = xcenter;
		stage1_launch_mount_vertices[index][lc1].y = ycenter + radius*sin(increment*lc1);
		stage1_launch_mount_vertices[index][lc1].z = zcenter + radius*cos(increment*lc1);

		stage1_launch_mount_vertices[index][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_launch_mount_vertices[index][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_launch_mount_vertices[index][lc1].z *= STAGE1_NOZZLE_SCALE;

		stage1_launch_mount_vertices[index][lc1].tu = 0;
		stage1_launch_mount_vertices[index][lc1].tv = 0;
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int fill_stage1_pipe_vertices(int index, double xstart, double ystart, double zstart, double xend, double yend, double zend, double radius)
{
	vec start(xstart,ystart,zstart);
	vec end(xend, yend, zend);
	
	vec dir(xend -xstart, yend-ystart,zend -zstart);
	dir.normalize();
	
	//make a vector perpendicular to the direction vector.
	//this turns out to be surprisingly easy.
	vec vx(0, -dir.z, dir.y);

	//there is one corner case, if the y and z directions are 0
	//you would get (0,0,0) as the perpendicular vector.
	//however, in that case the correct vector is easy to pick out
	if (vx.mag() == 0)
		vx = vec(0,1,0);

	//create another vector perpendicular to the first two
	vec vy = dir | vx;

	double inc = (2*PI) / RESOLUTION;

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		vec vrt(0, 0, 0);

		vrt = vx*sin(inc*lc1) + vy*cos(inc*lc1);
		vrt = vrt*radius*0.01;

		stage1_pipes_vertices[index][0][lc1].x = xstart + vrt.x;
		stage1_pipes_vertices[index][0][lc1].y = ystart + vrt.y;
		stage1_pipes_vertices[index][0][lc1].z = zstart + vrt.z;

		stage1_pipes_vertices[index][0][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][0][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][0][lc1].z *= STAGE1_NOZZLE_SCALE;
	}

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		vec vrt(0,0,0);

		vrt = vx*sin(inc*lc1) + vy*cos(inc*lc1);
		vrt = vrt*radius;

		stage1_pipes_vertices[index][1][lc1].x = xstart + vrt.x;
		stage1_pipes_vertices[index][1][lc1].y = ystart + vrt.y;
		stage1_pipes_vertices[index][1][lc1].z = zstart + vrt.z;

		stage1_pipes_vertices[index][1][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][1][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][1][lc1].z *= STAGE1_NOZZLE_SCALE;

	}

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		vec vrt(0,0,0);

		vrt = vx*sin(inc*lc1) + vy*cos(inc*lc1);
		vrt = vrt*radius;

		stage1_pipes_vertices[index][2][lc1].x = xend + vrt.x;
		stage1_pipes_vertices[index][2][lc1].y = yend + vrt.y;
		stage1_pipes_vertices[index][2][lc1].z = zend + vrt.z;

		stage1_pipes_vertices[index][2][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][2][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][2][lc1].z *= STAGE1_NOZZLE_SCALE;

	}

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		vec vrt(0,0,0);

		vrt = vx*sin(inc*lc1) + vy*cos(inc*lc1);
		vrt = vrt*radius*0.01;

		stage1_pipes_vertices[index][3][lc1].x = xend + vrt.x;
		stage1_pipes_vertices[index][3][lc1].y = yend + vrt.y;
		stage1_pipes_vertices[index][3][lc1].z = zend + vrt.z;

		stage1_pipes_vertices[index][3][lc1].x *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][3][lc1].y *= STAGE1_NOZZLE_SCALE;
		stage1_pipes_vertices[index][3][lc1].z *= STAGE1_NOZZLE_SCALE;

	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_main_vertices()
{

	for (int lc1 = 0; lc1 < 20; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", stage1_main_vertices[lc1][lc2].x, stage1_main_vertices[lc1][lc2].y, stage1_main_vertices[lc1][lc2].z);
	}


	return 20*2*RESOLUTION;
}
//*********************************************************//
//*********************************************************//
int print_stage1_launch_mount_vertices()
{

	for (int lc1 = 0; lc1 < 3; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", stage1_launch_mount_vertices[lc1][lc2].x, stage1_launch_mount_vertices[lc1][lc2].y, stage1_launch_mount_vertices[lc1][lc2].z);
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_pipes_vertices()
{

	for (int lc0 = 0; lc0 < 5; lc0++)
	for (int lc1 = 0; lc1 < 4; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", stage1_pipes_vertices[lc0][lc1][lc2].x,stage1_pipes_vertices[lc0][lc1][lc2].y, stage1_pipes_vertices[lc0][lc1][lc2].z);
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_pipes_faces(int start)
{

	int i1;
	int i2;
	int i3;
	int i4;

	for (int lc0 = 0; lc0 < 5; lc0++)
	for (int lc1 = 0; lc1 < 3; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		i1 = lc1*RESOLUTION + lc2;
		i2 = lc1*RESOLUTION + lc2 + 1;
		i3 = (lc1+1)*RESOLUTION + lc2 + 1;
		i4 = (lc1+1)*RESOLUTION + lc2;

		i1 += lc0 * 4 * RESOLUTION;
		i2 += lc0 * 4 * RESOLUTION;
		i3 += lc0 * 4 * RESOLUTION;
		i4 += lc0 * 4 * RESOLUTION;

		if (lc2 == (RESOLUTION-1))
		{
			i2 -= RESOLUTION;
			i3 -= RESOLUTION;
		}
		fprintf(outfile, "f %d %d %d %d\n", i1+start+1, i2+start+1, i3+start+1, i4+start+1);

	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_main_faces(int start)
{

	int i1;
	int i2;
	int i3;
	int i4;

	for (int lc1 = 0; lc1 < 19; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		i1 = lc1*RESOLUTION + lc2;
		i2 = lc1*RESOLUTION + lc2 + 1;
		i3 = (lc1+1)*RESOLUTION + lc2 + 1;
		i4 = (lc1+1)*RESOLUTION + lc2;
		if (lc2 == (RESOLUTION-1))
		{
			i2 -= RESOLUTION;
			i3 -= RESOLUTION;
		}
		fprintf(outfile, "f %d %d %d %d\n", i4+start+1, i3+start+1, i2+start+1, i1+start+1);

	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_stage1_launch_mount_faces(int start)
{

	int i1;
	int i2;
	int i3;
	int i4;

	for (int lc1 = 0; lc1 < 2; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		i1 = lc1*RESOLUTION + lc2;
		i2 = lc1*RESOLUTION + lc2 + 1;
		i3 = (lc1+1)*RESOLUTION + lc2 + 1;
		i4 = (lc1+1)*RESOLUTION + lc2;
		if (lc2 == (RESOLUTION-1))
		{
			i2 -= RESOLUTION;
			i3 -= RESOLUTION;
		}
		fprintf(outfile, "f %d %d %d %d\n", i4+start+1, i3+start+1, i2+start+1, i1+start+1);

	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_stage1_main()
{
		
	stage1_main_vertex_ring(0,1332-972,0,0,188-187.9);
	stage1_main_vertex_ring(1,1329-972,0,0,210-187.9);
	stage1_main_vertex_ring(2,1327-972,0,0,226-187.9);
	stage1_main_vertex_ring(3,1324-972,0,0,236-187.9);
	stage1_main_vertex_ring(4,1321-972,0,0,246-187.9);
	stage1_main_vertex_ring(5,1316-972,0,0,255-187.9);
	stage1_main_vertex_ring(6,1312-972,0,0,262-187.9);
	stage1_main_vertex_ring(7,1307-972,0,0,269-187.9);
	stage1_main_vertex_ring(8,1300-972,0,0,275-187.9);
	stage1_main_vertex_ring(9,1288-972,0,0,279-187.9);
	stage1_main_vertex_ring(10,1280-972,0,0,280-187.9);
	stage1_main_vertex_ring(11,243-972,0,0,280-187.9);
	stage1_main_vertex_ring(12,229-972,0,0,272-187.9);
	stage1_main_vertex_ring(13,220-972,0,0,259-187.9);
	stage1_main_vertex_ring(14,209-972,0,0,230-187.9);
	stage1_main_vertex_ring(15,205-972,0,0,206-187.9);
	stage1_main_vertex_ring(16,200-972,0,0,199-187.9);
	stage1_main_vertex_ring(17,195-972,0,0,194-187.9);
	stage1_main_vertex_ring(18,187-972,0,0,194-187.9);
	stage1_main_vertex_ring(19,187-972,0,0,188-187.9);

	fill_stage1_pipe_vertices(0, 203-972, 272-187.9,    0.0,             118-972,      0, 0, 5.0);
	fill_stage1_pipe_vertices(1, 203-972, -(272-187.9), 0.0,             118-972,      0, 0, 5.0);
	fill_stage1_pipe_vertices(2, 203-972, 0.0,          272-187.9,       118-972,      0, 0, 5.0);
	fill_stage1_pipe_vertices(3, 203-972, 0.0,         -(272-187.9),     118-972,      0, 0, 5.0);
	fill_stage1_pipe_vertices(4, 203-972, 0.0,          0.0,             118-972,      0, 0, 5.0);

	stage1_launch_mount_vertex_ring(0,254-972,0,0,280-187.9);
	stage1_launch_mount_vertex_ring(1,209-972,0,0,280-187.9);
	stage1_launch_mount_vertex_ring(2,209-972,0,0,220-187.9);

	outfile = fopen("stage1_main.objx", "w");

		print_stage1_main_vertices();
		print_stage1_pipes_vertices();
		print_stage1_launch_mount_vertices();

		print_stage1_main_faces(0);
		print_stage1_pipes_faces(20*RESOLUTION);
		print_stage1_launch_mount_faces(20*RESOLUTION + 4*5*RESOLUTION);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_stage2_main()
{
		
	stage1_main_vertex_ring(0,751-751,0,0,256-255.9);
	stage1_main_vertex_ring(1,751-751,0,0,312-255.9);
	stage1_main_vertex_ring(2,719-751,0,0,336-255.9);
	stage1_main_vertex_ring(3,712-751,0,0,336-255.9);
	stage1_main_vertex_ring(4,704-751,0,0,340-255.9);
	stage1_main_vertex_ring(5,686-751,0,0,344-255.9);
	stage1_main_vertex_ring(6,668-751,0,0,348-255.9);
	stage1_main_vertex_ring(7,667-751,0,0,349-255.9);
	stage1_main_vertex_ring(8,665-751,0,0,349-255.9);
	stage1_main_vertex_ring(9,664-751,0,0,349-255.9);
	stage1_main_vertex_ring(10,663-751,0,0,349-255.9);
	stage1_main_vertex_ring(11,662-751,0,0,349-255.9);
	stage1_main_vertex_ring(12,661-751,0,0,349-255.9);
	stage1_main_vertex_ring(13,660-751,0,0,349-255.9);
	stage1_main_vertex_ring(14,659-751,0,0,349-255.9);
	stage1_main_vertex_ring(15,658-751,0,0,349-255.9);
	stage1_main_vertex_ring(16,657-751,0,0,349-255.9);
	stage1_main_vertex_ring(17,656-751,0,0,349-255.9);
	stage1_main_vertex_ring(18,420-751,0,0,349-255.9);
	stage1_main_vertex_ring(19,420-751,0,0,256-255.9);

	outfile = fopen("stage2_main.objx", "w");

		print_stage1_main_vertices();
		print_stage1_main_faces(0);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_payload()
{
		
	stage1_main_vertex_ring(0,1096-890,0,0,256-255.9);
	stage1_main_vertex_ring(1,1095-890,0,0,262-255.9);
	stage1_main_vertex_ring(2,1093-890,0,0,267-255.9);
	stage1_main_vertex_ring(3,1088-890,0,0,271-255.9);
	stage1_main_vertex_ring(4,1081-890,0,0,274-255.9);
	stage1_main_vertex_ring(5,1073-890,0,0,277-255.9);
	stage1_main_vertex_ring(6,1065-890,0,0,281-255.9);
	stage1_main_vertex_ring(7,1055-890,0,0,285-255.9);
	stage1_main_vertex_ring(8,1043-890,0,0,290-255.9);
	stage1_main_vertex_ring(9,1034-890,0,0,295-255.9);
	stage1_main_vertex_ring(10,1022-890,0,0,300-255.9);
	stage1_main_vertex_ring(11,1010-890,0,0,305-255.9);
	stage1_main_vertex_ring(12,995-890,0,0,311-255.9);
	stage1_main_vertex_ring(13,974-890,0,0,320-255.9);
	stage1_main_vertex_ring(14,934-890,0,0,330-255.9);
	stage1_main_vertex_ring(15,891-890,0,0,338-255.9);
	stage1_main_vertex_ring(16,890-890,0,0,338-255.9);
	stage1_main_vertex_ring(17,889-890,0,0,338-255.9);
	stage1_main_vertex_ring(18,719-890,0,0,339-255.9);
	stage1_main_vertex_ring(19,718-890,0,0,256-255.9);

	outfile = fopen("payload.objx", "w");

		print_stage1_main_vertices();
		print_stage1_main_faces(0);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//
int interstage_vertex_ring(int index, double xcenter, double ycenter, double zcenter, double radius)
{
	double increment = (2*PI) / RESOLUTION;

	for (int lc1 = 0; lc1 < RESOLUTION; lc1++)
	{
		interstage_vertices_inner[index][lc1].x = xcenter;
		interstage_vertices_inner[index][lc1].y = ycenter + radius*sin(increment*lc1);
		interstage_vertices_inner[index][lc1].z = zcenter + radius*cos(increment*lc1);

		interstage_vertices_inner[index][lc1].x *= STAGE1_NOZZLE_SCALE;
		interstage_vertices_inner[index][lc1].y *= STAGE1_NOZZLE_SCALE;
		interstage_vertices_inner[index][lc1].z *= STAGE1_NOZZLE_SCALE;

		interstage_vertices_inner[index][lc1].tu = 0;
		interstage_vertices_inner[index][lc1].tv = 0;

		interstage_vertices_outer[index][lc1].x = xcenter;
		interstage_vertices_outer[index][lc1].y = ycenter + (radius*1.01)*sin(increment*lc1);
		interstage_vertices_outer[index][lc1].z = zcenter + (radius*1.01)*cos(increment*lc1);

		interstage_vertices_outer[index][lc1].x *= STAGE1_NOZZLE_SCALE;
		interstage_vertices_outer[index][lc1].y *= STAGE1_NOZZLE_SCALE;
		interstage_vertices_outer[index][lc1].z *= STAGE1_NOZZLE_SCALE;

		interstage_vertices_outer[index][lc1].tu = 0;
		interstage_vertices_outer[index][lc1].tv = 0;
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_interstage_vertices()
{

	for (int lc1 = 0; lc1 < 2; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", interstage_vertices_inner[lc1][lc2].x, interstage_vertices_inner[lc1][lc2].y, interstage_vertices_inner[lc1][lc2].z);
	}

	for (int lc1 = 0; lc1 < 2; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		fprintf(outfile, "v %f %f %f\n", interstage_vertices_outer[lc1][lc2].x, interstage_vertices_outer[lc1][lc2].y, interstage_vertices_outer[lc1][lc2].z);
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int print_interstage_faces(int start)
{
	int gap = 2*RESOLUTION;

	int i1;
	int i2;
	int i3;
	int i4;

	for (int lc1 = 0; lc1 < 1; lc1++)
	for (int lc2 = 0; lc2 < RESOLUTION; lc2++)
	{
		i1 = lc1*RESOLUTION + lc2;
		i2 = lc1*RESOLUTION + lc2 + 1;
		i3 = (lc1+1)*RESOLUTION + lc2 + 1;
		i4 = (lc1+1)*RESOLUTION + lc2;
		if (lc2 == (RESOLUTION-1))
		{
			i2 -= RESOLUTION;
			i3 -= RESOLUTION;
		}
		fprintf(outfile, "f %d %d %d %d\n", i4+start+1, i3+start+1, i2+start+1, i1+start+1);
		fprintf(outfile, "f %d %d %d %d\n", i1+gap+start+1, i2+start+gap+1, i3+start+gap+1, i4+start+gap+1);
	}

	return 0;
}
//*********************************************************//
//*********************************************************//
int build_interstage()
{
		
	interstage_vertex_ring(0,0,0,0,    280-187.9 + 2);
	interstage_vertex_ring(1,345,0,0,  280-187.9 + 2);

	outfile = fopen("interstage.objx", "w");

	print_interstage_vertices();
	print_interstage_faces(0);

	fclose(outfile);	

	return 0;
}
//*********************************************************//
//*********************************************************//

int main()
{

	build_interstage();
	build_stage1_nozzle();
	build_stage1_main();
	build_stage2_nozzle();
	build_stage2_main();
	build_globe();
	build_payload();
	

	return 0;
}