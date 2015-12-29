//this is the implementation of the data recorder.
#include "main.h"
//**********************************************************************************
//**********************************************************************************
//the init function
int cDataRecorder::init()
{
	//clear arrays
	for (int i = 0; i < DATA_STORAGE_FRAMES; i++)
	for (int j = 0; j < NUM_OBJECTS; j++)
	{
		DataBuffer[j][i].clear();
	}

	//reset pointers and job numbers
	iCurrEntry = 0;
	iJobNumber = 0;

	//set recorder mode to inactive
	iRecorderMode = RECORDER_MODE_INACTIVE;

	//default record interval is 0.2 seconds
	fRecordInterval = 0.2;

	//reset last record time
	fLastRecordTime = 0;

	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//the exit function
int cDataRecorder::exit()
{
	//nothing to do here, yet.


	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//the function to begin recording
int cDataRecorder::record_begin()
{

	//increment job number
	iJobNumber += 1;

	//set recorder mode
	iRecorderMode = RECORDER_MODE_RECORDING;

	//done. 
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//the function to end recording
int cDataRecorder::record_end()
{
	//set recorder mode
	iRecorderMode = RECORDER_MODE_INACTIVE;

	//done
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//set the recording interval
int cDataRecorder::set_recording_interval(double seconds_per_frame)
{
	//screen invalid values
	if (seconds_per_frame <= 0)
		return -1;

	//actually set it
	fRecordInterval = seconds_per_frame;

	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//give data recorder a chance to capture data.
int cDataRecorder::capture_data()
{
	//exit if not recording
	if (iRecorderMode != RECORDER_MODE_RECORDING)
		return 0;
	
	//obtain simulation time
	double sim_time = sm_read_double(SIM_TIME);

	//exit if not time to record.
	if ((sim_time - fLastRecordTime) < fRecordInterval)
		return 0;

	//otherwise obtain data:
	//obtain the mission phase
	int sim_phase = sm_read_int(SIM_PHASE);

	//route to appropriate sub-function based on phase
	if (sim_phase == 0)
		capture_data_phase0();
	if (sim_phase == 1)
		capture_data_phase1();

	//increment the recorder position
	iCurrEntry += 1;

	//if we exceeded the boundaries, go back to 0
	if (iCurrEntry >= DATA_STORAGE_FRAMES)
		iCurrEntry = 0;

	//update last record time
	fLastRecordTime = sim_time;

	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//sub-function for gathering data during phase 0
int cDataRecorder::capture_data_phase0()
{
	//in phase 0 the rocket is a single body, so both objects have the same position.

	//object 0:

	//job number
	DataBuffer[0][iCurrEntry].iJobNumber = iJobNumber;
	//sim time
	DataBuffer[0][iCurrEntry].fTime = sm_read_double(SIM_TIME);
	//x,y,z positions
	DataBuffer[0][iCurrEntry].fX_ECEF = sm_read_double(PHASE0_X);
	DataBuffer[0][iCurrEntry].fY_ECEF = sm_read_double(PHASE0_Y);
	DataBuffer[0][iCurrEntry].fZ_ECEF = sm_read_double(PHASE0_Z);
	//lat, long, r
	DataBuffer[0][iCurrEntry].fLongitude = sm_read_double(PHASE0_LONGITUDE);
	DataBuffer[0][iCurrEntry].fLatitude = sm_read_double(PHASE0_LATITUDE);
	DataBuffer[0][iCurrEntry].fR = sm_read_double(PHASE0_R);

	//object 1:

	//job number
	DataBuffer[1][iCurrEntry].iJobNumber = iJobNumber;
	//sim time
	DataBuffer[1][iCurrEntry].fTime = sm_read_double(SIM_TIME);
	//x,y,z positions
	DataBuffer[1][iCurrEntry].fX_ECEF = sm_read_double(PHASE0_X);
	DataBuffer[1][iCurrEntry].fY_ECEF = sm_read_double(PHASE0_Y);
	DataBuffer[1][iCurrEntry].fZ_ECEF = sm_read_double(PHASE0_Z);
	//lat, long, r
	DataBuffer[1][iCurrEntry].fLongitude = sm_read_double(PHASE0_LONGITUDE);
	DataBuffer[1][iCurrEntry].fLatitude = sm_read_double(PHASE0_LATITUDE);
	DataBuffer[1][iCurrEntry].fR = sm_read_double(PHASE0_R);

	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//sub-function for gathering data during phase 1
int cDataRecorder::capture_data_phase1()
{
	//in phase 1 the rocket is in three pieces, so we record separate positions for each.
	//...although we currently don't record the position of the interstage.

	//object 0 (stage 1 lower stage):

	//job number
	DataBuffer[0][iCurrEntry].iJobNumber = iJobNumber;
	//sim time
	DataBuffer[0][iCurrEntry].fTime = sm_read_double(SIM_TIME);
	//x,y,z positions
	DataBuffer[0][iCurrEntry].fX_ECEF = sm_read_double(PHASE1_STAGE1_X);
	DataBuffer[0][iCurrEntry].fY_ECEF = sm_read_double(PHASE1_STAGE1_Y);
	DataBuffer[0][iCurrEntry].fZ_ECEF = sm_read_double(PHASE1_STAGE1_Z);
	//lat, long, r
	DataBuffer[0][iCurrEntry].fLongitude = sm_read_double(PHASE1_STAGE1_LONG);
	DataBuffer[0][iCurrEntry].fLatitude = sm_read_double(PHASE1_STAGE1_LAT);
	DataBuffer[0][iCurrEntry].fR = sm_read_double(PHASE1_STAGE1_R);

	//object 1 (stage 2 upper stage):

	//job number
	DataBuffer[1][iCurrEntry].iJobNumber = iJobNumber;
	//sim time
	DataBuffer[1][iCurrEntry].fTime = sm_read_double(SIM_TIME);
	//x,y,z positions
	DataBuffer[1][iCurrEntry].fX_ECEF = sm_read_double(PHASE1_STAGE2_X);
	DataBuffer[1][iCurrEntry].fY_ECEF = sm_read_double(PHASE1_STAGE2_Y);
	DataBuffer[1][iCurrEntry].fZ_ECEF = sm_read_double(PHASE1_STAGE2_Z);
	//lat, long, r
	DataBuffer[1][iCurrEntry].fLongitude = sm_read_double(PHASE1_STAGE2_LONG);
	DataBuffer[1][iCurrEntry].fLatitude = sm_read_double(PHASE1_STAGE2_LAT);
	DataBuffer[1][iCurrEntry].fR = sm_read_double(PHASE1_STAGE2_R);

	//done.
	return 0;
}
//**********************************************************************************
//**********************************************************************************
//export the data to external ephemeris format
int cDataRecorder::export_external_ephemeris(int iObjNumber, const char* filename)
{
	//create a file at the indicated filename
	FILE* pFile = fopen(filename, "w");

	//abort if error
	if (pFile == NULL)
	{
		//
		printf("export_external_ephemeris(): error, could not create file.\n");
		return -1;
	}

	//do some homework to obtain some required parameters we need:
	//find the earliest data point with the current job number
	int iCurrEarliestIndex = -1;
	double fEarliestTime = 0;

	//go through all the data
	for (int i = 0; i < DATA_STORAGE_FRAMES; i++)
	{
		//if the job number matches
		if (DataBuffer[iObjNumber][i].iJobNumber == iJobNumber) 
		{
			//we have no data yet, use this point
			if (iCurrEarliestIndex == -1)
			{
				iCurrEarliestIndex = i;
				fEarliestTime = DataBuffer[iObjNumber][i].fTime;
			}
			else if (DataBuffer[iObjNumber][i].fTime < fEarliestTime) //if we have an earlier time, use that
			{
				iCurrEarliestIndex = i;
				fEarliestTime = DataBuffer[iObjNumber][i].fTime;
			}
		}
	}
	//now get the last data point
	int iLastIndex = iCurrEntry - 1;
	//wrap up if we hit 0
	if (iLastIndex < 0)
		iLastIndex += DATA_STORAGE_FRAMES;

	//now obtain the difference between the two points
	int iDifference = iLastIndex - iCurrEarliestIndex;

	//if the value is less tha 0, wrap back around
	if (iDifference < 0)
		iDifference += DATA_STORAGE_FRAMES;

	//save the number of points
	//the number of points is the difference between earliest and current
	//(i.e. diff plus one)
	int iNumPoints = iDifference+1;

	//start writing file data:
	//version stamp
	fprintf(pFile, "stk.v.8.0\n\n");
	//"BEGIN ephemeris" keyword
	fprintf(pFile, "BEGIN Ephemeris\n\n");

	
	fprintf(pFile, "NumberOfEphemerisPoints %d\n", iNumPoints);
	//distance unit is kilometers
	fprintf(pFile, "DistanceUnit            Kilometers\n");
	//central body is Earth
	fprintf(pFile, "CentralBody             Earth\n");
	//coordinate system is Fixed (ECEF)
	fprintf(pFile, "CoordinateSystem        Fixed\n\n");

	//ephemeris format, for now is LLATimePos
	fprintf(pFile, "EphemerisLLATimePos\n\n");

	//starting index is earliest index
	int iCurrWriteIndex = iCurrEarliestIndex;

	//now go through the databuffer and export the data
	for (int j = 0; j < iNumPoints; j++)
	{
		double ft = DataBuffer[iObjNumber][iCurrWriteIndex].fTime;
		double fx = DataBuffer[iObjNumber][iCurrWriteIndex].fX_ECEF;
		double fy = DataBuffer[iObjNumber][iCurrWriteIndex].fY_ECEF;
		double fz = DataBuffer[iObjNumber][iCurrWriteIndex].fZ_ECEF;
		double flat = DataBuffer[iObjNumber][iCurrWriteIndex].fLatitude * RAD_TO_DEG;
		double flong = DataBuffer[iObjNumber][iCurrWriteIndex].fLongitude * RAD_TO_DEG;
		double fa = DataBuffer[iObjNumber][iCurrWriteIndex].fR - 6378.1;   //umm....we need a better way to altitude here.

		//write the data
		fprintf(pFile, "%6.6f    %6.6f    %6.6f    %6.6f\n", ft, flat, flong, fa);

		//increment and wrap
		iCurrWriteIndex += 1;
		if (iCurrWriteIndex >= DATA_STORAGE_FRAMES)
			iCurrWriteIndex = 0;
	}

	//then finish
	fprintf(pFile, "\nEND Ephemeris");

	//close the file
	fclose(pFile);

	//done.
	return 0;
}