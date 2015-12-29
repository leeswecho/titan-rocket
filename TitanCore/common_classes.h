#include <math.h>
#include "cSharedMem.h"

#pragma once

#define cPI 3.14159265
#define RAD_TO_DEG (180.0/cPI)
#define DEG_TO_RAD (cPI/180.0)

class cVec
{
public: 
	double x;
	double y;
	double z;

	cVec(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	cVec()
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
	cVec get_normalized()
	{
		double magnitude = mag();
		if (magnitude != 0)
			return cVec(x / magnitude, y / magnitude, z / magnitude);
		else
			return cVec(0, 0, 0);		
	}
	cVec scale(double factor)
	{
		x *= factor;
		y *= factor; 
		z *= factor;

		return cVec(x,y,z);
	}

	cVec operator*(double scale)
	{
			cVec ret;
			ret.x = this->x * scale;
			ret.y = this->y * scale;
			ret.z = this->z * scale;

			return ret;
	}

	cVec operator+(const cVec& rhs)
	{
			cVec ret;
			ret.x = this->x + rhs.x;
			ret.y = this->y + rhs.y;
			ret.z = this->z + rhs.z;

			return ret;
	}
	cVec operator-(const cVec& rhs)
	{
			cVec ret;
			ret.x = this->x - rhs.x;
			ret.y = this->y - rhs.y;
			ret.z = this->z - rhs.z;

			return ret;
	}

	double dot(cVec other)
	{
		return x*other.x + y*other.y + z*other.z;
	}
	cVec cross(cVec other)
	{
		return cVec(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
	}

	double component_along(cVec other)
	{
		// a dot b = |a||b| cos(theta)
		cVec norm_a = get_normalized();
		cVec norm_b = other.get_normalized();

		double dot_product = norm_a.dot(norm_b);   //equal to cos(theta)

		return dot_product*mag();
	}
	cVec component_along_v(cVec other)
	{
		// a dot b = |a||b| cos(theta)
		cVec norm_a = get_normalized();
		cVec norm_b = other.get_normalized();

		double dot_product = norm_a.dot(norm_b);   //equal to cos(theta)

		return norm_b*dot_product*mag();
	}


	void invert()
	{
		scale(-1.0);
	}

	//just a quick utility function to convert long-lat-alt into ECEF x,y,z
	//Note: I could have done this using the Quaternion unit as well, but just for
	//mental exercise I did it this way.
	static cVec compute_ecef_xyz(double longitude_deg, double latitude_deg, double alt_meters)
	{
		//convert to radians
		double long_rad = longitude_deg * DEG_TO_RAD;
		double lat_rad = latitude_deg * DEG_TO_RAD;

		//create unit vector with prescribed longitude and latitude
		cVec output(cos(long_rad)*cos(lat_rad), sin(long_rad)*cos(lat_rad), sin(lat_rad));
		//scale by altitude
		output.scale(alt_meters);

		//done.
		return output;
	}

};

class cQuaternion
{
public:
	double w;  //q0
	double i;  //q1
	double j;  //q2
	double k;  //q3

	int apply_rotation(double w_x, double w_y, double w_z, double dt);
	int set_rotation(double phi, double theta, double psi);
	int set_rotation_deg(double phi_deg, double theta_deg, double psi_deg);
	int set_rotation(double v_x, double v_y, double v_z, double rotation_rad);

	int apply_quaternion(cQuaternion oth);

	cVec get_pitch_axis();
	cVec get_roll_axis();
	cVec get_yaw_axis();

	cVec rotate_vector(cVec input);
	cVec rotate_vector(double x, double y, double z);

	double get_theta();
	double get_theta_deg();
	double get_phi();
	double get_phi_deg();
	double get_psi();
	double get_psi_deg();
		
	~cQuaternion() { w = 0; i = 0; j = 0; k = 0; }
	cQuaternion() { set_rotation(0, 0, 0); }
	cQuaternion(double phi, double theta, double psi)
	{
		set_rotation(phi, theta, psi);
	}

};

class cState
{
public:
	//position
	double x;
	double y;
	double z;
	//attitude
	double phi;
	double psi;
	double theta;
	//attitude (quaternion)
	cQuaternion att;
	//velocity
	double vx;
	double vy;
	double vz;
	//angular velocity
	double wx;
	double wy;
	double wz;

	//properties
	double mass;
	double Ixx;
	double Iyy;
	double Izz;
	double Ixy;
	double Ixz;
	double Iyz;

	cState();
	int reset();
	double get_r();
	double get_v();
	double get_longitude()
	{
		//x axis of ECEF goes through prime meridian.
		//the range of the return value matches with east (negative) 
		//and west (positive) longitudes well, so no need to recalculate.
		return atan2(y, x);
	}
	double get_longitude_deg() {return get_longitude() * RAD_TO_DEG; }
	double get_latitude()
	{
		//get the adjacent side (the opposite side is just z)
		double adj = x*x + y*y;
		if (adj > 0)
			adj = sqrt(adj);
		//latitude will end up as just positive(north) and negative (south)
		return atan2(z, adj);
	}
	double get_latitude_deg() { return get_latitude() * RAD_TO_DEG; }

	//ENU is "east-north-up" (which is unwittingly the convention I started with when I made the models...)
	cVec convert_orientation_ENU_to_ECEF(cVec input)
	{
		double longitude = get_longitude();
		double latitude = get_latitude();

		//pre-processing:
		// longitude = +0 -->   psi = +90
		// longitude = +90 -->  psi = +180
		// longitude = +/- 180 --> psi = +270 = -90
		// longitude = +270 / -90 --> psi = 0
		double equiv_psi = longitude + (cPI / 2.0);
		if (equiv_psi >= (2*cPI))
			equiv_psi -= (2*cPI);
		//pre-processing:
		// latitude = +90 -->   phi = +0
		// latitude = +45 -->   phi = +45
		// latitude = 0   -->   phi = +90
		// latitude = -45 -->   phi = +135
		// latitude = -90 -->   phi = +180
		double equiv_phi = (latitude - (cPI / 2.0))*-1;
		//no range-wrapping needed since the desired ranges line up

		cQuaternion latlong(equiv_phi, 0, equiv_psi);

		return latlong.rotate_vector(input);

	}

private:

};

class cDerivative
{
public:
	double ax;
	double ay;
	double az;

	double vx;
	double vy;
	double vz;
	
	double alx; 
	double aly; 
	double alz;

	double wx;
	double wy;
	double wz;

	double dmass;
	double dIxx;
	double dIyy;
	double dIzz;
	double dIxz;
	double dIyz;
	double dIxy;

private:

};

class cForceMoment
{
public:
		double fx;
		double fy;
		double fz;

		double mx;
		double my;
		double mz;
	
		cForceMoment(double _fx, double _fy, double _fz, double _mx, double _my, double _mz)
		{
			fx = _fx;
			fy = _fy;
			fz = _fz;
			mx = _mx;
			my = _my;
			mz = _mz;
		}

		cForceMoment()
		{
			fx = 0;
			fy = 0; 
			fz = 0;
			mx = 0;
			my = 0;
			mz = 0;

		}
		cForceMoment operator+(const cForceMoment& rhs)
		{
			cForceMoment ret;
			ret.fx = this->fx + rhs.fx;
			ret.fy = this->fy + rhs.fy;
			ret.fz = this->fz + rhs.fz;

			ret.mx = this->mx + rhs.mx;
			ret.my = this->my + rhs.my;
			ret.mz = this->mz + rhs.mz;

			return ret;
		}
		cForceMoment operator-(const cForceMoment& rhs)
		{
			cForceMoment ret;
			ret.fx = this->fx - rhs.fx;
			ret.fy = this->fy - rhs.fy;
			ret.fz = this->fz - rhs.fz;

			ret.mx = this->mx - rhs.mx;
			ret.my = this->my - rhs.my;
			ret.mz = this->mz - rhs.mz;

			return ret;
		}

private:

};

class cAtmState
{
public:
	//SI unit: kg/m^3
	double air_density;
	//SI unit: kelvin
	double static_air_temperature;

	//default constructor
	cAtmState()
	{
		air_density = 0;
		static_air_temperature = 0;
	}
	//value constructor
	cAtmState(double _air_density, double _static_air_temperature)
	{
		air_density = _air_density;
		static_air_temperature = _static_air_temperature;
	}

private:



};