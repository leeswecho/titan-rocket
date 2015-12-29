// Kinect Header files
#include <Kinect.h>
#include <stdio.h>

int main()
{
	//some defines 
	#define BODY_COUNT 6  //6 bodies
	#define NUM_JOINTS 25	 // 25 joints

	//variables
	HRESULT hr;
	IKinectSensor* pSensor;
	IBodyFrameSource* pBodyFrameSource = NULL;
	ICoordinateMapper* pCoordinateMapper = NULL;
	IBodyFrameReader* pBodyFrameReader = NULL;

	//access the Kinect
	hr = GetDefaultKinectSensor(&pSensor);
	pSensor->Open();

	//obtain some structures we need
	pSensor->get_BodyFrameSource(&pBodyFrameSource);
	pSensor->get_CoordinateMapper(&pCoordinateMapper);
	pBodyFrameSource->OpenReader(&pBodyFrameReader);

	//main loop
	while (true)
	{
			//try to get frame
			IBodyFrame* pBodyFrame = NULL;
			hr = pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

			if (SUCCEEDED(hr))
			{ 
				//////////////////////////////////////////////////
				//download all the body data
				IBody* ppBodies[BODY_COUNT] = { 0 };
				pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
				///////////////////////////////////////////////////

				//go through each of the body slots
				for (int i = 0; i < BODY_COUNT; ++i)
				{
					//safe check
					if (ppBodies[i] != NULL)
					{
						//check to see whether Kinect actually found something in this slot
						BOOLEAN bTracked = false;
						ppBodies[i]->get_IsTracked(&bTracked);

						//if we are...
						if (bTracked == TRUE)
						{
							//DEBUG
							printf("Body %d is active!\n", i);						
							
							/////////////////////////////////////////////
							//Download the joint positions for all 25 joints	
							Joint joints[25];
							ppBodies[i]->GetJoints(25, joints);
							////////////////////////////////////////////

							//go through all the joints and print out where
							//it thinks the head and elbow are
							for (int j = 0; j < _countof(joints); ++j)
							{
								if (joints[j].JointType == _JointType::JointType_Head)
									printf("Head: x: %f y: %f z: %f\n", joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z);
								if (joints[j].JointType == _JointType::JointType_ElbowLeft)
									printf("Left Elbow: x: %f y: %f z: %f\n", joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z);
							}
						}
					}
				}

			//release of bodies and frame
			for (int i = 0; i < BODY_COUNT; ++i)
			{
				ppBodies[i]->Release();
			}
			pBodyFrame->Release();

		}   //end of succeeded getting frame

	}  //end while loop

	//close
	pSensor->Close();

	return 0;
}