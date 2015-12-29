#include "common_classes.h"
#include "atm_table.h"

#pragma once

class cAtmosphereModel
{
public:
	cSharedMem* pSharedMem;

	int init(cSharedMem* _pSharedMem);

	int calculate_atmosphere_phase0(cState state);
	int calculate_atmosphere_phase1(cState stage1_state, cState interstage_state, cState stage2_state);

	//internal utility function to calculate the air temperature at the provided altitude above sea level.
	double get_air_temperature(double altitude);
	//internal utility function to calculate the air pressure at the provided altitude above sea level.
	double get_air_pressure(double altitude);
	//internal utility function to calculate the air density at the provided altitude above sea level.
	double get_air_density(double altitude);
	//this is the base linear interpolation function
	double get_interp(double altitude, const double* v_table);

private:

};