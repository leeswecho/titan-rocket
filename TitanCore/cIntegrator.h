#include <stdio.h>
#include "cQuaternion.h"
#include "cSharedMem.h"
#include "cForceModel.h"
#include "cControlModel.h"
#include "cAtmosphereModel.h"
#include "cAerodynamicsModel.h"

#pragma once

#define MAX_MASSES 5

//###################################################//
//###################################################//

class cPointMass
{
public:
	cState State;
private:

};

//###################################################//
//###################################################//

class cIntegrator
{
public:

	cSharedMem* pSharedMem;
	cForceModel* pForceModel;
	cControlModel* pControlModel;
	cAtmosphereModel* pAtmosphereModel;
	cAerodynamicsModel* pAerodynamicsModel;

	cPointMass Masses[MAX_MASSES];

	double sim_time;

	double num_masses;

	cState test_advance(cDerivative derivs, cState state, double time_step);
	cState rk4_advance(cState state, double time_step);
	cDerivative rk4_weighted_avg_derivs(cDerivative k1, cDerivative k2, cDerivative k3, cDerivative k4);
	int load_inputs(cForceMoment input_fm, cThrustEffect input_te, cForceMoment input_aero_fm);

	int init(cSharedMem* _pSharedMem, 
			cForceModel* _pForceModel, 
			cControlModel* _pControlModel, 
			cAtmosphereModel* _pAtmosphereModel,
			cAerodynamicsModel* _pAerodynamicsModel);
	int exit();

	int run_step_phase0(double time_step);
	int run_step_phase1(double time_step);


	int run_step(double time_step);


	int ic();
	int first_stage_sep();

private:

};