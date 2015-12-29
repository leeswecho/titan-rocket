#include "main.h"
LRESULT CALLBACK WndCallBackProc0(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndCallBackProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

vertex2d square2d[] ={{ -960,  720, 0.5f, 1.0f, 0xFFFFFFFF, 0.0, 3.0 }, // x, y, z, rhw, color, tu, tv
					{ -960,  -720,  0.5f, 1.0f, 0xFFFFFFFF, 0.0, 0.0 }, 
					{ 960,  720, 0.5f, 1.0f, 0xFFFFFFFF, 3.0, 3.0}, 
					{ 960,  -720,  0.5f, 1.0f, 0xFFFFFFFF, 3.0, 0.0}}; 

vertex2d sp2d[] = {{ -2,  2, 0.5f, 1.0f, 0xFFFFFFFF, 0.0, 1.0 }, // x, y, z, rhw, color, tu, tv
					{ -2,  -2,  0.5f, 1.0f, 0xFFFFFFFF, 0.0, 0.0 }, 
					{ 2,  2, 0.5f, 1.0f, 0xFFFFFFFF, 1.0, 1.0}, 
					{ 2,  -2,  0.5f, 1.0f, 0xFFFFFFFF, 1.0, 0.0}}; 

    float x, y;      // screen position    
    float z;         // Z-buffer depth    
    float rhw;       // reciprocal homogeneous W    
    DWORD Diffuse;   // diffuse color    
    DWORD Specular;  // specular color    
    float tu1, tv1;  // texture coordinates

//3D Objects
cRenderObject* pShip;
cRenderObject* pStage1Nozzle;
cRenderObject* pInterstage;
cRenderObject* pEarth;
cRenderObject* pStage2Nozzle;
cRenderObject* pStage2Main;
cRenderObject* pPayload;
//2D Objects
cRenderObject* pMap2D;
cRenderObject* pStage1_2D;
cRenderObject* pStage2_2D;

double input_scale;

//playing around
#include <Kinect.h>
//some defines 
#define BODY_COUNT 6  //6 bodies
#define NUM_JOINTS 25	 // 25 joints
IKinectSensor* pSensor;
IBodyFrameSource* pBodyFrameSource = NULL;
ICoordinateMapper* pCoordinateMapper = NULL;
IBodyFrameReader* pBodyFrameReader = NULL;
//**************************************************************************************//
//**************************************************************************************//
int initKinect()
{
	//access the Kinect
	HRESULT hr = GetDefaultKinectSensor(&pSensor);
	pSensor->Open();

	//obtain some structures we need
	pSensor->get_BodyFrameSource(&pBodyFrameSource);
	pSensor->get_CoordinateMapper(&pCoordinateMapper);
	pBodyFrameSource->OpenReader(&pBodyFrameReader);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int exitKinect()
{
	pSensor->Close();
	pSensor->Release();
	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int processKinect()
{

	//try to get frame
	IBodyFrame* pBodyFrame = NULL;
	HRESULT hr = pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

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
					//printf("Body %d is active!\n", i);

					/////////////////////////////////////////////
					//Download the joint positions for all 25 joints	
					Joint joints[25];
					ppBodies[i]->GetJoints(25, joints);
					////////////////////////////////////////////

					//go through all the joints and print out where
					//it thinks the head and elbow are
					for (int j = 0; j < _countof(joints); ++j)
					{
						//if (joints[j].JointType == _JointType::JointType_Head)
						//	printf("Head: x: %f y: %f z: %f\n", joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z);
						if (joints[j].JointType == _JointType::JointType_HandLeft)
						{
							D3D.Devices[0].cam_yaw = joints[j].Position.X;
							D3D.Devices[0].cam_tilt = joints[j].Position.Y;
							D3D.Devices[0].bViewChanged = true;
						}
						
							//	printf("Left Elbow: x: %f y: %f z: %f\n", joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z);
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

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
D3DXVECTOR3 get_d3d_vector(VECTOR3 input)
{
	return D3DXVECTOR3(input.x, input.y, input.z);
}
//**************************************************************************************//
//**************************************************************************************//
int calc_views_earthfixed(cD3D_Device* pDevice, double cam_dist, double cam_tilt, double cam_yaw)
{

	double base_pitch = pDevice->RenderList->pitch;
	double base_roll = pDevice->RenderList->roll;
	double base_yaw = pDevice->RenderList->yaw;

	//the original basis vectors
	D3DXVECTOR3 xbody0 = D3DXVECTOR3(1,0,0);
	D3DXVECTOR3 ybody0 = D3DXVECTOR3(0,1,0);
	D3DXVECTOR3 zbody0 = D3DXVECTOR3(0,0,1);

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for yaw
	D3DXVECTOR3 xbody1a = (cos(base_yaw) * xbody0) + (sin(base_yaw) * ybody0);
	D3DXVECTOR3 zbody1a = zbody0;
	D3DXVECTOR3 ybody1a = (cos(base_yaw) * ybody0) - (sin(base_yaw) * xbody0);

	//perform rotation to account for to account for pitch
	D3DXVECTOR3 xbody2a = (cos(base_pitch) * xbody1a) - (sin(base_pitch) * zbody1a);
	D3DXVECTOR3 zbody2a = (cos(base_pitch) * zbody1a) + (sin(base_pitch) * xbody1a);
	D3DXVECTOR3 ybody2a = ybody1a;

	//perform rotation to account for to account for roll
	D3DXVECTOR3 ybody3a = (cos(base_roll) * ybody2a) - (sin(base_roll) * zbody2a);
	D3DXVECTOR3 zbody3a = (cos(base_roll) * zbody2a) + (sin(base_roll) * ybody2a);
	D3DXVECTOR3 xbody3a = xbody2a;

	//perform rotation to account for cam yaw
	D3DXVECTOR3 xbody4 = (cos(cam_yaw) * xbody3a) + (sin(cam_yaw) * ybody3a);
	D3DXVECTOR3 zbody4 = zbody3a;
	D3DXVECTOR3 ybody4 = (cos(cam_yaw) * ybody3a) - (sin(cam_yaw) * xbody3a);

	//perform rotation to account for cam tilt
	D3DXVECTOR3 xbody5 = (cos(cam_tilt) * xbody4) + (sin(cam_tilt) * zbody4);
	D3DXVECTOR3 zbody5 = (cos(cam_tilt) * zbody4) - (sin(cam_tilt) * xbody4);
	D3DXVECTOR3 ybody5 = ybody4;

	D3DXVECTOR3 base = D3DXVECTOR3(0,0,0);

	base = cam_dist * xbody5;

	pDevice->lookat_vector = D3DXVECTOR3(0,0,0);
	pDevice->eye_vector = base;
	pDevice->up_vector = zbody5;

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int calc_views_targetfixed(cD3D_Device* pDevice, double r, double longitude, double latitude, 
								 double cam_dist, double cam_tilt, double cam_yaw,
								 double target_pitch, double target_roll, double target_yaw, double x_body_offset)
{

	//just set them all to 0 for now.
	double base_pitch = 0;//pDevice->RenderList->pitch;
	double base_roll = 0;//pDevice->RenderList->roll;
	double base_yaw = 0; //cam_yaw;//pDevice->RenderList->yaw;
	target_pitch = 0;
	target_roll = 0;
	target_yaw = 0;

	//the original basis vectors
	VECTOR3 xbody0 = VECTOR3(1,0,0);
	VECTOR3 ybody0 = VECTOR3(0,1,0);
	VECTOR3 zbody0 = VECTOR3(0,0,1);

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for yaw
	VECTOR3 xbody1a = (cos(base_yaw) * xbody0) + (sin(base_yaw) * ybody0);
	VECTOR3 zbody1a = zbody0;
	VECTOR3 ybody1a = (cos(base_yaw) * ybody0) - (sin(base_yaw) * xbody0);

	//perform rotation to account for to account for pitch
	VECTOR3 xbody2a = (cos(base_pitch) * xbody1a) - (sin(base_pitch) * zbody1a);
	VECTOR3 zbody2a = (cos(base_pitch) * zbody1a) + (sin(base_pitch) * xbody1a);
	VECTOR3 ybody2a = ybody1a;

	//perform rotation to account for to account for roll
	VECTOR3 ybody3a = (cos(base_roll) * ybody2a) - (sin(base_roll) * zbody2a);
	VECTOR3 zbody3a = (cos(base_roll) * zbody2a) + (sin(base_roll) * ybody2a);
	VECTOR3 xbody3a = xbody2a;

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for longitude
	VECTOR3 xbody1 = (cos(-longitude) * xbody3a) - (sin(-longitude) * ybody3a);
	VECTOR3 zbody1 = zbody3a;
	VECTOR3 ybody1 = (cos(-longitude) * ybody3a) + (sin(-longitude) * xbody3a);

	//D3DXVECTOR3 xbody1 = (cos(longitude) * xbody0) - (sin(longitude) * ybody0);
	//D3DXVECTOR3 zbody1 = zbody0;
	//D3DXVECTOR3 ybody1 = (cos(longitude) * ybody0) + (sin(longitude) * xbody0);

	//perform rotation to account for to account for latitude
	VECTOR3 xbody2 = (cos(-latitude) * zbody1) - (sin(-latitude) * xbody1);
	VECTOR3 zbody2 = (cos(-latitude) * xbody1) + (sin(-latitude) * zbody1);
	VECTOR3 ybody2 = ybody1;

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for yaw
	VECTOR3 xbody3 = (cos(target_yaw) * xbody2) + (sin(target_yaw) * ybody2);
	VECTOR3 zbody3 = zbody2;
	VECTOR3 ybody3 = (cos(target_yaw) * ybody2) - (sin(target_yaw) * xbody2);

	//perform rotation to account for cam yaw
	VECTOR3 xbody4 = (cos(cam_yaw) * xbody3) + (sin(cam_yaw) * ybody3);
	VECTOR3 zbody4 = zbody3;
	VECTOR3 ybody4 = (cos(cam_yaw) * ybody3) - (sin(cam_yaw) * xbody3);

	//perform rotation to account for cam tilt
	VECTOR3 xbody5 = (cos(cam_tilt) * xbody4) + (sin(cam_tilt) * zbody4);
	VECTOR3 zbody5 = (cos(cam_tilt) * zbody4) - (sin(cam_tilt) * xbody4);
	VECTOR3 ybody5 = ybody4;

	VECTOR3 base = VECTOR3(0,0,0);

	base = cam_dist * xbody5;

	VECTOR3 pos = VECTOR3(0,0,0);
	//
	//pos = xbody3a*cos(latitude)*cos(3.14159 +  longitude) - ybody3a*cos(latitude)*sin(3.14159 +  longitude) + zbody3a*sin(latitude);
	//pos = pos * r;

	pDevice->lookat_vector = get_d3d_vector(pos);
	pDevice->eye_vector = get_d3d_vector(base + pos);
	pDevice->up_vector = get_d3d_vector(zbody5);

	pDevice->bLight0Changed = true;

	return 0;

}
//**************************************************************************************//
//**************************************************************************************//
int map_sm()
{
	hSharedMem = LoadLibraryA("Titan4DLL.dll");

	read_int = (_read_int_type)GetProcAddress(hSharedMem, "_read_int");
	write_int = (_write_int_type)GetProcAddress(hSharedMem, "_write_int");
	read_double = (_read_double_type)GetProcAddress(hSharedMem, "_read_double");
	write_double = (_write_double_type)GetProcAddress(hSharedMem, "_write_double");

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int update_coords_phase0()
{
	pStage2Main->longitude = read_double(PHASE0_LONGITUDE);
	pStage2Main->latitude = read_double(PHASE0_LATITUDE);
	pStage2Main->pr = read_double(PHASE0_R) * 100.0; 
	
	pStage2Main->pitch = read_double(PHASE0_PITCH) * -1;
	pStage2Main->roll = read_double(PHASE0_ROLL);
	pStage2Main->yaw = read_double(PHASE0_YAW);

	//TEMP HACK:
	//this is to reverse the definition of yaw
	// 0 heading = 90 yaw
	// 90 heading = 0 yaw
	// 180 heading = -90 yaw
	// 270 heading = -180 yaw
	pStage2Main->yaw = pStage2Main->yaw + 90.0*(3.14159/180.0);

	pStage1Nozzle->pitch = read_double(STAGE1_ENGINE_GIMBAL_TILT); 
	pStage1Nozzle->yaw  = read_double(STAGE1_ENGINE_GIMBAL_ROLL); 

	//set history buffer
	pStage2Nozzle->pHistoryBuffer->bEnable = false;
	pStage1Nozzle->pHistoryBuffer->bEnable = true;


	//the number of degrees of long/lat per pixel is
	double deg_to_pixel_longitude = 640.0 / 360.0;
	double deg_to_pixel_latitude = (480.0 / 180.0) * -1;

	//2D components
	pStage2_2D->px = read_double(PHASE0_LONGITUDE) * RAD_TO_DEG * deg_to_pixel_longitude;
	pStage2_2D->py = read_double(PHASE0_LATITUDE) * RAD_TO_DEG * deg_to_pixel_latitude;

	pStage1_2D->px = pStage2_2D->px;
	pStage1_2D->py = pStage2_2D->py;

	//set history buffer for 2D
	pStage2_2D->pHistoryBuffer->bEnable = false;
	pStage1_2D->pHistoryBuffer->bEnable = true;

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int update_coords_phase1()
{
	//first stage
	pShip->longitude = read_double(PHASE1_STAGE1_LONG);
	pShip->latitude = read_double(PHASE1_STAGE1_LAT);
	pShip->pr = read_double(PHASE1_STAGE1_R) * 100.0;

	pShip->pitch = read_double(PHASE1_STAGE1_PITCH) * -1;
	pShip->roll = read_double(PHASE1_STAGE1_ROLL);
	pShip->yaw = read_double(PHASE1_STAGE1_YAW);

	//TEMP HACK:
	//this is to reverse the definition of yaw
	// 0 heading = 90 yaw
	// 90 heading = 0 yaw
	// 180 heading = -90 yaw
	// 270 heading = -180 yaw
	pShip->yaw = pShip->yaw + 90.0*(3.14159/180.0);

	pStage1Nozzle->pitch = read_double(STAGE1_ENGINE_GIMBAL_TILT); 
	pStage1Nozzle->yaw  = read_double(STAGE1_ENGINE_GIMBAL_ROLL); 

	//second stage
	pStage2Main->longitude = read_double(PHASE1_STAGE2_LONG);
	pStage2Main->latitude = read_double(PHASE1_STAGE2_LAT);
	pStage2Main->pr = read_double(PHASE1_STAGE2_R) * 100.0;

	pStage2Main->pitch = read_double(PHASE1_STAGE2_PITCH) * -1;
	pStage2Main->roll = read_double(PHASE1_STAGE2_ROLL);
	pStage2Main->yaw = read_double(PHASE1_STAGE2_YAW);

	//TEMP HACK:
	//this is to reverse the definition of yaw
	// 0 heading = 90 yaw
	// 90 heading = 0 yaw
	// 180 heading = -90 yaw
	// 270 heading = -180 yaw
	pStage2Main->yaw = pStage2Main->yaw + 90.0*(3.14159/180.0);

	pStage2Nozzle->pitch = read_double(STAGE2_ENGINE_GIMBAL_TILT); 
	pStage2Nozzle->yaw  = read_double(STAGE2_ENGINE_GIMBAL_ROLL); 

	//interstage
	pInterstage->longitude = read_double(PHASE1_INTERSTAGE_LONG);
	pInterstage->latitude = read_double(PHASE1_INTERSTAGE_LAT);
	pInterstage->pr = read_double(PHASE1_INTERSTAGE_R) * 100.0;

	pInterstage->pitch = read_double(PHASE1_INTERSTAGE_PITCH) * -1;
	pInterstage->roll = read_double(PHASE1_INTERSTAGE_ROLL);
	pInterstage->yaw = read_double(PHASE1_INTERSTAGE_YAW);

	//TEMP HACK:
	//this is to reverse the definition of yaw
	// 0 heading = 90 yaw
	// 90 heading = 0 yaw
	// 180 heading = -90 yaw
	// 270 heading = -180 yaw
	pInterstage->yaw = pInterstage->yaw + 90.0*(3.14159/180.0);

	//the number of degrees of long/lat per pixel is
	double deg_to_pixel_longitude = 640.0 / 360.0;
	double deg_to_pixel_latitude = (480.0 / 180.0) * -1;

	//2D components
	pStage2_2D->px = read_double(PHASE1_STAGE2_LONG) * RAD_TO_DEG * deg_to_pixel_longitude;
	pStage2_2D->py = read_double(PHASE1_STAGE2_LAT) * RAD_TO_DEG * deg_to_pixel_latitude;

	pStage1_2D->px = read_double(PHASE1_STAGE1_LONG) * RAD_TO_DEG * deg_to_pixel_longitude;
	pStage1_2D->py = read_double(PHASE1_STAGE1_LAT) * RAD_TO_DEG * deg_to_pixel_latitude;

	//set history buffer
	pStage2Nozzle->pHistoryBuffer->bEnable = true;
	pStage1Nozzle->pHistoryBuffer->bEnable = true;

	//set history buffer for 2D
	pStage2_2D->pHistoryBuffer->bEnable = true;
	pStage1_2D->pHistoryBuffer->bEnable = true;

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int first_stage_separation()
{


	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int set_components_phase0()
{
	/////////////////////////////////////////////////////////////////////////

	//set relationships such that everything is a single group
	pEarth = D3D.Devices[0].RenderList;
	
	pEarth->pNextSibling = NULL;
	pEarth->pFirstChild = pStage2Main;

	pStage2Main->pNextSibling = NULL;//pPayload;
	pStage2Main->pFirstChild = pShip;


	pShip->pNextSibling = pStage1Nozzle;
	pShip->pFirstChild = NULL;

	pStage1Nozzle->pNextSibling = pInterstage;
	pStage1Nozzle->pFirstChild = NULL;
	
	pInterstage->pNextSibling = pStage2Nozzle;
	pInterstage->pFirstChild = NULL;

	pStage2Nozzle->pNextSibling = pPayload;
	pStage2Nozzle->pFirstChild = NULL;

	pPayload->pNextSibling = NULL;
	pPayload->pFirstChild = NULL;

	///////////////////////////////////////////////////////////////////////

	pEarth->iOrientationMode = ORIENTATION_MODE_PITCHROLLYAW;

	pStage2Main->latitude = 0;
	pStage2Main->longitude = 0;
	pStage2Main->pr = 105 * SHIP_SCALE;
	pStage2Main->iOrientationMode = ORIENTATION_MODE_LATLONGPR | ORIENTATION_MODE_PITCHROLLYAW;

	pShip->px = 0;//-8.5 * 0.8965 * SHIP_SCALE;
	pShip->py = 0.0;
	pShip->pz = 0.0;
	pShip->pitch = 0;
	pShip->roll = 0;
	pShip->yaw = 0;
	pShip->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

	pStage1Nozzle->px = -8.5 * 0.8965 * SHIP_SCALE;
	pStage1Nozzle->py = 0.0;
	pStage1Nozzle->pz = 0.0;
	pStage1Nozzle->pitch = 0;
	pStage1Nozzle->roll = 0;
	pStage1Nozzle->yaw = 0;
	pStage1Nozzle->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

	pInterstage->px = 3.1 * 0.8965 * SHIP_SCALE;
	pInterstage->py = 0.0;
	pInterstage->pz = 0.0;
	pInterstage->pitch = 0;
	pInterstage->roll = 0;
	pInterstage->yaw = 0;
	pInterstage->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

	pStage2Nozzle->pitch = 0;
	pStage2Nozzle->px = 6.1 * 0.8965 * SHIP_SCALE;
	pStage2Nozzle->py = 0.0;
	pStage2Nozzle->pz = 0.0;
	pStage2Nozzle->pitch = 0;
	pStage2Nozzle->roll = 0;
	pStage2Nozzle->yaw = 0;
	pStage2Nozzle->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

/*
	pStage2Main->pitch = 0;
	pStage2Main->px = 9.1 * 0.8965 * SHIP_SCALE;
	pStage2Main->py = 0.0;
	pStage2Main->pz = 0.0;
	pStage2Main->pitch = 0;
	pStage2Main->roll = 0;
	pStage2Main->yaw = 0;
	pStage2Main->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;
*/
	pPayload->pitch = 0;
	pPayload->px = 10.3 * 0.8965 * SHIP_SCALE;
	pPayload->py = 0.0;
	pPayload->pz = 0.0;
	pPayload->pitch = 0;
	pPayload->roll = 0;
	pPayload->yaw = 0;
	pPayload->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

	//this offsets everything
	pPayload->px -= 9.1 * 0.8965 * SHIP_SCALE;
	//pStage2Main->px -= 9.1 * 0.8965 * SHIP_SCALE;
	pStage2Nozzle->px -= 9.1 * 0.8965 * SHIP_SCALE;
	pInterstage->px -= 9.1 * 0.8965 * SHIP_SCALE;
	pStage1Nozzle->px -= 9.1 * 0.8965 * SHIP_SCALE;
	pShip->px -= 9.1 * 0.8965 * SHIP_SCALE;

/////////////////////
		//now do the same for the 2D map
		pMap2D = D3D.Devices[1].RenderList;
		pMap2D->pNextSibling = NULL;
		pMap2D->pFirstChild = pStage2_2D;

		pStage2_2D->pNextSibling = pStage1_2D;
		pStage2_2D->pFirstChild = NULL;

		pStage1_2D->pNextSibling = NULL;
		pStage1_2D->pFirstChild = NULL;


	component_setup = 0;

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int set_components_phase1()
{

	/////////////////////////////////////////////////////////////////////////

	//set relationships such that everything into three groups, Stage1, Interstage and Stage2
	pEarth = D3D.Devices[0].RenderList;
	
	pEarth->pNextSibling = NULL;
	pEarth->pFirstChild = pShip;

	//top-level
	pShip->pNextSibling = pInterstage;
	pShip->pFirstChild = pStage1Nozzle;

		pStage1Nozzle->pNextSibling = NULL;
		pStage1Nozzle->pFirstChild = NULL;
	
	//top-level
	pInterstage->pNextSibling = pStage2Main;
	pInterstage->pFirstChild = NULL;

	//top-level
	pStage2Main->pNextSibling = NULL;
	pStage2Main->pFirstChild = pStage2Nozzle;

		pStage2Nozzle->pNextSibling = pPayload;
		pStage2Nozzle->pFirstChild = NULL;

		pPayload->pNextSibling = NULL;
		pPayload->pFirstChild = NULL;

	///////////////////////////////////////////////////////////////////////

	pEarth->iOrientationMode = ORIENTATION_MODE_PITCHROLLYAW;
	
	pShip->latitude = 0;
	pShip->longitude = 0;
	pShip->pr = 105 * SHIP_SCALE;
	pShip->iOrientationMode = ORIENTATION_MODE_LATLONGPR | ORIENTATION_MODE_PITCHROLLYAW;

		pStage1Nozzle->pitch = 0.45;
		pStage1Nozzle->px = -8.5 * 0.8965 * SHIP_SCALE;
		pStage1Nozzle->py = 0.0;
		pStage1Nozzle->pz = 0.0;
		pStage1Nozzle->pitch = 0;
		pStage1Nozzle->roll = 0;
		pStage1Nozzle->yaw = 0;
		pStage1Nozzle->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

	pInterstage->latitude = 0;
	pInterstage->longitude = 0;
	pInterstage->pr = 105 * SHIP_SCALE;
	pInterstage->iOrientationMode = ORIENTATION_MODE_LATLONGPR | ORIENTATION_MODE_PITCHROLLYAW;

	pStage2Main->latitude = 0;
	pStage2Main->longitude = 0;
	pStage2Main->pr = 105 * SHIP_SCALE;
	pStage2Main->iOrientationMode = ORIENTATION_MODE_LATLONGPR | ORIENTATION_MODE_PITCHROLLYAW;

		pStage2Nozzle->pitch = 0;
		pStage2Nozzle->px = (-3.0) * 0.8965 * SHIP_SCALE;
		pStage2Nozzle->py = 0.0;
		pStage2Nozzle->pz = 0.0;
		pStage2Nozzle->pitch = 0;
		pStage2Nozzle->roll = 0;
		pStage2Nozzle->yaw = 0;
		pStage2Nozzle->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

		pPayload->pitch = 0;
		pPayload->px = 1.2 * 0.8965 * SHIP_SCALE;
		pPayload->py = 0.0;
		pPayload->pz = 0.0;
		pPayload->pitch = 0;
		pPayload->roll = 0;
		pPayload->yaw = 0;
		pPayload->iOrientationMode = ORIENTATION_MODE_BODYXYZ | ORIENTATION_MODE_PITCHROLLYAW;

/////////////////////
		//now do the same for the 2D map
		pMap2D = D3D.Devices[1].RenderList;
		pMap2D->pNextSibling = NULL;
		pMap2D->pFirstChild = pStage1_2D;

		pStage1_2D->pNextSibling = pStage2_2D;
		pStage1_2D->pFirstChild = NULL;

		pStage2_2D->pNextSibling = NULL;
		pStage2_2D->pFirstChild = NULL;

	component_setup = 1;

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
//this function calculates 


//**************************************************************************************//
//**************************************************************************************//
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	WindowManager.init(hInstance);
	WindowManager.create_window(0, L"Class1", L"Main Application Window", WndCallBackProc0);
	WindowManager.create_window(1, L"Class2", L"Secondary Application Window", WndCallBackProc1);

	D3D.init();

	D3D.load_window_3d(0, WindowManager.hWindows[0], 640, 480);
	D3D.load_window_2d(1, WindowManager.hWindows[1], 640, 480);

	D3D.Devices[0].init_views0();
	D3D.Devices[1].init_views0();

	D3D.Devices[0].init_lights0();
	D3D.Devices[1].init_lights1();

	Loader.init();


	D3D.Devices[1].load_with_sprite(D3D.Devices[1].RenderList, square2d, 4, L"infile.png");
	pMap2D = D3D.Devices[1].RenderList;
	
	pStage2_2D = pMap2D->add_child();
	D3D.Devices[1].load_with_sprite(pStage2_2D, sp2d, 4, L"stage2.png");
	D3D.Devices[1].add_history_buffer(pStage2_2D, false, D3DXCOLOR(0,0,0,0));
	pStage2_2D->set_name("stage2_2D");
	pStage2_2D->bFixedScale2D = true;
	

	pStage1_2D = pMap2D->add_child();
	D3D.Devices[1].load_with_sprite(pStage1_2D, sp2d, 4, L"stage1.png");
	pStage1_2D->set_name("stage1_2D");
	D3D.Devices[1].add_history_buffer(pStage1_2D, true, D3DXCOLOR(0,0,0,0));
	pStage1_2D->bFixedScale2D = true;

	Loader.load_obj_model("globe.objx", 637810.0);
	D3D.Devices[0].load_with_model(D3D.Devices[0].RenderList, Loader.FinalVertexes, Loader.iNumFinalVertexes, L"infile.png");
	pEarth = D3D.Devices[0].RenderList;
	pEarth->set_name("globe");

	Loader.load_obj_model("stage1_main.objx", SHIP_SCALE);
	pShip = D3D.Devices[0].RenderList->add_child();

	D3D.Devices[0].load_with_model(pShip, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pShip);
	pShip->set_name("stage1_main");

	pStage1Nozzle = pShip->add_child();
	Loader.load_obj_model("stage1_nozzle.objx", SHIP_SCALE);
	D3D.Devices[0].load_with_model(pStage1Nozzle, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pStage1Nozzle);
	pStage1Nozzle->set_name("stage1_nozzle");
	D3D.Devices[0].add_history_buffer(pStage1Nozzle, true, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	pInterstage = pShip->add_child();
	Loader.load_obj_model("interstage.objx", SHIP_SCALE);
	D3D.Devices[0].load_with_model(pInterstage, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pInterstage);
	pInterstage->set_name("interstage");

	pStage2Nozzle = pShip->add_child();
	Loader.load_obj_model("stage2_nozzle.objx", SHIP_SCALE * 2.0);
	D3D.Devices[0].load_with_model(pStage2Nozzle, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pStage2Nozzle);
	pStage2Nozzle->set_name("stage2_nozzle");
	D3D.Devices[0].add_history_buffer(pStage2Nozzle, false, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	pStage2Main = pShip->add_child();
	Loader.load_obj_model("stage2_main.objx", SHIP_SCALE);
	D3D.Devices[0].load_with_model(pStage2Main, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pStage2Main);
	pStage2Main->set_name("stage2_main");

	pPayload = pShip->add_child();
	Loader.load_obj_model("payload.objx", SHIP_SCALE);
	D3D.Devices[0].load_with_model(pPayload, Loader.FinalVertexes, Loader.iNumFinalVertexes, NULL);
	D3D.Devices[0].reverse_normals(pPayload);
	pPayload->set_name("payload");

	D3D.Devices[0].follow_on();
	D3D.Devices[1].follow_on();

	set_components_phase0();

	MSG WinMsg;

	map_sm();

	int count = 0;

	camera_target = 0;
	D3D.Devices[0].cam_dist = 18.0;

	cRenderObject* object_to_follow = pShip;

	//DEBUG
	write_double(PHASE0_R, 6379.0);
	write_double(PHASE0_LONGITUDE, 0);
	write_double(PHASE0_LATITUDE,  0);

	//DEBUG
	pMap2D->px = 320;
	pMap2D->py = 240;

	//init scaling factor
	input_scale = 1.0;

	initKinect();

	while (WindowManager.bRunFlag == true)
	{
			processKinect();

			count++;
			write_int(0x100, count);

			//get command
			int command = read_int(VIS_COMMAND);
			write_int(VIS_COMMAND, 0);
			if (command == 2)
			{
				pStage1Nozzle->pHistoryBuffer->clear();
				pStage2Nozzle->pHistoryBuffer->clear();
				pStage1_2D->pHistoryBuffer->clear();
				pStage2_2D->pHistoryBuffer->clear();
			}
			//quit
			if (command == 3)
				break;
			
			//update location
			//update coordinates based on phase
			int sim_phase = read_int(SIM_PHASE);
			double sim_time = read_double(SIM_TIME);

			if (sim_phase == 0)
			{
				if (component_setup != 0)
					set_components_phase0();

				update_coords_phase0();
				object_to_follow = pStage2Main;
			}
			if (sim_phase == 1)
			{
				if (component_setup != 1)
					set_components_phase1();

				update_coords_phase1();
				object_to_follow = pStage2Main;
			}

			if (camera_target == 0)
				calc_views_targetfixed(&D3D.Devices[0], object_to_follow->pr, object_to_follow->longitude, object_to_follow->latitude, D3D.Devices[0].cam_dist, D3D.Devices[0].cam_tilt, D3D.Devices[0].cam_yaw, object_to_follow->pitch, object_to_follow->roll, object_to_follow->yaw, 0);
			if (camera_target == 1)
				calc_views_earthfixed(&D3D.Devices[0], D3D.Devices[0].cam_dist, D3D.Devices[0].cam_tilt, D3D.Devices[0].cam_yaw);

			VECTOR3 pos = VECTOR3(0,0,0);
			//the original basis vectors
			VECTOR3 xbody0 = VECTOR3(1,0,0);
			VECTOR3 ybody0 = VECTOR3(0,1,0);
			VECTOR3 zbody0 = VECTOR3(0,0,1);
			pos = xbody0*cos(-object_to_follow->latitude)*cos(-object_to_follow->longitude) - ybody0*cos(-object_to_follow->latitude)*sin(-object_to_follow->longitude) + zbody0*sin(-object_to_follow->latitude);
			pos = pos * object_to_follow->pr;

			D3D.render(VECTOR3(-pos.x, -pos.y, -pos.z), VECTOR3(0, 0, 0), input_scale, sim_time);

			int iRet = PeekMessage(&WinMsg, NULL, 0, 0, PM_REMOVE);
			while (iRet != 0)
			{
				TranslateMessage(&WinMsg);
				DispatchMessage(&WinMsg);
				iRet = PeekMessage(&WinMsg, NULL, 0, 0, PM_REMOVE);
			} 

			Sleep(10);
	}
	write_int(SIM_COMMAND, 0);
	D3D.exit();

	exitKinect();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
LRESULT CALLBACK WndCallBackProc0(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
		{
		case WM_KEYDOWN:
			switch(wParam)
				{
				case VK_ESCAPE:
					WindowManager.bRunFlag = 0;
					break;
				case VK_UP:
					D3D.Devices[0].eye_tilt_up();
					break;
				case VK_DOWN:
					D3D.Devices[0].eye_tilt_down();
					break;
				case VK_LEFT:
					D3D.Devices[0].eye_tilt_left();
					break;
				case VK_RIGHT:
					D3D.Devices[0].eye_tilt_right();
					break;
				case VK_PRIOR:
					if (D3D.Devices[0].bFollow == false)
						D3D.Devices[0].eye_forward();
					else
						D3D.Devices[0].zoom_in();
					break;
				case VK_NEXT:
					if (D3D.Devices[0].bFollow == false)
						D3D.Devices[0].eye_back();
					else
						D3D.Devices[0].zoom_out();
					break;
				case VK_NUMPAD8:
					D3D.Devices[0].pan_up();
					break;				
				case VK_NUMPAD4:
					D3D.Devices[0].pan_left();
					break;				
				case VK_NUMPAD6:
					D3D.Devices[0].pan_right();
					break;				
				case VK_NUMPAD2:
					D3D.Devices[0].pan_down();
					break;		
				case VK_F1:
					//D3D.Devices[0].follow_on();
					camera_target = 0;
					D3D.Devices[0].cam_dist = 10.0;
					break;
				case VK_F2:
					//D3D.Devices[0].follow_off();
					camera_target = 1;
					D3D.Devices[0].cam_dist = 120.0;
					break;
				}
			break;
		case WM_CLOSE:
			WindowManager.bRunFlag = 0;
		break;
		}
	  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//**************************************************************************************//
//**************************************************************************************//
LRESULT CALLBACK WndCallBackProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
		{
		case WM_KEYDOWN:
			switch(wParam)
				{
				case VK_ESCAPE:
					WindowManager.bRunFlag = 0;
					break;
				case VK_UP:
					//D3D.Devices[1].eye_tilt_up();
					pMap2D->py += (10.0/input_scale);
					//wrap around
					if (pMap2D->py > 480.0)
						pMap2D->py -= 480.0;
					break;
				case VK_DOWN:
					//D3D.Devices[1].eye_tilt_down();
					pMap2D->py -= (10.0/input_scale);
					//wrap around
					if (pMap2D->py < 0.0)
						pMap2D->py += 480.0;
					break;
				case VK_LEFT:
					//D3D.Devices[1].eye_tilt_left();
					pMap2D->px += (10.0/input_scale);
					//wrap around
					if (pMap2D->px > 640.0)
						pMap2D->px -= 640.0;
					break;
				case VK_RIGHT:
					//D3D.Devices[1].eye_tilt_right();
					pMap2D->px -= (10.0/input_scale);
					//wrap around
					if (pMap2D->px < 0.0)
						pMap2D->px += 640.0;
					break;
				case VK_PRIOR:
					if (input_scale < 20.0)
						input_scale *= 1.1;

					/*if (D3D.Devices[1].bFollow == false)
						D3D.Devices[1].eye_forward();
					else
						D3D.Devices[1].zoom_in(); */
					break;
				case VK_NEXT:
					if (input_scale > 1.0)
						input_scale *= 0.9;
					/*if (D3D.Devices[1].bFollow == false)
						D3D.Devices[1].eye_back();
					else
						D3D.Devices[1].zoom_out(); */

					break;
				case VK_NUMPAD8:
					D3D.Devices[1].pan_up();
					break;				
				case VK_NUMPAD4:
					D3D.Devices[1].pan_left();
					break;				
				case VK_NUMPAD6:
					D3D.Devices[1].pan_right();
					break;				
				case VK_NUMPAD2:
					D3D.Devices[1].pan_down();
					break;
				case VK_F1:
					//D3D.Devices[1].follow_on();
					camera_target = 0;
					break;
				case VK_F2:
					//D3D.Devices[1].follow_off();
					camera_target = 1;
					break;
				}
			break;
		case WM_CLOSE:
			WindowManager.bRunFlag = 0;
		break;
		}
	  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//**************************************************************************************//
//**************************************************************************************//