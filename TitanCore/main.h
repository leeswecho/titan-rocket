//Description: Titan6 is the dynamic simulation itself. It performs
//the integration of the equations of motion for the stages of the rocket.
//It reads in commands to start, stop or step from the shared memory, and 
//outputs the state vectors to shared memory.

//standard includes
#include <windows.h>
#include <stdio.h>
//include the shared memory definition file
#include "sm_defines.h"
//include the shared utility classes
#include "common_classes.h"
//include the definitions of the classes we intend to instantiate
#include "cSharedMem.h"
#include "cIntegrator.h"
#include "cForceModel.h"
#include "cControlModel.h"
#include "cAtmosphereModel.h"
#include "cAerodynamicsModel.h"

//instantiate each of the pieces of the simulation here:
//the Shared Memory interface
cSharedMem SharedMem;
//the Force computation and aggregation
cForceModel ForceModel;
//the Integrator itself
cIntegrator Integrator;
//the (internal) Controller logic
cControlModel ControlModel;
//the Atmospheric property computation
cAtmosphereModel AtmosphereModel;
//the Aerodynamic Force computation
cAerodynamicsModel AerodynamicsModel;
