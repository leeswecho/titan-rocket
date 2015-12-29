#include "cEngineModel.h"
//***********************************************************//
//***********************************************************//
cForceMoment cEngineModel::get_thrust_phase0(cState state)
{
	//note: this is rated sea level thrust, which is limited by the 
	//fact that the exhaust gas can only be expanded to 
	//sea level pressure.
	//Thrust is listed to increase to 400 kN in vacuum. 
	double rated_thrust = 347000; //newtons

	cForceMoment ret(0, 0, 0, 0, 0, 0);

	double gimbal_roll = pSharedMem->read_double(STAGE1_ENGINE_GIMBAL_ROLL);
	double gimbal_tilt = pSharedMem->read_double(STAGE1_ENGINE_GIMBAL_TILT);
		
	double x_thrust = cos(gimbal_tilt);
	double y_thrust = sin(gimbal_tilt)*sin(gimbal_roll);
	double z_thrust = sin(gimbal_tilt)*cos(gimbal_roll);

	//use y and z thrusts to compute moments
	//temp: use 21.3/2 = 11.65m as moment arm (correct value would require position of CG)
	double x_moment = 0;
	double y_moment = z_thrust * 11.65 * rated_thrust;
	double z_moment = y_thrust * 11.65 * rated_thrust;

	cVec input(x_thrust, y_thrust, z_thrust);
	//this rotates from body to east-north-up
	cVec thrust_enu = state.att.rotate_vector(input);
	//we have to rotate again to get to ecef
	cVec thrust_ecef = state.convert_orientation_ENU_to_ECEF(thrust_enu);

	thrust_ecef.scale(rated_thrust);

// the fuel just burns for the entire first stage activity
//	if (state.mass < (27500 - 29510))
//		bBurning = false;
//	else
		bBurning = false;

	//first check if there is an external command to turn on the engine
	if (pSharedMem->read_int(STAGE1_ENGINE_ON) != 0)
		bBurning = true;

	//if we have less than 29510 - 21540 = 7970 kg of mass, stop burning because we have run out of fuel
	if (state.mass < 7970)
		bBurning = false;

	if (bBurning == true)
	{
		ret.fx = thrust_ecef.x;
		ret.fy = thrust_ecef.y;
		ret.fz = thrust_ecef.z;
		//note that, we are currently passing moments
		//in the body frame of reference,
		//while forces are being passed in ECEF frame of reference
		ret.mx = x_moment;
		ret.my = y_moment;
		ret.mz = z_moment;
	}

	return ret;
	
}
//***********************************************************//
//***********************************************************//
cThrustEffect cEngineModel::get_thrust_effect_phase0(cState state, cForceMoment thrust)
{
	cThrustEffect ret(0,0,0,0);

	//Wikipedia lists a fuel burn of 140 kg/sec.
	//but this is probably a figure for a specific time in flight as
	//a 169 sec burn at this rate uses more fuel than the user's guide says
	//the rocket carries. 

	if (bBurning == true)
	{
		//for the moment, just use an assumption that the 169 sec burn evenly burns all the 
		//21540 kg of available fuel at a constant rate, leading to about 127 kg/sec.
		ret.dMass = -21540.0 / 169.0;  //kg/sec
	}

	return ret;
}
//***********************************************************//
//***********************************************************//
cForceMoment cEngineModel::get_thrust_phase1(cState state)
{
	double rated_thrust = 31000; //newtons

	cForceMoment ret(0, 0, 0, 0, 0, 0);

	double gimbal_roll = pSharedMem->read_double(STAGE2_ENGINE_GIMBAL_ROLL);
	double gimbal_tilt = pSharedMem->read_double(STAGE2_ENGINE_GIMBAL_TILT);
		
	double x_thrust = cos(gimbal_tilt);
	double y_thrust = sin(gimbal_tilt)*sin(gimbal_roll);
	double z_thrust = sin(gimbal_tilt)*cos(gimbal_roll);

	//use y and z thrusts to compute moments
	//temp: use 8.25/2 = 4.13m as moment arm (correct value would require position of CG)
	double x_moment = 0;
	double y_moment = z_thrust * 4.13 * rated_thrust;
	double z_moment = y_thrust * 4.13 * rated_thrust;

	cVec input(x_thrust, y_thrust, z_thrust);
	//this rotates from body to east-north-up
	cVec thrust_enu = state.att.rotate_vector(input);
	//we have to rotate again to get to ecef
	cVec thrust_ecef = state.convert_orientation_ENU_to_ECEF(thrust_enu);

	thrust_ecef.scale(rated_thrust);

	bBurning = false;

	//first check if there is an external command to turn on the engine
	if (pSharedMem->read_int(STAGE2_ENGINE_ON) != 0)
		bBurning = true;

	//if we have less than 4590 - 4050 = 540 kg of mass, stop burning because we have run out of fuel
	if (state.mass < 540)
		bBurning = false;

	if (bBurning == true)
	{
		ret.fx = thrust_ecef.x;
		ret.fy = thrust_ecef.y;
		ret.fz = thrust_ecef.z;
		//note that, we are currently passing moments
		//in the body frame of reference,
		//while forces are being passed in ECEF frame of reference
		ret.mx = x_moment;
		ret.my = y_moment;
		ret.mz = z_moment;
	}


	return ret;
}
//***********************************************************//
//***********************************************************//
cThrustEffect cEngineModel::get_thrust_effect_phase1(cState state, cForceMoment thrust)
{
	cThrustEffect ret(0,0,0,0);

	if (bBurning == true)
	{
		//4050 kg in 378 sec
		ret.dMass = -4050.0 / 378.0;
	}

	return ret;
}
//***********************************************************//
//***********************************************************//