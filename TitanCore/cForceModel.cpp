#include "cForceModel.h"
//*****************************************//
//*****************************************//
cDerivative cForceModel::get_derivatives(cState state)
{
	cDerivative ret;

	cForceMoment total(0,0,0,0,0,0); 
	cThrustEffect thrust_effect(0,0,0,0);  

	//get thrust force and moment
	total.fx = pSharedMem->read_double(THRUST_FX);
	total.fy = pSharedMem->read_double(THRUST_FY);
	total.fz = pSharedMem->read_double(THRUST_FZ);

	total.mx = pSharedMem->read_double(THRUST_MX);
	total.my = pSharedMem->read_double(THRUST_MY);
	total.mz = pSharedMem->read_double(THRUST_MZ);

	//get aerodynamic force and moment
	total.fx += pSharedMem->read_double(AERODYN_FX);
	total.fy += pSharedMem->read_double(AERODYN_FY);
	total.fz += pSharedMem->read_double(AERODYN_FZ);

	total.mx += pSharedMem->read_double(AERODYN_MX);
	total.my += pSharedMem->read_double(AERODYN_MY);
	total.mz += pSharedMem->read_double(AERODYN_MZ);

	//get effect of thrust on mass properties
	thrust_effect.dMass = pSharedMem->read_double(THRUST_EFFECT_DMASS);
	thrust_effect.dIxx = pSharedMem->read_double(THRUST_EFFECT_DIXX);
	thrust_effect.dIyy = pSharedMem->read_double(THRUST_EFFECT_DIYY);
	thrust_effect.dIzz = pSharedMem->read_double(THRUST_EFFECT_DIZZ);

	//now add the effect of ground reaction
	total = total + get_ground_reaction_force(state);
	//now add the effect of gravity
	total = total + get_gravity_force(state);
	//add centrifugal force
	total = total + get_centrifugal_force(state);
	//add coriolis force
	total = total + get_coriolis_force(state);

	ret.ax = total.fx / state.mass;
	ret.ay = total.fy / state.mass;
	ret.az = total.fz / state.mass;

	//
	ret.alx = (total.mx - (state.Izz - state.Iyy)*state.wy*state.wz) / state.Ixx;
	ret.aly = (total.my - (state.Ixx - state.Izz)*state.wx*state.wz) / state.Iyy;
	ret.alz = (total.mz - (state.Iyy - state.Ixx)*state.wx*state.wy) / state.Izz;

	ret.wx = state.wx;
	ret.wy = state.wy;
	ret.wz = state.wz;

	ret.vx = state.vx;
	ret.vy = state.vy; 
	ret.vz = state.vz;


	ret.dmass = 0 + thrust_effect.dMass;
	ret.dIxx = 0 + thrust_effect.dIxx;
	ret.dIyy = 0 + thrust_effect.dIyy;
	ret.dIzz = 0 + thrust_effect.dIzz;

	ret.dIxy = 0;
	ret.dIxz = 0;
	ret.dIyz = 0;

	return ret;
}
//*****************************************//
//*****************************************//
cForceMoment cForceModel::get_gravity_force(cState state)
{
	//get unit vector repreenting the direction
	cVec dir(state.x, state.y, state.z);
	
	//normalize and invert
	dir.normalize();
	dir.invert();

	//now get the magnitude
	//F_g = G*m_1*m_2/r^2
	double force = ((6.67e-11)*state.mass*5.97e24);

	if (dir.mag() != 0)
		force = force / (state.x*state.x + state.y*state.y + state.z*state.z);
	else
		force = 0;

	dir.scale(force);

	cForceMoment ret;

	ret.fx = dir.x;
	ret.fy = dir.y;
	ret.fz = dir.z;

	ret.mx = 0; 
	ret.my = 0;
	ret.mz = 0;

	return ret;

}
//*****************************************//
//*****************************************//
cForceMoment cForceModel::get_centrifugal_force(cState state)
{
	//get the angular velocity of the Earth's rotation
	double w_earth = (2.0*cPI)/86400.0;   //2 pi radians in 86400.0 seconds
	//make this into a vector
	cVec omega(0, 0, w_earth);
	//create a vector that is the position vector
	cVec r(state.x, state.y, state.z);

	//get the first cross product
	cVec omega_cross_r = omega.cross(r);
	//get the second cross product
	cVec ococr = omega.cross(omega_cross_r);

	//now scale by the negative of the mass
	cVec f_cen = ococr.scale(-state.mass);

	//return the result
	return cForceMoment(f_cen.x, f_cen.y, f_cen.z, 0, 0, 0);
}
//*****************************************//
//*****************************************//
cForceMoment cForceModel::get_coriolis_force(cState state)
{
	//get the angular velocity of the Earth's rotation
	double w_earth = (2.0*cPI)/86400.0;   //2 pi radians in 86400.0 seconds
	//make this into a vector
	cVec omega(0, 0, w_earth);
	//get the velocity as a vector
	cVec v(state.vx, state.vy, state.vz);

	//get the cross product
	cVec omega_cross_v = omega.cross(v);

	//scale by inverse of twice the mass
	cVec f_cor = omega_cross_v.scale(state.mass * -2.0);

	//return the result
	return cForceMoment(f_cor.x, f_cor.y, f_cor.z, 0, 0, 0);
}
//*****************************************//
//*****************************************//
cForceMoment cForceModel::get_ground_reaction_force(cState state)
{
	cForceMoment ret(0,0,0,0,0,0);

	//get a vector representing the direction of the ground force
	cVec dir(state.x, state.y, state.z);
	dir.normalize();

	//get the radius
	double rad = sqrt(state.x*state.x + state.y*state.y + state.z*state.z);
	//for now, the altitude, is relative to simply the mean radius of the earth
	double alt = rad - (6378100.0);

	if (alt >= 0)
	{
		ret.fx = 0;
		ret.fy = 0;
		ret.fz = 0;
		ret.mx = 0;
		ret.my = 0;
		ret.mz = 0;
		return ret;
	}

	//proportional (position) (elastic) component
	double prop_cmp =  -0.0001 * alt * state.mass;  //there's no point in having it bounce

	ret.fx = dir.x * prop_cmp;
	ret.fy = dir.y * prop_cmp;
	ret.fz = dir.z * prop_cmp;

	//velocity (inelastic) component 
	cVec v(state.vx, state.vy, state.vz);
	double vel_cmp = fabs(v.component_along(dir)) * state.mass*10;

	ret.fx += dir.x * vel_cmp;
	ret.fy += dir.y * vel_cmp;
	ret.fz += dir.z * vel_cmp;

	ret.mx = 0;
	ret.my = 0;
	ret.mz = 0;

	//get ground friction force:
	//start by getting z-component of velocity
	cVec v_vert = v.component_along_v(dir);
	//now remove that to get the purely ground component
	cVec v_gnd = v - v_vert;

	//now apply a constant (for now use the mass) in the opposite direction of the velocity
	cVec f_friction = v_gnd * -state.mass;

	//add that to the reaction force
	ret.fx += f_friction.x;
	ret.fy += f_friction.y;
	ret.fz += f_friction.z;

	return ret;
}
//*****************************************//
//*****************************************//