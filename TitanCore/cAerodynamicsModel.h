#include "common_classes.h"
#include "drag_table.h"

#pragma once

class cAerodynamicsModel
{
public:
	cSharedMem* pSharedMem;

	int init(cSharedMem* _pSharedMem)
	{
		pSharedMem = _pSharedMem;
		return 0;
	}
	cForceMoment get_aerodyn_phase0(cState state);
	cForceMoment get_aerodyn_phase1_stage1(cState state);
	cForceMoment get_aerodyn_phase1_interstage(cState state);
	cForceMoment get_aerodyn_phase1_stage2(cState state);

	//utility function to retrieve the drag coefficient
	double get_CD_phase0(double mach_number, double angle_of_attack);
	double get_CL_phase0(double mach_number, double angle_of_attack);

	//utility function to compute the angle of attack based on the state.
	double compute_angle_of_attack(cState state);
	//utility function to compute the mach number
	double compute_mach_number(double air_density, double air_temp, double air_pressure, double velocity);

	//another utility function to compute the linear interpolation of a table, this time based
	//on mach number
	double get_interp(double mach_number, const double* v_table);

private:


};