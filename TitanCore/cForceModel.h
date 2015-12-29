#include "common_classes.h"
#include "cEngineModel.h"

#pragma once

class cForceModel
{
public:
	cEngineModel Engine;
	cForceMoment get_gravity_force(cState state);
	cForceMoment get_ground_reaction_force(cState state);
	cForceMoment get_aerodynamic_force(cState state);
	cForceMoment get_aerodynamic_force_drag(cState state);
	cForceMoment get_aerodynamic_force_lift(cState state);
	cForceMoment get_centrifugal_force(cState state);
	cForceMoment get_coriolis_force(cState state);

	cDerivative get_derivatives(cState state);

	cSharedMem* pSharedMem;

	int init(cSharedMem* _pSharedMem)
	{
		pSharedMem = _pSharedMem;
		Engine.init(pSharedMem);
		return 0;
	}

private:

};