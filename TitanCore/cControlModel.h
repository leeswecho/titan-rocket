#include "common_classes.h"

#pragma once

class cControlModel
{
public:
	cSharedMem* pSharedMem;

	//this parameter controls whether the internal controller is enabled or not.
	bool bEnabled;

	int init(cSharedMem* _pSharedMem)
	{
		pSharedMem = _pSharedMem;
		bEnabled = true;
		return 0;
	}
	int calculate_controls_phase0(cState state, cState prev_state, double sim_time);
	int calculate_controls_phase1(cState state, cState prev_state, double sim_time);

private:


};