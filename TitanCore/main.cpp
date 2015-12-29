#include "main.h"
//**************************************************************************************//
//**************************************************************************************//
int map_sm()
{
	//hSharedMem = LoadLibraryA("Titan4DLL.dll");

	//read_int = (_read_int_type)GetProcAddress(hSharedMem, "_read_int");
	//write_int = (_write_int_type)GetProcAddress(hSharedMem, "_write_int");
	//read_double = (_read_double_type)GetProcAddress(hSharedMem, "_read_double");
	//write_double = (_write_double_type)GetProcAddress(hSharedMem, "_write_double");

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int init()
{
	Integrator.init(&SharedMem, &ForceModel, &ControlModel, &AtmosphereModel, &AerodynamicsModel);
	ControlModel.init(&SharedMem);
	ForceModel.init(&SharedMem);
	AtmosphereModel.init(&SharedMem);
	AerodynamicsModel.init(&SharedMem);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int exit()
{
	Integrator.exit();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int startup_tests()
{
	cState state;

	//confirm latitude and longitude calculation are correct:

	//prime meridian
	state.x = 1;
	state.y = 0;
	state.z = 0;
	double long1 = state.get_longitude_deg();
	double lat1 = state.get_latitude_deg();
	printf("lat: %f long: %f", lat1, long1);
	if ((long1 == 0.0) && (lat1 == 0.0))
		printf(" PASS\n");
	else
		printf(" FAIL\n");
	
	//north pole
	state.x = 0;
	state.y = 0;
	state.z = 1;
	double long2 = state.get_longitude_deg();
	double lat2 = state.get_latitude_deg();
	printf("lat: %f long: %f", lat2, long2);
	if (lat2 >= 90.0)  //longitude can be anything
		printf(" PASS\n");
	else
		printf(" FAIL\n");

	//check quaternion attitude conversions
	state.att.set_rotation(0,0,0);
	cVec rtd = state.att.rotate_vector(cVec(1,0,0));
	printf("0-0-0: %f, %f, %f\n", rtd.x, rtd.y, rtd.z);

	state.att.set_rotation_deg(0,0,90.0);
	rtd = state.att.rotate_vector(cVec(1,0,0));
	printf("0-0-90: %f, %f, %f\n", rtd.x, rtd.y, rtd.z);

	state.att.set_rotation_deg(0,90.0,0);
	rtd = state.att.rotate_vector(cVec(1,0,0));
	printf("0-90-0: %f, %f, %f\n", rtd.x, rtd.y, rtd.z);


/*	
	cVec vec_enu(0,0,1);
	cVec vec_ecef = state.convert_orientation_ENU_to_ECEF(vec_enu);

	printf("ret: %f, %f, %f\n", vec_ecef.x, vec_ecef.y, vec_ecef.z);
*/

	cQuaternion quat;
	quat.set_rotation_deg(0, -90.0, 0);
	double phi1 = quat.get_phi();
	double psi1 = quat.get_psi();
	double theta1 = quat.get_theta();
	quat.apply_rotation(0.01, 0, 0, 1.0);
	double phi2 = quat.get_phi();
	double psi2 = quat.get_psi();
	double theta2 = quat.get_theta();

	cQuaternion q1;
	q1.set_rotation_deg(0, 90, 0);
	cQuaternion q2;
	q2.set_rotation_deg(90, 0, 0);
	cQuaternion q3;
	q3.set_rotation_deg(0, 0, 90);



	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int debug_run()
{

	Integrator.ic();

	while (Integrator.sim_time < 30.0)
	{
		Integrator.run_step(0.01);
	}

	while (Integrator.sim_time < 169.0)
	{
		Integrator.run_step(0.01);
	}
	Integrator.first_stage_sep();
	
	for (int lc1 = 0; lc1 < 37800; lc1++)
		Integrator.run_step(0.01);
	

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int upload_coords_phase0()
{
		SharedMem.write_double(PHASE0_R, Integrator.Masses[0].State.get_r() / 1000.0);
		SharedMem.write_double(PHASE0_ROLL, Integrator.Masses[0].State.att.get_phi());
		SharedMem.write_double(PHASE0_PITCH, Integrator.Masses[0].State.att.get_theta());
		SharedMem.write_double(PHASE0_YAW, Integrator.Masses[0].State.att.get_psi());
		SharedMem.write_double(PHASE0_LONGITUDE, Integrator.Masses[0].State.get_longitude());
		SharedMem.write_double(PHASE0_LATITUDE, Integrator.Masses[0].State.get_latitude());
		SharedMem.write_double(SIM_TIME, Integrator.sim_time);

		SharedMem.write_double(PHASE0_X, Integrator.Masses[0].State.x);
		SharedMem.write_double(PHASE0_Y, Integrator.Masses[0].State.y);
		SharedMem.write_double(PHASE0_Z, Integrator.Masses[0].State.z);

		SharedMem.write_double(PHASE0_VX, Integrator.Masses[0].State.vx);
		SharedMem.write_double(PHASE0_VY, Integrator.Masses[0].State.vy);
		SharedMem.write_double(PHASE0_VZ, Integrator.Masses[0].State.vz);

		SharedMem.write_double(PHASE0_WX, Integrator.Masses[0].State.wx);
		SharedMem.write_double(PHASE0_WY, Integrator.Masses[0].State.wy);
		SharedMem.write_double(PHASE0_WZ, Integrator.Masses[0].State.wz);

		SharedMem.write_double(PHASE0_Q1, Integrator.Masses[0].State.att.w);
		SharedMem.write_double(PHASE0_Q2, Integrator.Masses[0].State.att.i);
		SharedMem.write_double(PHASE0_Q3, Integrator.Masses[0].State.att.j);
		SharedMem.write_double(PHASE0_Q4, Integrator.Masses[0].State.att.k);

		SharedMem.write_double(PHASE0_MASS, Integrator.Masses[0].State.mass);

		SharedMem.write_double(PHASE0_IXX, Integrator.Masses[0].State.Ixx);
		SharedMem.write_double(PHASE0_IYY, Integrator.Masses[0].State.Iyy);		
		SharedMem.write_double(PHASE0_IZZ, Integrator.Masses[0].State.Izz);


	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int upload_coords_phase1()
{
		SharedMem.write_double(SIM_TIME, Integrator.sim_time);

		SharedMem.write_double(PHASE1_STAGE1_X, Integrator.Masses[0].State.x);
		SharedMem.write_double(PHASE1_STAGE1_Y, Integrator.Masses[0].State.y);
		SharedMem.write_double(PHASE1_STAGE1_Z, Integrator.Masses[0].State.z);

		SharedMem.write_double(PHASE1_STAGE1_VX, Integrator.Masses[0].State.vx);
		SharedMem.write_double(PHASE1_STAGE1_VY, Integrator.Masses[0].State.vy);
		SharedMem.write_double(PHASE1_STAGE1_VZ, Integrator.Masses[0].State.vz);

		SharedMem.write_double(PHASE1_STAGE1_PITCH, Integrator.Masses[0].State.att.get_theta());
		SharedMem.write_double(PHASE1_STAGE1_ROLL, Integrator.Masses[0].State.att.get_phi());
		SharedMem.write_double(PHASE1_STAGE1_YAW, Integrator.Masses[0].State.att.get_psi());

		SharedMem.write_double(PHASE1_STAGE1_Q1, Integrator.Masses[0].State.att.w);
		SharedMem.write_double(PHASE1_STAGE1_Q2, Integrator.Masses[0].State.att.i);
		SharedMem.write_double(PHASE1_STAGE1_Q3, Integrator.Masses[0].State.att.j);
		SharedMem.write_double(PHASE1_STAGE1_Q4, Integrator.Masses[0].State.att.k);

		SharedMem.write_double(PHASE1_STAGE1_WX, Integrator.Masses[0].State.wx);
		SharedMem.write_double(PHASE1_STAGE1_WY, Integrator.Masses[0].State.wy);
		SharedMem.write_double(PHASE1_STAGE1_WZ, Integrator.Masses[0].State.wz);

		SharedMem.write_double(PHASE1_STAGE1_R, Integrator.Masses[0].State.get_r() / 1000.0);
		SharedMem.write_double(PHASE1_STAGE1_LONG, Integrator.Masses[0].State.get_longitude());
		SharedMem.write_double(PHASE1_STAGE1_LAT, Integrator.Masses[0].State.get_latitude());

		SharedMem.write_double(PHASE1_STAGE1_MASS, Integrator.Masses[0].State.mass);

		SharedMem.write_double(PHASE1_STAGE1_IXX, Integrator.Masses[0].State.Ixx);
		SharedMem.write_double(PHASE1_STAGE1_IYY, Integrator.Masses[0].State.Iyy);		
		SharedMem.write_double(PHASE1_STAGE1_IZZ, Integrator.Masses[0].State.Izz);

		SharedMem.write_double(PHASE1_INTERSTAGE_X, Integrator.Masses[1].State.x);
		SharedMem.write_double(PHASE1_INTERSTAGE_Y, Integrator.Masses[1].State.y);
		SharedMem.write_double(PHASE1_INTERSTAGE_Z, Integrator.Masses[1].State.z);

		SharedMem.write_double(PHASE1_INTERSTAGE_VX, Integrator.Masses[1].State.vx);
		SharedMem.write_double(PHASE1_INTERSTAGE_VY, Integrator.Masses[1].State.vy);
		SharedMem.write_double(PHASE1_INTERSTAGE_VZ, Integrator.Masses[1].State.vz);

		SharedMem.write_double(PHASE1_INTERSTAGE_PITCH, Integrator.Masses[1].State.att.get_theta());
		SharedMem.write_double(PHASE1_INTERSTAGE_ROLL, Integrator.Masses[1].State.att.get_phi());
		SharedMem.write_double(PHASE1_INTERSTAGE_YAW, Integrator.Masses[1].State.att.get_psi());

		SharedMem.write_double(PHASE1_INTERSTAGE_Q1, Integrator.Masses[1].State.att.w);
		SharedMem.write_double(PHASE1_INTERSTAGE_Q2, Integrator.Masses[1].State.att.i);
		SharedMem.write_double(PHASE1_INTERSTAGE_Q3, Integrator.Masses[1].State.att.j);
		SharedMem.write_double(PHASE1_INTERSTAGE_Q4, Integrator.Masses[1].State.att.k);

		SharedMem.write_double(PHASE1_INTERSTAGE_WX, Integrator.Masses[1].State.wx);
		SharedMem.write_double(PHASE1_INTERSTAGE_WY, Integrator.Masses[1].State.wy);
		SharedMem.write_double(PHASE1_INTERSTAGE_WZ, Integrator.Masses[1].State.wz);

		SharedMem.write_double(PHASE1_INTERSTAGE_R, Integrator.Masses[1].State.get_r() / 1000.0);
		SharedMem.write_double(PHASE1_INTERSTAGE_LONG, Integrator.Masses[1].State.get_longitude());
		SharedMem.write_double(PHASE1_INTERSTAGE_LAT, Integrator.Masses[1].State.get_latitude());

		SharedMem.write_double(PHASE1_INTERSTAGE_MASS, Integrator.Masses[1].State.mass);

		SharedMem.write_double(PHASE1_INTERSTAGE_IXX, Integrator.Masses[1].State.Ixx);
		SharedMem.write_double(PHASE1_INTERSTAGE_IYY, Integrator.Masses[1].State.Iyy);		
		SharedMem.write_double(PHASE1_INTERSTAGE_IZZ, Integrator.Masses[1].State.Izz);

		SharedMem.write_double(PHASE1_STAGE2_X, Integrator.Masses[2].State.x);
		SharedMem.write_double(PHASE1_STAGE2_Y, Integrator.Masses[2].State.y);
		SharedMem.write_double(PHASE1_STAGE2_Z, Integrator.Masses[2].State.z);

		SharedMem.write_double(PHASE1_STAGE2_VX, Integrator.Masses[2].State.vx);
		SharedMem.write_double(PHASE1_STAGE2_VY, Integrator.Masses[2].State.vy);
		SharedMem.write_double(PHASE1_STAGE2_VZ, Integrator.Masses[2].State.vz);

		SharedMem.write_double(PHASE1_STAGE2_PITCH, Integrator.Masses[2].State.att.get_theta());
		SharedMem.write_double(PHASE1_STAGE2_ROLL, Integrator.Masses[2].State.att.get_phi());
		SharedMem.write_double(PHASE1_STAGE2_YAW, Integrator.Masses[2].State.att.get_psi());

		SharedMem.write_double(PHASE1_STAGE2_Q1, Integrator.Masses[2].State.att.w);
		SharedMem.write_double(PHASE1_STAGE2_Q2, Integrator.Masses[2].State.att.i);
		SharedMem.write_double(PHASE1_STAGE2_Q3, Integrator.Masses[2].State.att.j);
		SharedMem.write_double(PHASE1_STAGE2_Q4, Integrator.Masses[2].State.att.k);

		SharedMem.write_double(PHASE1_STAGE2_WX, Integrator.Masses[2].State.wx);
		SharedMem.write_double(PHASE1_STAGE2_WY, Integrator.Masses[2].State.wy);
		SharedMem.write_double(PHASE1_STAGE2_WZ, Integrator.Masses[2].State.wz);

		SharedMem.write_double(PHASE1_STAGE2_R, Integrator.Masses[2].State.get_r() / 1000.0);
		SharedMem.write_double(PHASE1_STAGE2_LONG, Integrator.Masses[2].State.get_longitude());
		SharedMem.write_double(PHASE1_STAGE2_LAT, Integrator.Masses[2].State.get_latitude());
		
		SharedMem.write_double(PHASE1_STAGE2_MASS, Integrator.Masses[2].State.mass);

		SharedMem.write_double(PHASE1_STAGE2_IXX, Integrator.Masses[2].State.Ixx);
		SharedMem.write_double(PHASE1_STAGE2_IYY, Integrator.Masses[2].State.Iyy);		
		SharedMem.write_double(PHASE1_STAGE2_IZZ, Integrator.Masses[2].State.Izz);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int write_to_log_file()
{
	FILE* pNew = fopen("output.txt", "a+");

	if (pNew == NULL)
		return -1;

	fprintf(pNew, "%f,%f,%f,%f,%f,%f,%f,%f,%f\n", Integrator.Masses[0].State.x, 
												  Integrator.Masses[0].State.y, 
												  Integrator.Masses[0].State.z, 
												  Integrator.Masses[1].State.x, 
												  Integrator.Masses[1].State.y, 
												  Integrator.Masses[1].State.z, 
												  Integrator.Masses[2].State.x, 
												  Integrator.Masses[2].State.y, 
												  Integrator.Masses[2].State.z); 

	fclose(pNew);

	return 0;

}
//**************************************************************************************//
//**************************************************************************************//
//The main program performs initialization, and then goes into a loop waiting for external
//commands from the simulation drivers.
int main()
{
	init();

	startup_tests();
	
	//debug_run();
	//exit();
	//return 0;
/*	SharedMem.write_double(IC_PX_ECEF, 919767);
	SharedMem.write_double(IC_PY_ECEF, -5530997);
	SharedMem.write_double(IC_PZ_ECEF, -3040108);
	SharedMem.write_double(IC_VX_ECEF, 0);
	SharedMem.write_double(IC_VY_ECEF, 0);
	SharedMem.write_double(IC_VZ_ECEF, 0);
	SharedMem.write_double(IC_WX, 0);
	SharedMem.write_double(IC_WY, 0);
	SharedMem.write_double(IC_WZ, 0);
	SharedMem.write_double(IC_PITCH_RAD, 3.14159265/2.0);
	SharedMem.write_double(IC_ROLL_RAD, 0);
	SharedMem.write_double(IC_YAW_RAD, 0);
	SharedMem.write_double(IC_MASS_FUEL_FIRST_STAGE, 0);
	SharedMem.write_double(SIM_STEP_SIZE, 0.05);
	SharedMem.write_double(ENGINE_EXTERNAL_CONTROL, 0);
	Integrator.ic();
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
	Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));

	return 0;
*/
	Integrator.ic();

	while (TRUE)
	{
		Sleep(1);
		int command = SharedMem.read_int(SIM_COMMAND);

		if (command == 1)
		{
			Integrator.ic();
		}
		if (command == 2)
		{
			Integrator.run_step(SharedMem.read_double(SIM_STEP_SIZE));
			write_to_log_file();
		}
		if (command == 3)
		{
			break;
		}
		if (command == 100)
		{
			Integrator.first_stage_sep();
		}
		SharedMem.write_int(SIM_COMMAND, 0);

		int sim_phase = SharedMem.read_int(SIM_PHASE);

		if (sim_phase == 0)
			upload_coords_phase0();
		if (sim_phase == 1)
			upload_coords_phase1();

		//upload state of external engine control
		if (SharedMem.read_int(ENGINE_EXTERNAL_CONTROL) == 0)
			ControlModel.bEnabled = true;
		else
			ControlModel.bEnabled = false;
	}

	SharedMem.write_int(SIM_COMMAND, 0);

	exit();

	return 0;
}