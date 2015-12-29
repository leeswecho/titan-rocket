#include "cControlModel.h"
//*****************************************************************//
//*****************************************************************//
int cControlModel::calculate_controls_phase0(cState state, cState prev_state, double sim_time)
{
	if (bEnabled == false)
		return 0;

	//keep engine on
	pSharedMem->write_int(STAGE1_ENGINE_ON, 1);

	if (sim_time < 30.0)
	{
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_TILT, 0.0);
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_ROLL, 0);

	} else if (sim_time < 30.1)
	{
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_TILT, 1.0*DEG_TO_RAD);
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_ROLL, 0);


	} else 
	{
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_TILT, 0.0);
		pSharedMem->write_double(STAGE1_ENGINE_GIMBAL_ROLL, 0);
	}

	return 0;
}
//*****************************************************************//
//*****************************************************************//
int cControlModel::calculate_controls_phase1(cState state, cState prev_state, double sim_time)
{
	if (bEnabled == false)
		return 0;

	//keep engine on
	pSharedMem->write_int(STAGE2_ENGINE_ON, 1);

	//try to maintain theta above 0
	if (state.theta < 0)  //try to maintain angle at 0
	{
		double dtheta = state.theta - prev_state.theta;

		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_TILT, 0.001*state.theta + 1.0*dtheta);
		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_ROLL, 0);
	} else
	{
		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_TILT, 0);
		pSharedMem->write_double(STAGE2_ENGINE_GIMBAL_ROLL, 0);
	}

	return 0;
}
//*****************************************************************//
//*****************************************************************//
