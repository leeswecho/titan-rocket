#include "cAerodynamicsModel.h"
//*****************************************************************//
//*****************************************************************//
//this function returns a vector 
cForceMoment cAerodynamicsModel::get_aerodyn_phase0(cState state)
{
	//assemble the quantities needed to produce the lift and drag forces:
	//velocity
	double v = state.get_v();
	//for now, just assume that the reference area is pi*r^2 (r = 0.85m)
	double ref_area = cPI * 0.85 * 0.85;
	//retrieve the air density from the variable
	double rho = pSharedMem->read_double(PHASE0_AIR_DENSITY);
	//retrieve other aerodynamic quantities
	double static_temp = pSharedMem->read_double(PHASE0_STATIC_AIR_TEMPERATURE);
	double static_pressure = pSharedMem->read_double(PHASE0_STATIC_PRESSURE);
	//compute the mach number
	double mach_number = compute_mach_number(rho, static_temp, static_pressure, v);
	//compute the angle fo attack
	double angle_of_attack = compute_angle_of_attack(state);

	//retrieve the drag coefficient
	double CD = get_CD_phase0(mach_number, angle_of_attack);

	//calculate the drag value
	double drag = 0.5*rho*ref_area*v*v*CD;

	//this vector is in the direction of the velocity. 
	cVec vel_vect(state.vx, state.vy, state.vz);

	//get a normalized vector
	cVec e_v = vel_vect.get_normalized();

	//scale this by the negative drag (since it's operating negative to the velocity vector)
	cVec v_drag = e_v.scale(-drag);

	//retrieve the lift coefficient
	double CL = get_CL_phase0(mach_number, angle_of_attack);

	//calculate the magnitude of the lift
	double lift = 0.5*rho*ref_area*v*v*CL;

	//the lift acts in the direction that is the difference between the velocity vector and the heading vector.
	
	//re-obtain the normalized velocity vector
	e_v = vel_vect.get_normalized();

	//intermediate: obtain the flight path angle (angle between current flight path and vertical)
	cVec e_up = state.convert_orientation_ENU_to_ECEF(cVec(0,0,1)).get_normalized();
	//get the dot product between the two unit vectors
	double dot_product = e_v.dot(e_up);
	//limit to prevent overflow
	if (dot_product > 1.0)
		dot_product = 1.0;
	if (dot_product < -1.0)
		dot_product = -1.0;
	//now get the angle
	double flight_path_angle = acos(dot_product);
	//now invert the angle so that straight up is 90 degrees
	flight_path_angle = (flight_path_angle * -1) + (cPI/2.0);
	//upload to SM
	pSharedMem->write_double(PHASE0_FLIGHT_PATH_ANGLE, flight_path_angle);

	//obtain a heading vector:
	//first get the heading vector in the EAST-NORTH-UP reference
	cVec hdg_enu = state.att.rotate_vector(cVec(1,0,0)).get_normalized();

	//now convert it to ECEF
	cVec e_hdg = state.convert_orientation_ENU_to_ECEF(hdg_enu).get_normalized();
	
	//now subtract one from the other and scale by the lift value:
	//note that if the two vectors are exactly equal, then the lift value 
	//that comes out will be 0, due to the way get_normalized() works.
	//This happens to be exactly what we want,
	//although for a subtly different reason: a symmetrical rocket should have 0 lift at
	//when flying straight ahead.

	//the rationale for the subtraction order is this:
	//the lift vector is in the direction going from velocity vector to the heading vector
	//(i.e. for an aircraft, positive angle of attack (orientation vector above velocity vector)
	//leads to upward lift)
	cVec v_lift = ( e_hdg - e_v ).get_normalized().scale(lift);

	//combine forces from lift and drag
	cVec f_aero = v_lift + v_drag;

	//return what we got. note that moments are 0 at the moment.
	return cForceMoment(f_aero.x, f_aero.y, f_aero.z, 0, 0, 0);

}
//*****************************************************************//
//*****************************************************************//
cForceMoment cAerodynamicsModel::get_aerodyn_phase1_stage2(cState state)
{
	//for now, we don't have any reliable data on what
	//the aerodynamic forces on the 2nd stage might look like, 
	//so return 0 here for now.
	cForceMoment ret(0,0,0,0,0,0);

	//compute the angle fo attack
	double angle_of_attack = compute_angle_of_attack(state);
	double aoa_deg = angle_of_attack * RAD_TO_DEG;

	//output to shared memory. don't know where to put this for now.
	pSharedMem->write_double(PHASE1_STAGE2_ANGLE_OF_ATTACK, aoa_deg);

	//this vector is in the direction of the velocity. 
	cVec vel_vect(state.vx, state.vy, state.vz);

	//get a normalized vector
	cVec e_v = vel_vect.get_normalized();

	//intermediate: obtain the flight path angle (angle between current flight path and vertical)
	cVec e_up = state.convert_orientation_ENU_to_ECEF(cVec(0,0,1)).get_normalized();
	//get the dot product between the two unit vectors
	double dot_product = e_v.dot(e_up);
	//limit to prevent overflow
	if (dot_product > 1.0)
		dot_product = 1.0;
	if (dot_product < -1.0)
		dot_product = -1.0;
	//now get the angle
	double flight_path_angle = acos(dot_product);
	//now invert the angle so that straight up is 90 degrees
	flight_path_angle = (flight_path_angle * -1) + (cPI/2.0);
	//upload to SM
	pSharedMem->write_double(PHASE1_STAGE2_FLIGHT_PATH_ANGLE, flight_path_angle);

	return ret;
}
//*****************************************************************//
//*****************************************************************//
cForceMoment cAerodynamicsModel::get_aerodyn_phase1_stage1(cState state)
{
	//for now, we don't have any reliable data for this, so, return 0
	cForceMoment ret(0,0,0,0,0,0);

	//compute the angle fo attack
	double angle_of_attack = compute_angle_of_attack(state);
	double aoa_deg = angle_of_attack * RAD_TO_DEG;

	//output to shared memory. don't know where to put this for now.
	pSharedMem->write_double(PHASE1_STAGE1_ANGLE_OF_ATTACK, aoa_deg);

	return ret;
}
//*****************************************************************//
//*****************************************************************//
cForceMoment cAerodynamicsModel::get_aerodyn_phase1_interstage(cState state)
{
	//for now, we don't have any reliable data for this, so, return 0
	cForceMoment ret(0,0,0,0,0,0);

	//compute the angle fo attack
	double angle_of_attack = compute_angle_of_attack(state);
	double aoa_deg = angle_of_attack * RAD_TO_DEG;

	//output to shared memory. don't know where to put this for now.
	pSharedMem->write_double(PHASE1_INTERSTAGE_ANGLE_OF_ATTACK, aoa_deg);

	return ret;
}
//*****************************************************************//
//*****************************************************************//
//this function retrieves the aerodynamic coefficient based on the state
double cAerodynamicsModel::get_CD_phase0(double mach_number, double angle_of_attack)
{
	//get angle of attack in degrees
	double aoa_deg = angle_of_attack * RAD_TO_DEG;

	//interpolate cd0 and cdaoa from the table
	double cd0 = get_interp(mach_number, cd0_table);
	double cdaoa = get_interp(mach_number, cdaoa_deg_table);

	double cd = cd0 + aoa_deg * cdaoa;

	//output this to shared memory here, for lack of any idea where to put it
	pSharedMem->write_double(PHASE0_CD, cd);
	pSharedMem->write_double(PHASE0_CD0, cd0);
	pSharedMem->write_double(PHASE0_ANGLE_OF_ATTACK, aoa_deg);

	//return what we got
	return cd;
}
//*****************************************************************//
//*****************************************************************//
//this function retrieves the aerodynamic coefficient based on the state
double cAerodynamicsModel::get_CL_phase0(double mach_number, double angle_of_attack)
{
	//because we don't have reliable lift numbers for a finless rocket,
	//we return 0, for now.

	return 0.0;
}
//*****************************************************************//
//*****************************************************************//
//utility function to compute angle of attack based on state.
double cAerodynamicsModel::compute_angle_of_attack(cState state)
{
	//obtain a normalized velocity vector
	cVec e_v = cVec(state.vx, state.vy, state.vz).get_normalized();

	//obtain a heading vector:
	//first get the heading vector in the EAST-NORTH-UP reference
	cVec hdg_enu = state.att.rotate_vector(cVec(1,0,0)).get_normalized();
	//now convert it to ECEF
	cVec e_hdg = state.convert_orientation_ENU_to_ECEF(hdg_enu).get_normalized();

	//now use the dot production relation
	// a dot b = |a||b| cos(theta) to get the angle between them.
	//because both are already unit vectors there is no need to compute their magnitudes
	double dot_product = e_v.dot(e_hdg);

	//due to rounding errors? in the calculation of the dot product,
	//we have to limit the dot product to a maximum of 1
	if (dot_product > 1)
		dot_product = 1;

	//get the angle as the arccosine
	return acos(dot_product);

}
//*****************************************************************//
//*****************************************************************//
double cAerodynamicsModel::get_interp(double mach_number, const double* v_table)
{
	//if we are lower than the lowest value in the M_table, then
	//return the lowest value
	if (mach_number < M_table[0])
		return v_table[0];
	//if we're higher than the highest value, just return the highest value
	if (mach_number > M_table[DRAG_TABLE_LENGTH -1])
		return v_table[DRAG_TABLE_LENGTH-1];

	//initialize some indexes 
	int lower_index = 0;
	int higher_index = 1;

	//loop until altitude is lower than the value at the higher index
	for (int i = 1; i < DRAG_TABLE_LENGTH; i++)
	{
		if (mach_number < M_table[i])
		{
			lower_index = i-1;
			higher_index = i;
			break;
		}
	}

	//now set up linear interpolation
	double base_val = v_table[lower_index];
	double base_M = M_table[lower_index];
	double dM = M_table[higher_index] - M_table[lower_index];
	double dv = v_table[higher_index] - v_table[lower_index];
	//dv/dM (we don't worry about divide by 0 because we know the M table always increments)
	double gradient = dv/dM;

	//actually calculate the value
	return base_val + (mach_number-base_M)*gradient;

}
//*****************************************************************//
//*****************************************************************//
//utility function to compute the mach number
double cAerodynamicsModel::compute_mach_number(double air_density, double air_temp, double air_pressure, double velocity)
{
	//compute speed of sound:
	//sqrt(gamma*R*T / M), where gamma = 1.4, R = 8.314510 J/mol-K, M = 0.0289645 kg/mol
	double a = sqrt(1.4 * (8.314510 / 0.0289645) * air_temp);
	//divide with velocity
	return velocity / a;

}