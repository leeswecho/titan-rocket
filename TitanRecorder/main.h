//this project implements a Data Recording function for the simulation.
//It is implemented separately from the core simulation so that it could theoretically
//be hosted on another system, away from the recording itself. 

#include <stdio.h>
#include <Windows.h>
#include "../common/sm_defines.h"
#include "../common/common_defines.h"
#include "../common/inc/TitanSM.h"

//this is the amount of data storage
#define DATA_STORAGE_FRAMES  1000000
//this is the number of objects we are tracking
#define NUM_OBJECTS 2

//definitions of recorder modes
#define RECORDER_MODE_INACTIVE   0
#define RECORDER_MODE_RECORDING  1

//this structure encapsulates the data stored in one cycle ("frame") of data recording. 
class cRecordFrame
{
public:

	//job number
	int iJobNumber;
	
	//time in seconds
	double fTime;
	
	//x coordinates in meters, ECEF
	double fX_ECEF;
	//y coordinates in meters, ECEF
	double fY_ECEF;
	//z coordinates in meters, ECEF
	double fZ_ECEF;
	
	//longitude
	double fLongitude;
	//latitude
	double fLatitude;
	//R (meters)
	double fR;

	//handy function to clear everything
	int clear() { iJobNumber = -1; fTime = 0; return 0; }

private:

};


//class is the self-contained implementation of the data recorder.
class cDataRecorder
{
public:

	//init function (this is a separate function just in case 
	//we ever need to init a second time).
	int init();
	//exit function (separate function just in case).
	int exit();

	//constructor (just calls init())
	cDataRecorder() { init(); }
	//destructor (just calls exit())
	~cDataRecorder() { exit(); }

	//this function is a way for the calling function
	//to give the recorder a chance to capture data
	int capture_data();

	//start a recording.
	int record_begin();
	//stop a recording.
	int record_end();

	//set the recording interval
	int set_recording_interval(double seconds_per_frame);

	//export to external ephemeris
	int export_external_ephemeris(int iObjNumber, const char* filename);

private:

	//sub-function to record data during mission phase 0
	int capture_data_phase0();
	//sub-function to record data during mission phase 1
	int capture_data_phase1();

	//the current recorder mode
	int iRecorderMode;

	//this is the current data recording interval.
	double fRecordInterval;
	//the last time at which we recorded anything
	double fLastRecordTime;
	//this is the index of the next buffer slot that will be written. 
	int iCurrEntry;
	//this is the current "job number" that the recorder is working on.
	int iJobNumber;

	//this is the underlying data storage. 
	//for initial simplicity, this is just a static array that functions as a circular buffer.
	cRecordFrame DataBuffer[NUM_OBJECTS][DATA_STORAGE_FRAMES];

};