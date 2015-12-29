#include "main.h"

//these are the required plugins for XPlane
PLUGIN_API int XPluginStart ( char * outName, char * outSignature, char * outDescription )
{
	strcpy(outName, "Titan Project Plugin for XPlane");
	strcpy(outSignature, "Vector.Inc.TitanProject");
	strcpy(outDescription, "Uploads state data from Titan Project environment");
	

	return 1;
}

//XPlane Plugin Stop callback
PLUGIN_API void XPluginStop ()
{
	return;
}

//XPlane Plugin Enable callback
PLUGIN_API void XPluginEnable ()
{
	return;
}

//XPlane Plugin Disable callback
PLUGIN_API void XPluginDisable ()
{
	return;
}

//XPlane Receive Message callback
PLUGIN_API void XPluginReceiveMessage ( XPLMPluginID inFrom, long inMessage, void * inParam )
{
	if (inFrom == XPLM_PLUGIN_XPLANE)
	{
		
	}

	return;
}


//entry point of DLL function.
int WINAPI DllMain()
{
	return TRUE;
}