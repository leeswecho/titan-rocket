#include "main.h"

//instantiate the data recorder.
cDataRecorder DataRecorder;

//this is the main function of the program.
int main()
{

	//initialize shared memory
	init_sm();
	
	//loop and read commands from 
	while (TRUE)
	{
		int iCommand = sm_read_int(RECORDER_COMMAND);

		//clear to indicate reception
		sm_write_int(RECORDER_COMMAND, 0);

		//act on command:
		//1 is quit
		if (iCommand == 1)
			break;
		//2 is to start recording
		if (iCommand == 2)
		{
			DataRecorder.record_begin();
		}
		//3 is to stop recording
		if (iCommand == 3)
		{
			DataRecorder.record_end();
		}
		//4 is to export files
		if (iCommand == 4)
		{
			DataRecorder.export_external_ephemeris(0, "stage1.e");
			DataRecorder.export_external_ephemeris(1, "stage2.e");
		}

		//give the recorder a chance to capture
		DataRecorder.capture_data();

		//sleep for 1 second to yield time
		Sleep(1);
	}

	return 0;
}