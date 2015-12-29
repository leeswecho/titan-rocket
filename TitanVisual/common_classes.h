#include <math.h>
#include <windows.h>

#pragma once

//this is normal, "3d" vertex format
struct vertex{
   float x, y, z;
   float nx, ny, nz;
   DWORD colour;
   float tu, tv;
};

//this is the 2d vertex format
struct vertex2d{
    float x, y;      // screen position    
    float z;         // Z-buffer depth    
    float rhw;       // reciprocal homogeneous W    
    DWORD Diffuse;   // diffuse color    
    float tu1, tv1;  // texture coordinates
};

//we bring Titan6's cVec class because the D3DXVECTOR version
//only has single precision, and that is starting to cause us pain
class VECTOR3
{
public: 
	double x;
	double y;
	double z;

	VECTOR3(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	VECTOR3()
	{
		x = 0;
		y = 0; 
		z = 0;
	}
	double mag()
	{
		double mag_squared = x*x + y*y + z*z;

		if (mag_squared == 0.0)
			return 0;
		else
			return sqrt(mag_squared);
	}

	void normalize()
	{
		double magnitude = mag();
		if (magnitude != 0)
		{
			x = x / magnitude;
			y = y / magnitude;
			z = z / magnitude;
		}
	}
	VECTOR3 get_normalized()
	{
		double magnitude = mag();
		if (magnitude != 0)
			return VECTOR3(x / magnitude, y / magnitude, z / magnitude);
		else
			return VECTOR3(0, 0, 0);		
	}
	VECTOR3 scale(double factor)
	{
		x *= factor;
		y *= factor; 
		z *= factor;

		return VECTOR3(x,y,z);
	}

	VECTOR3 operator*(double scale)
	{
			VECTOR3 ret;
			ret.x = this->x * scale;
			ret.y = this->y * scale;
			ret.z = this->z * scale;

			return ret;
	}
	friend VECTOR3 operator * ( double scale, const VECTOR3& rhs)
	{
			VECTOR3 ret;
			ret.x = rhs.x * scale;
			ret.y = rhs.y * scale;
			ret.z = rhs.z * scale;

			return ret;
	}


	VECTOR3 operator+(const VECTOR3& rhs)
	{
			VECTOR3 ret;
			ret.x = this->x + rhs.x;
			ret.y = this->y + rhs.y;
			ret.z = this->z + rhs.z;

			return ret;
	}
	VECTOR3 operator-(const VECTOR3& rhs)
	{
			VECTOR3 ret;
			ret.x = this->x - rhs.x;
			ret.y = this->y - rhs.y;
			ret.z = this->z - rhs.z;

			return ret;
	}

	double dot(VECTOR3 other)
	{
		return x*other.x + y*other.y + z*other.z;
	}

	double component_along(VECTOR3 other)
	{
		// a dot b = |a||b| cos(theta)
		VECTOR3 norm_a = get_normalized();
		VECTOR3 norm_b = other.get_normalized();

		double dot_product = norm_a.dot(norm_b);   //equal to cos(theta)

		return dot_product*mag();
	}

	VECTOR3 component_along_v(VECTOR3 other)
	{
		// a dot b = |a||b| cos(theta)
		VECTOR3 norm_a = get_normalized();
		VECTOR3 norm_b = other.get_normalized();

		double dot_product = norm_a.dot(norm_b);   //equal to cos(theta)

		return norm_b*dot_product*mag();
	}


	void invert()
	{
		scale(-1.0);
	}

};


//and our own cMatrix struct (this is the D3DMATRIX struct with the 
//floats changed to doubles
typedef struct _MATRIX {
    union {
        struct {
            double        _11, _12, _13, _14;
            double        _21, _22, _23, _24;
            double        _31, _32, _33, _34;
            double        _41, _42, _43, _44;

        };
        double m[4][4];
    };
} MATRIX;

vertex compute_normal(vertex p1, vertex p2, vertex p3);
int copy_normal(vertex* p1, vertex* p2);
MATRIX create_identity_matrix();

