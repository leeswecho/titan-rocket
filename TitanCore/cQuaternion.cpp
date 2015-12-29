#include "cQuaternion.h"
//***********************************************//
//***********************************************//
double cQuaternion::get_phi()
{
	return atan2(2*(w*i + j*k), 1 - 2*(i*i + j*j));
}
//***********************************************//
//***********************************************//
double cQuaternion::get_phi_deg()
{
	return get_phi() * RAD_TO_DEG;
}
//***********************************************//
//***********************************************//
double cQuaternion::get_theta()
{
	double convention_theta = asin(2*(w*j - k*i));	

	//this is done so that a positive theta yields the convention
	//that the x-axis rotates towards the z

	return convention_theta*-1;
}
//***********************************************//
//***********************************************//
double cQuaternion::get_theta_deg()
{
	return get_theta()*RAD_TO_DEG;
}
//***********************************************//
//***********************************************//
double cQuaternion::get_psi()
{
	return atan2(2*(w*k + i*j), 1 - 2*(j*j + k*k));
}
//***********************************************//
//***********************************************//
double cQuaternion::get_psi_deg()
{
	return get_psi() * RAD_TO_DEG;
}
//***********************************************//
//***********************************************//
//performs a post-multiplication (this * other)
int cQuaternion::apply_quaternion(cQuaternion oth)
{
	double nw = w*oth.w - i*oth.i - j*oth.j - k*oth.k;
	double ni = w*oth.i + i*oth.w + j*oth.k - k*oth.j;
	double nj = w*oth.j - i*oth.k + j*oth.w + k*oth.i;
	double nk = w*oth.k + i*oth.j - j*oth.i + k*oth.w;

    w = nw;
	i = ni;
	j = nj;
	k = nk;

	return 0;
}
//***********************************************//
//***********************************************//
int cQuaternion::set_rotation(double v_x, double v_y, double v_z, double rotation_rad)
{
	//normalize 
	double magnitude = sqrt(v_x*v_x + v_y*v_y + v_z*v_z);

	if (magnitude != 0)
	{
		v_x /= magnitude;
		v_y /= magnitude;
		v_z	/= magnitude;
	} else
	{
		v_x = 0;
		v_y = 0;
		v_z = 0;
	}

	double half_sine = sin(rotation_rad * 0.5);

	w = cos(rotation_rad * 0.5);
	i = half_sine*v_x;
	j = half_sine*v_y;
	k = half_sine*v_z;

	return 0;
}
//***********************************************//
//***********************************************//
cVec cQuaternion::rotate_vector(cVec input)
{
	return rotate_vector(input.x, input.y, input.z);
}
//***********************************************//
//***********************************************//
cVec cQuaternion::rotate_vector(double x, double y, double z)
{
	cQuaternion q_input;
	
	q_input.w = 0;
	q_input.i = x;
	q_input.j = y;
	q_input.k = z;

	cQuaternion q_output;
	q_output.w = w;
	q_output.i = i;
	q_output.j = j;
	q_output.k = k;

	cQuaternion q_inverse;
	q_inverse.w = w;
	q_inverse.i = -i;
	q_inverse.j = -j;
	q_inverse.k = -k;

	q_output.apply_quaternion(q_input);
	q_output.apply_quaternion(q_inverse);

	cVec ret(0, 0, 0);

	ret.x = q_output.i;
	ret.y = q_output.j;
	ret.z = q_output.k;

	return ret;
}
//***********************************************//
//***********************************************//
int cQuaternion::apply_rotation(double w_x, double w_y, double w_z, double dt)
{
	double rot_mag = 0;
	
	if ((w_x != 0) || (w_y != 0) || (w_z != 0))
		rot_mag = sqrt(w_x*w_x + w_y*w_y + w_z*w_z)*dt;
	
	cQuaternion q_new;
	q_new.set_rotation(w_x, w_y, w_z, rot_mag);

	cQuaternion q_curr;
	q_curr.w = w;
	q_curr.i = i;
	q_curr.j = j;
	q_curr.k = k;

	//apply_quaternion(q_new);
	//the new quaternion gets pre-multiplied onto the current quat
	//not post-multiplied
	q_new.apply_quaternion(q_curr);

	w = q_new.w;
	i = q_new.i;
	j = q_new.j;
	k = q_new.k;

	return 0;
}
//***********************************************//
//***********************************************//
int cQuaternion::set_rotation(double phi, double theta, double psi)
{
	//this is to enforce the convention that positive theta rotates the x-axis towards the z
	theta *= -1;

	double cosphi = cos(phi / 2.0);
	double sinphi = sin(phi / 2.0);
	double costheta = cos(theta / 2.0);
	double sintheta = sin(theta / 2.0);
	double cospsi = cos(psi / 2.0);
	double sinpsi = sin(psi / 2.0);

	w = cosphi*costheta*cospsi + sinphi*sintheta*sinpsi;
	i = sinphi*costheta*cospsi - cosphi*sintheta*sinpsi;
	j = cosphi*sintheta*cospsi + sinphi*costheta*sinpsi;
	k = cosphi*costheta*sinpsi - sinphi*sintheta*cospsi;

	return 0;
}
//***********************************************//
//***********************************************//
int cQuaternion::set_rotation_deg(double phi_deg, double theta_deg, double psi_deg)
{
	return set_rotation(phi_deg * DEG_TO_RAD, theta_deg * DEG_TO_RAD, psi_deg*DEG_TO_RAD);
}
//***********************************************//
//***********************************************//
cVec cQuaternion::get_pitch_axis()
{
	return rotate_vector(0, 1, 0); 
}
//***********************************************//
//***********************************************//
cVec cQuaternion::get_roll_axis()
{
	return rotate_vector(1, 0, 0); 
}
//***********************************************//
//***********************************************//
cVec cQuaternion::get_yaw_axis()
{
	return rotate_vector(0, 0, 1); 
}
//***********************************************//
//***********************************************//
