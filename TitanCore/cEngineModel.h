#include "common_classes.h"

#pragma once

class cThrustEffect
{
public:
	double dMass;
	double dIxx;
	double dIyy;
	double dIzz;

	cThrustEffect(double _dMass, double _dIxx, double _dIyy, double _dIzz)
	{
		dMass = _dMass;
		dIxx = _dIxx;
		dIyy = _dIyy;
		dIzz = _dIzz;
	}

	cThrustEffect()
	{
		dMass = 0;
		dIxx = 0;
		dIyy = 0;
		dIzz = 0;
	}

private:

};

class cEngineModel
{

public:
	bool bBurning;
	double mass_propellant;

	cForceMoment get_thrust_phase0(cState state);
	cThrustEffect get_thrust_effect_phase0(cState state, cForceMoment thrust);
	cForceMoment get_thrust_phase1(cState state);
	cThrustEffect get_thrust_effect_phase1(cState state, cForceMoment thrust);


	cEngineModel()
	{
		bBurning = true;
	}

	cSharedMem* pSharedMem;
		
	int init(cSharedMem* _pSharedMem) 
	{
		pSharedMem = _pSharedMem;

		return 0;
	}

private:


};