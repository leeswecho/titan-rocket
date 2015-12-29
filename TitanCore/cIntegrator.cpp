#include "cIntegrator.h"
//*****************************************************//
//*****************************************************//
int cIntegrator::init(cSharedMem* _pSharedMem, 
					  cForceModel* _pForceModel, 
					  cControlModel* _pControlModel, 
					  cAtmosphereModel* _pAtmosphereModel,
					  cAerodynamicsModel* _pAerodynamicsModel)
{
	pSharedMem = _pSharedMem;
	pForceModel = _pForceModel;
	pControlModel = _pControlModel;
	pAtmosphereModel = _pAtmosphereModel;
	pAerodynamicsModel = _pAerodynamicsModel;

	for (int lc1 = 0; lc1 < MAX_MASSES; lc1++)
		Masses[lc1].State.reset();

	printf("Integrator: init.\n");

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::exit()
{
	printf("Integrator: exit.\n");

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::run_step_phase0(double time_step)
{
	printf("Integrator Phase 0: run step. Time: %f\n", sim_time);

	//in phase 0, the entire rocket is firing as one piece.

	//save previous state
	cState prev_state = Masses[0].State;

	//calculate atmospheric conditions
	pAtmosphereModel->calculate_atmosphere_phase0(Masses[0].State);
	//calculate aerodynamic forces moments
	cForceMoment aerodyns = pAerodynamicsModel->get_aerodyn_phase0(Masses[0].State);

	//put thrust into system
	cForceMoment thrust = pForceModel->Engine.get_thrust_phase0(prev_state);
	cThrustEffect thrust_effect = pForceModel->Engine.get_thrust_effect_phase0(prev_state, thrust);
	//load them into the integrator
	load_inputs(thrust, thrust_effect, aerodyns);

	//run the integration step
	Masses[0].State = rk4_advance(Masses[0].State, time_step);

	//recalculate control inputs
	pControlModel->calculate_controls_phase0(Masses[0].State, prev_state, sim_time);

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::run_step_phase1(double time_step)
{
	printf("Integrator Phase 1: run step. Time: %f\n", sim_time);

	//in phase 1, we are tracking the motion of three objects, the first stage,
	//the interstage fairing, and the second stage.
	//only the second stage is powered, the first stage and the interstage 
	//are falling back to Earth.

	//save previous states
	cState prev_state_first = Masses[0].State;
	cState prev_state_inter = Masses[1].State;
	cState prev_state_second = Masses[2].State;

	//calculate atmospheric conditions
	pAtmosphereModel->calculate_atmosphere_phase1(Masses[0].State, Masses[1].State, Masses[2].State);

	//calculate aerodynamic forces for fisrt stage
	cForceMoment aerodyns_first = pAerodynamicsModel->get_aerodyn_phase1_stage1(Masses[0].State);
	//calculate the first stage
	cForceMoment thrust_first(0,0,0,0,0,0); //no thrust
	cThrustEffect thrust_effect_first(0,0,0,0);  //no thrust effect
	//load them into the integrator
	load_inputs(thrust_first, thrust_effect_first, aerodyns_first);
	//run the integrator on the first stage
	Masses[0].State = rk4_advance(Masses[0].State, time_step);

	//calculate the interstage
	cForceMoment aerodyns_inter = pAerodynamicsModel->get_aerodyn_phase1_interstage(Masses[1].State);
	cForceMoment thrust_inter(0,0,0,0,0,0); //no thrust
	cThrustEffect thrust_effect_inter(0,0,0,0); //no thrust effect
	//load into integrator
	load_inputs(thrust_inter, thrust_effect_inter, aerodyns_inter);
	//run the integrator on the interstage
	Masses[1].State = rk4_advance(Masses[1].State, time_step);

	//calculate the second stage
	cForceMoment aerodyns_second = pAerodynamicsModel->get_aerodyn_phase1_stage2(Masses[2].State);
	cForceMoment thrust_second = pForceModel->Engine.get_thrust_phase1(prev_state_second); 
	cThrustEffect thrust_effect_second = pForceModel->Engine.get_thrust_effect_phase1(prev_state_second, thrust_second); 
	//load into integrator
	load_inputs(thrust_second, thrust_effect_second, aerodyns_second);
	//run the integrator on the second stage
	Masses[2].State = rk4_advance(Masses[2].State, time_step);
	//re-calculate control inputs on second stage
	pControlModel->calculate_controls_phase1(Masses[2].State, prev_state_second, sim_time);

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::run_step(double time_step)
{
	int sim_phase = pSharedMem->read_int(SIM_PHASE);

	if (sim_phase == 0)
	{
		run_step_phase0(time_step);
	}
	if (sim_phase == 1)
	{
		run_step_phase1(time_step);
	}

	sim_time += time_step;

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::ic()
{
	printf("Integrator: ic.\n");

	//just a goof to put the start location at Cape Canaveral
	cVec pos = cVec::compute_ecef_xyz(-80.5585, 28.4667, 6.3781e6);

	Masses[0].State.x = pSharedMem->read_double(IC_PX_ECEF); //pos.x;
	Masses[0].State.y = pSharedMem->read_double(IC_PY_ECEF); //pos.y;
	Masses[0].State.z = pSharedMem->read_double(IC_PZ_ECEF); //pos.z;

	Masses[0].State.vx = pSharedMem->read_double(IC_VX_ECEF);  //0.0;
	Masses[0].State.vy = pSharedMem->read_double(IC_VY_ECEF);  //0.0;
	Masses[0].State.vz = pSharedMem->read_double(IC_VZ_ECEF);  //0.0;
	
	double ic_pitch = pSharedMem->read_double(IC_PITCH_RAD);
	double ic_roll = pSharedMem->read_double(IC_ROLL_RAD);
	double ic_yaw = pSharedMem->read_double(IC_YAW_RAD);

	Masses[0].State.att.set_rotation(ic_roll, ic_pitch, ic_yaw);
	Masses[0].State.phi = Masses[0].State.att.get_phi();
	Masses[0].State.psi = Masses[0].State.att.get_psi();
	Masses[0].State.theta = Masses[0].State.att.get_theta();

	Masses[0].State.wx = pSharedMem->read_double(IC_WX);//0.0;
	Masses[0].State.wy = pSharedMem->read_double(IC_WY);//0.0;
	Masses[0].State.wz = pSharedMem->read_double(IC_WZ);//0.0;

	//read in a specified initial mass from SM
	double sm_mass = pSharedMem->read_double(IC_MASS_FUEL_FIRST_STAGE);

	//if the SM mass is 0 (i.e. invalid), use the default numbers instead
	if (sm_mass == 0)
	{
		//source: user's guide
		Masses[0].State.mass = 27500;   //kg
		//note: Falcon 1's user's guide says the first stage is carrying 21540 kg of propellant.

		//the below assumes the rocket having
		//2 axes of symmetry (thus only 3 different moments of inertia)
		//the following integral assumes the rocket being a cylinder 0.85m in radius
		//and 21.3m in length, and therefore the axis being in the middle. 
		Masses[0].State.Ixx = 13927.99375;   //about main axis: 0.5*m*r*r
		Masses[0].State.Iyy = 1464632.2;   //about secondary axis: (1/12)*m*(3*r*r + L*L)
		Masses[0].State.Izz = 1464632.2;     //about secondary axis: (1/12)*m*(3*r*r + L*L)
	} else
	{
		//not sure what to do here.

		//umm.....assume non-fuel mass is 7970 kg, add the fuel mass to the total
		Masses[0].State.mass = 7970 + sm_mass;   //kg

		//keep the rocket dimensions and inertias the same for lack of anything better I can do.
		Masses[0].State.Ixx = 13927.99375;   //about main axis: 0.5*m*r*r
		Masses[0].State.Iyy = 1464632.2;   //about secondary axis: (1/12)*m*(3*r*r + L*L)
		Masses[0].State.Izz = 1464632.2;     //about secondary axis: (1/12)*m*(3*r*r + L*L)
	}

	sim_time = 0;

	num_masses = 1;

	//reset all engine gimbals
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_TILT, 0);
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_ROLL, 0);
		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_TILT, 0);
		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_ROLL, 0);

	pSharedMem->write_int(SIM_PHASE, 0);

	return 0;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::first_stage_sep()
{
	//move the phase marker
	pSharedMem->write_int(SIM_PHASE, 1);

	//break up the old mass into multiple pieces
	//with slightly different reference positions

	//first get a unit vector representing the main axis 
	//of the rocket
	cVec axial = Masses[0].State.att.rotate_vector(1,0,0);
	axial = Masses[0].State.convert_orientation_ENU_to_ECEF(axial);
	
	//save the original pos
	cVec old_pos(Masses[0].State.x, Masses[0].State.y, Masses[0].State.z);
	//save the old velocity vector
	cVec old_vel(Masses[0].State.vx, Masses[0].State.vy, Masses[0].State.vz);
	//save the old attitude
	cState old_state = Masses[0].State;

	//reposition the new mass 0 point position at -5.23m of the old position
	//(this is mostly cosmetic. The end-to-end length of the first stage is about 12.6m,
	//this would basically put the new center for the first stage at exactly the middle of its length.)
	cVec new_stage1_pos = old_pos + (axial * -5.23);
	
	//reposition the interstage at +3.38m of the old position
	//(again, cosmetic, based on the position of the interstage on the rocket.
	cVec new_interstage_pos = old_pos + (axial * 3.38);

	//reposition the second stage at +6.53m of the old position
	cVec new_stage2_pos = old_pos + (axial * 6.53);

	//Now determine the new velocities of the components.
	//the charges that push apart the components have an unknown kinetic
	//energy, so we'll just use a scaling factor to make the separation
	//event more or less energetic.
	double energy_scale = 1.0;

	//assume that 4590 kg (i.e. no payload) of the remaining mass is the second stage,
	//145kg is the interstage, and whatever is left is the first stage.
	double first_stage_mass = Masses[0].State.mass - 4590.0 - 145.0;
	//assume that interstage doesn't change momentum (retains original velocity)
	//therefore ...
	double v1 = -1.0;
	double v2 = first_stage_mass / 4590.0;

	cVec new_stage1_vel = old_vel + (axial*v1*energy_scale);
	cVec new_stage2_vel = old_vel + (axial*v2*energy_scale);

	//now set the new values
	Masses[0].State.x = new_stage1_pos.x;
	Masses[0].State.y = new_stage1_pos.y;
	Masses[0].State.z = new_stage1_pos.z;
	Masses[0].State.vx = new_stage1_vel.x;
	Masses[0].State.vy = new_stage1_vel.y;
	Masses[0].State.vz = new_stage1_vel.z;
	//angular state remains unchanged.
	Masses[0].State.mass = first_stage_mass; //new mass
	Masses[0].State.Ixx = 1; //not really sure what to do here yet
	Masses[0].State.Iyy = 1; //not really sure what to do here yet
	Masses[0].State.Izz = 1; //not really sure what to do here yet

	//for the interstage, state properties are the same as the original
	Masses[1].State = old_state;
	//the mass properties are different
	Masses[1].State.mass = 145; //kg
	Masses[1].State.Ixx = 1; //dunno?
	Masses[1].State.Iyy = 1; //dunno?
	Masses[1].State.Izz = 1; //dunno?

	//for the second stage...
	Masses[2].State = old_state;
	Masses[2].State.x = new_stage2_pos.x;
	Masses[2].State.y = new_stage2_pos.y;
	Masses[2].State.z = new_stage2_pos.z;
	Masses[2].State.vx = new_stage2_vel.x;
	Masses[2].State.vy = new_stage2_vel.y;
	Masses[2].State.vz = new_stage2_vel.z;
	//the mass properties are different
	Masses[2].State.mass = 4950; //kg
	//for the new Ixx, Iyy, and Izz, assume for
	//the moment that the 2nd stage is a uniform cylinder 0.85m in
	//radius and 8.25m long.
	Masses[2].State.Ixx = 1788.2; //about main axis: 0.5*m*r*r
	Masses[2].State.Iyy = 28969.9; //about secondary axis: (1/12)*m*(3*r*r + L*L)
	Masses[2].State.Izz = 28969.9; //about secondary axis: (1/12)*m*(3*r*r + L*L)

	return 0;
}
//*****************************************************//
//*****************************************************//
cState cIntegrator::test_advance(cDerivative derivs, cState state, double time_step)
{
	cState ret;

	//NOTE: accelerations are assumed in ECEF frame of reference.

	//apply the accelerations to velocities
	ret.vx = state.vx + derivs.ax * time_step;
	ret.vy = state.vy + derivs.ay * time_step;
	ret.vz = state.vz + derivs.az * time_step;

	//apply the (deriv) velocities to positions
	ret.x = state.x + derivs.vx * time_step;
	ret.y = state.y + derivs.vy * time_step;
	ret.z = state.z + derivs.vz * time_step;

	//NOTE: angular accelerations are assumed in BODY frame of reference.

	//apply angular accelerations to velocities
	ret.wx = state.wx + derivs.alx * time_step;
	ret.wy = state.wy + derivs.aly * time_step;
	ret.wz = state.wz + derivs.alz * time_step;

	//apply the velocities to the quaternion
	ret.att.set_rotation(state.phi, state.theta, state.psi);
	//convert 
	cVec convert_vec = ret.att.rotate_vector(derivs.wx, derivs.wy, derivs.wz);
	ret.att.apply_rotation(convert_vec.x, convert_vec.y, convert_vec.z, time_step);

	//now get the new euler angles
	ret.phi = ret.att.get_phi();
	ret.psi = ret.att.get_psi();
	ret.theta = ret.att.get_theta();

	//state changes
	ret.mass = state.mass + derivs.dmass * time_step;
	ret.Ixx = state.Ixx + derivs.dIxx * time_step;
	ret.Iyy = state.Iyy + derivs.dIyy * time_step;
	ret.Izz = state.Izz + derivs.dIzz * time_step;

	ret.Ixz = state.Ixz + derivs.dIxz * time_step;
	ret.Iyz = state.Iyz + derivs.dIyz * time_step;
	ret.Ixy = state.Ixy + derivs.dIxy * time_step;


	return ret;
}
//*****************************************************//
//*****************************************************//
int cIntegrator::load_inputs(cForceMoment input_fm, cThrustEffect input_te, cForceMoment input_aero_fm)
{
	//this function exists because the integrator does not know
	//anything about there being three separate masses that might need
	//to be integrated at any given time, and thus when we run the integrator,
	//we "load" the stage-specific values (thrust, aerodynamic forces, etc.)
	//via SM.

	pSharedMem->write_double(THRUST_FX, input_fm.fx);
	pSharedMem->write_double(THRUST_FY, input_fm.fy);
	pSharedMem->write_double(THRUST_FZ, input_fm.fz);

	pSharedMem->write_double(THRUST_MX, input_fm.mx);
	pSharedMem->write_double(THRUST_MY, input_fm.my);
	pSharedMem->write_double(THRUST_MZ, input_fm.mz);

	pSharedMem->write_double(THRUST_EFFECT_DMASS, input_te.dMass);
	pSharedMem->write_double(THRUST_EFFECT_DIXX, input_te.dIxx);
	pSharedMem->write_double(THRUST_EFFECT_DIYY, input_te.dIyy);
	pSharedMem->write_double(THRUST_EFFECT_DIZZ, input_te.dIzz);

	pSharedMem->write_double(AERODYN_FX, input_aero_fm.fx);
	pSharedMem->write_double(AERODYN_FY, input_aero_fm.fy);
	pSharedMem->write_double(AERODYN_FZ, input_aero_fm.fz);

	pSharedMem->write_double(AERODYN_MX, input_aero_fm.mx);
	pSharedMem->write_double(AERODYN_MY, input_aero_fm.my);
	pSharedMem->write_double(AERODYN_MZ, input_aero_fm.mz);

	return 0;
}
//*****************************************************//
//*****************************************************//
cState cIntegrator::rk4_advance(cState state, double time_step)
{
	cState s1 = state;

	cState ret;

	//first test advance
	cDerivative k1 = pForceModel->get_derivatives(s1);
	cState s2 = test_advance(k1, s1, time_step / 2.0);

	cDerivative k2 = pForceModel->get_derivatives(s2);
	cState s3 = test_advance(k2, s1, time_step / 2.0);

	cDerivative k3 = pForceModel->get_derivatives(s3);
	cState s4 = test_advance(k3, s1, time_step);

	cDerivative k4 = pForceModel->get_derivatives(s4);
	
	cDerivative k1234 = rk4_weighted_avg_derivs(k1, k2, k3, k4);
	ret = test_advance(k1234, s1, time_step);

	return ret;
}
//*****************************************************//
//*****************************************************//
cDerivative cIntegrator::rk4_weighted_avg_derivs(cDerivative k1, cDerivative k2, cDerivative k3, cDerivative k4)
{
	cDerivative ret;

	ret.alx = 0.166666666666666*(k1.alx + 2*k2.alx + 2*k3.alx + k4.alx);
	ret.aly = 0.166666666666666*(k1.aly + 2*k2.aly + 2*k3.aly + k4.aly);
	ret.alz = 0.166666666666666*(k1.alz + 2*k2.alz + 2*k3.alz + k4.alz);

	ret.ax = 0.166666666666666*(k1.ax + 2*k2.ax + 2*k3.ax + k4.ax);
	ret.ay = 0.166666666666666*(k1.ay + 2*k2.ay + 2*k3.ay + k4.ay);
	ret.az = 0.166666666666666*(k1.az + 2*k2.az + 2*k3.az + k4.az);

	ret.wx = 0.166666666666666*(k1.wx + 2*k2.wx + 2*k3.wx + k4.wx);
	ret.wy = 0.166666666666666*(k1.wy + 2*k2.wy + 2*k3.wy + k4.wy);
	ret.wz = 0.166666666666666*(k1.wz + 2*k2.wz + 2*k3.wz + k4.wz);

	ret.vx = 0.166666666666666*(k1.vx + 2*k2.vx + 2*k3.vx + k4.vx);
	ret.vy = 0.166666666666666*(k1.vy + 2*k2.vy + 2*k3.vy + k4.vy);
	ret.vz = 0.166666666666666*(k1.vz + 2*k2.vz + 2*k3.vz + k4.vz);

	ret.dmass =  0.166666666666666*(k1.dmass + 2*k2.dmass + 2*k3.dmass + k4.dmass);
	ret.dIxx =  0.166666666666666*(k1.dIxx + 2*k2.dIxx + 2*k3.dIxx + k4.dIxx);
	ret.dIyy =  0.166666666666666*(k1.dIyy + 2*k2.dIyy + 2*k3.dIyy + k4.dIyy);
	ret.dIzz =  0.166666666666666*(k1.dIzz + 2*k2.dIzz + 2*k3.dIzz + k4.dIzz);

	ret.dIxz =  0.166666666666666*(k1.dIxz + 2*k2.dIxz + 2*k3.dIxz + k4.dIxz);
	ret.dIyz =  0.166666666666666*(k1.dIyz + 2*k2.dIyz + 2*k3.dIyz + k4.dIyz);
	ret.dIxy =  0.166666666666666*(k1.dIxy + 2*k2.dIxy + 2*k3.dIxy + k4.dIxy);

	return ret;
}
//*****************************************************//
//*****************************************************//