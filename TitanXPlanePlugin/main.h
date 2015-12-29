//This is a Titan Project plugin for the popular aircraft simulator XPlane.
//It is intended a means for the Titan environment to use XPlane as a visualizer,
//if it is available.
//When enabled, the plugin will read state data from the Titan environment (i.e. from
//the state variables in the shared memory) and send them to XPlane for visualization.
#include <Windows.h>

//as directed, define APL to 0 and IBM to 1 to indicate we're compiling on Windows
#define APL 0
#define IBM 1

//includes to the XPlane API
#include <XPLMPlugin.h>
#include <XPLMDataAccess.h>
#include <XPLMDisplay.h>
#include <XPLMDefs.h>

//create a test data reference
XPLMDataRef ref1;