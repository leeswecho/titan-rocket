#include "cAtmosphereModel.h"
//*****************************************************************//
//*****************************************************************//
//initialization function.
//grabs shared memory pointer and initializes atmosphere tables.
int cAtmosphereModel::init(cSharedMem* _pSharedMem)
{
	//get shared memory pointer
	pSharedMem = _pSharedMem;

	//done. 
	return 0;
}
//*****************************************************************//
//*****************************************************************//
//this function uploads the atmospheric state to the appropriate shared memory variables,
//for use by other parts of the simulation as needed.
//It is done this way to allow the use of a separate program to calculate this if needed.
int cAtmosphereModel::calculate_atmosphere_phase0(cState state)
{
	//calculate the air density for this state 
	double rho = get_air_density(state.get_r() - 6378100.0);
	//get air temperature
	double t = get_air_temperature(state.get_r() - 6378100.0);
	//get air pressure
	double p = get_air_pressure(state.get_r() - 6378100.0);

	//upload these values to shared memory
	pSharedMem->write_double(PHASE0_AIR_DENSITY, rho);
	pSharedMem->write_double(PHASE0_STATIC_AIR_TEMPERATURE, t);
	pSharedMem->write_double(PHASE0_STATIC_PRESSURE, p);

	return 0;
}
//*****************************************************************//
//*****************************************************************//
int cAtmosphereModel::calculate_atmosphere_phase1(cState stage1_state, cState interstage_state, cState stage2_state)
{
	//calculate for stage 1

	//calculate the air density for this state 
	double rho = get_air_density(stage1_state.get_r() - 6378100.0);
	//get air temperature
	double t = get_air_temperature(stage1_state.get_r() - 6378100.0);
	//get air pressure
	double p = get_air_pressure(stage1_state.get_r() - 6378100.0);

	//upload these values to shared memory
	pSharedMem->write_double(PHASE1_STAGE1_AIR_DENSITY, rho);
	pSharedMem->write_double(PHASE1_STAGE1_STATIC_AIR_TEMPERATURE, t);
	pSharedMem->write_double(PHASE1_STAGE1_STATIC_PRESSURE, p);

	//calculate for interstage

	//calculate the air density for this state 
	rho = get_air_density(interstage_state.get_r() - 6378100.0);
	//get air temperature
	t = get_air_temperature(interstage_state.get_r() - 6378100.0);
	//get air pressure
	p = get_air_pressure(interstage_state.get_r() - 6378100.0);

	//upload these values to shared memory
	pSharedMem->write_double(PHASE1_INTERSTAGE_AIR_DENSITY, rho);
	pSharedMem->write_double(PHASE1_INTERSTAGE_STATIC_AIR_TEMPERATURE, t);
	pSharedMem->write_double(PHASE1_INTERSTAGE_STATIC_PRESSURE, p);

	//calculate for stage 2

	//calculate the air density for this state 
	rho = get_air_density(stage2_state.get_r() - 6378100.0);
	//get air temperature
	t = get_air_temperature(stage2_state.get_r() - 6378100.0);
	//get air pressure
	p = get_air_pressure(stage2_state.get_r() - 6378100.0);

	//upload these values to shared memory
	pSharedMem->write_double(PHASE1_STAGE2_AIR_DENSITY, rho);
	pSharedMem->write_double(PHASE1_STAGE2_STATIC_AIR_TEMPERATURE, t);
	pSharedMem->write_double(PHASE1_STAGE2_STATIC_PRESSURE, p);

	return 0;
}
//*****************************************************************//
//*****************************************************************//
double cAtmosphereModel::get_interp(double altitude, const double* v_table)
{
	//if we are lower than the lowest value in the h_table, then
	//return the lowest value
	if (altitude < h_table[0])
		return v_table[0];
	//if we're higher than the highest value, just return the highest value
	if (altitude > h_table[ATM_TABLE_LENGTH -1])
		return v_table[ATM_TABLE_LENGTH-1];

	//initialize some indexes 
	int lower_index = 0;
	int higher_index = 1;

	//loop until altitude is lower than the value at the higher index
	for (int i = 1; i < ATM_TABLE_LENGTH; i++)
	{
		if (altitude < h_table[i])
		{
			lower_index = i-1;
			higher_index = i;
			break;
		}
	}

	//now set up linear interpolation
	double base_val = v_table[lower_index];
	double base_h = h_table[lower_index];
	double dh = h_table[higher_index] - h_table[lower_index];
	double dv = v_table[higher_index] - v_table[lower_index];
	//dv/dh (we don't worry about divide by 0 because we know the h table always increments)
	double gradient = dv/dh;

	//actually calculate the value
	return base_val + (altitude-base_h)*gradient;

}
//*****************************************************************//
//*****************************************************************//
double cAtmosphereModel::get_air_pressure(double altitude)
{
	return get_interp(altitude, p_table);
}
//*****************************************************************//
//*****************************************************************//
double cAtmosphereModel::get_air_temperature(double altitude)
{
	return get_interp(altitude, t_table);

}
//*****************************************************************//
//*****************************************************************//
double cAtmosphereModel::get_air_density(double altitude)
{
	return get_interp(altitude, rho_table);
}
//*****************************************************************//
//*****************************************************************//