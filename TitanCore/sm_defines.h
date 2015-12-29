#pragma once

#define ENGINE_EXTERNAL_CONTROL	0x70

#define SIM_STEP_SIZE			0x80

#define SIM_COMMAND			0x90

#define VIS_COUNT			0x100


#define PHASE0_LONGITUDE	0x110	//radians
#define PHASE0_LATITUDE		0x120	//radians
#define PHASE0_R			0x130	//meters

#define PHASE0_PITCH		0x140	//radians
#define PHASE0_ROLL			0x150	//radians
#define PHASE0_YAW			0x160	//radians

#define SIM_TIME			0x170   //seconds

#define STAGE1_ENGINE_GIMBAL_ROLL  0x180   //radians
#define STAGE1_ENGINE_GIMBAL_TILT  0x190   //radians
		

#define PHASE0_X			0x200	//meters (ecef)
#define PHASE0_Y			0x210	//meters (ecef)
#define PHASE0_Z			0x220	//meters (ecef)

#define PHASE0_VX			0x230	//meters/sec 
#define PHASE0_VY			0x240	//meters/sec
#define PHASE0_VZ			0x250	//meters/sec

#define PHASE0_WX			0x260	//rad/sec
#define PHASE0_WY			0x270	//rad/sec
#define PHASE0_WZ			0x280	//rad/sec

//these params are used for loading into the integrator
#define THRUST_FX			0x290	//newtons (ecef)
#define THRUST_FY			0x300
#define THRUST_FZ			0x310
//these params are used for loading into the integrator
#define THRUST_MX			0x320	//newton-meters (body)
#define THRUST_MY			0x330
#define THRUST_MZ			0x340

#define THRUST_EFFECT_DMASS	0x350
#define THRUST_EFFECT_DIXX	0x360
#define THRUST_EFFECT_DIYY  0x370
#define THRUST_EFFECT_DIZZ	0x380

#define SIM_PHASE			0x390

#define VIS_COMMAND			0x400

#define STAGE2_ENGINE_GIMBAL_ROLL  0x410   //radians
#define STAGE2_ENGINE_GIMBAL_TILT  0x420   //radians

#define PHASE0_AERODYN_FX	0x440
#define PHASE0_AERODYN_FY	0x450
#define PHASE0_AERODYN_FZ	0x460

#define PHASE0_AERODYN_MX	0x470
#define PHASE0_AERODYN_MY	0x480
#define PHASE0_AERODYN_MZ	0x490

#define PHASE0_AIR_DENSITY	0x4a0
#define PHASE0_STATIC_AIR_TEMPERATURE	0x4b0
#define PHASE0_STATIC_PRESSURE 0x4c0

#define PHASE0_ANGLE_OF_ATTACK	0x4d0
#define PHASE0_CD				0x4e0
#define PHASE0_CD0				0x4f0
#define PHASE0_RHO				0x500

#define PHASE0_Q1				0x510
#define PHASE0_Q2				0x520
#define PHASE0_Q3				0x530
#define PHASE0_Q4				0x540

#define PHASE0_MASS				0x550
#define PHASE0_FLIGHT_PATH_ANGLE	0x560

#define PHASE0_IXX				0x570
#define PHASE0_IYY				0x580
#define PHASE0_IZZ				0x590

#define PHASE1_STAGE1_X		0x1000
#define PHASE1_STAGE1_Y		0x1010
#define PHASE1_STAGE1_Z		0x1020

#define PHASE1_STAGE1_VX	0x1030
#define PHASE1_STAGE1_VY	0x1040
#define PHASE1_STAGE1_VZ	0x1050

#define PHASE1_STAGE1_PITCH	0x1060
#define PHASE1_STAGE1_ROLL	0x1070
#define PHASE1_STAGE1_YAW	0x1080

#define PHASE1_STAGE1_WX	0x1090
#define PHASE1_STAGE1_WY	0x10a0
#define PHASE1_STAGE1_WZ	0x10b0

#define PHASE1_STAGE1_R		0x10c0
#define PHASE1_STAGE1_LONG	0x10d0
#define PHASE1_STAGE1_LAT	0x10e0

#define PHASE1_STAGE1_AERODYN_FX	0x10f0
#define PHASE1_STAGE1_AERODYN_FY	0x1100
#define PHASE1_STAGE1_AERODYN_FZ	0x1110

#define PHASE1_STAGE1_AERODYN_MX	0x1120
#define PHASE1_STAGE1_AERODYN_MY	0x1130
#define PHASE1_STAGE1_AERODYN_MZ	0x1140

#define PHASE1_STAGE1_AIR_DENSITY				0x1150
#define PHASE1_STAGE1_STATIC_AIR_TEMPERATURE	0x1160
#define PHASE1_STAGE1_STATIC_PRESSURE			0x1170

#define PHASE1_STAGE1_ANGLE_OF_ATTACK	0x1180
#define PHASE1_STAGE1_CD				0x1190
#define PHASE1_STAGE1_CD0				0x11a0
#define PHASE1_STAGE1_RHO				0x11b0

#define PHASE1_STAGE1_Q1				0x11c0
#define PHASE1_STAGE1_Q2				0x11d0
#define PHASE1_STAGE1_Q3				0x11e0
#define PHASE1_STAGE1_Q4				0x11f0

#define PHASE1_STAGE1_MASS				0x1200

#define PHASE1_STAGE1_FLIGHT_PATH_ANGLE		0x1210

#define PHASE1_STAGE1_IXX		0x1220
#define PHASE1_STAGE1_IYY		0x1230
#define PHASE1_STAGE1_IZZ		0x1240

#define PHASE1_INTERSTAGE_X		0x2000
#define PHASE1_INTERSTAGE_Y		0x2010
#define PHASE1_INTERSTAGE_Z		0x2020

#define PHASE1_INTERSTAGE_VX	0x2030
#define PHASE1_INTERSTAGE_VY	0x2040
#define PHASE1_INTERSTAGE_VZ	0x2050

#define PHASE1_INTERSTAGE_PITCH	0x2060
#define PHASE1_INTERSTAGE_ROLL	0x2070
#define PHASE1_INTERSTAGE_YAW	0x2080

#define PHASE1_INTERSTAGE_WX	0x2090
#define PHASE1_INTERSTAGE_WY	0x20a0
#define PHASE1_INTERSTAGE_WZ	0x20b0

#define PHASE1_INTERSTAGE_R		0x20c0
#define PHASE1_INTERSTAGE_LONG	0x20d0
#define PHASE1_INTERSTAGE_LAT	0x20e0

#define PHASE1_INTERSTAGE_AERODYN_FX	0x20f0
#define PHASE1_INTERSTAGE_AERODYN_FY	0x2100
#define PHASE1_INTERSTAGE_AERODYN_FZ	0x2110

#define PHASE1_INTERSTAGE_AERODYN_MX	0x2120
#define PHASE1_INTERSTAGE_AERODYN_MY	0x2130
#define PHASE1_INTERSTAGE_AERODYN_MZ	0x2140

#define PHASE1_INTERSTAGE_AIR_DENSITY				0x2150
#define PHASE1_INTERSTAGE_STATIC_AIR_TEMPERATURE	0x2160
#define PHASE1_INTERSTAGE_STATIC_PRESSURE			0x2170

#define PHASE1_INTERSTAGE_ANGLE_OF_ATTACK	0x2180
#define PHASE1_INTERSTAGE_CD				0x2190
#define PHASE1_INTERSTAGE_CD0				0x21a0
#define PHASE1_INTERSTAGE_RHO				0x21b0

#define PHASE1_INTERSTAGE_Q1				0x21c0
#define PHASE1_INTERSTAGE_Q2				0x21d0
#define PHASE1_INTERSTAGE_Q3				0x21e0
#define PHASE1_INTERSTAGE_Q4				0x21f0

#define PHASE1_INTERSTAGE_MASS				0x2200

#define PHASE1_INTERSTAGE_FLIGHT_PATH_ANGLE		0x2210

#define PHASE1_INTERSTAGE_IXX		0x2220
#define PHASE1_INTERSTAGE_IYY		0x2230
#define PHASE1_INTERSTAGE_IZZ		0x2240

#define PHASE1_STAGE2_X		0x3000
#define PHASE1_STAGE2_Y		0x3010
#define PHASE1_STAGE2_Z		0x3020

#define PHASE1_STAGE2_VX	0x3030
#define PHASE1_STAGE2_VY	0x3040
#define PHASE1_STAGE2_VZ	0x3050

#define PHASE1_STAGE2_PITCH	0x3060
#define PHASE1_STAGE2_ROLL	0x3070
#define PHASE1_STAGE2_YAW	0x3080

#define PHASE1_STAGE2_WX	0x3090
#define PHASE1_STAGE2_WY	0x30a0
#define PHASE1_STAGE2_WZ	0x30b0

#define PHASE1_STAGE2_R		0x30c0
#define PHASE1_STAGE2_LONG	0x30d0
#define PHASE1_STAGE2_LAT	0x30e0

#define PHASE1_STAGE2_AERODYN_FX	0x30f0
#define PHASE1_STAGE2_AERODYN_FY	0x3100
#define PHASE1_STAGE2_AERODYN_FZ	0x3110

#define PHASE1_STAGE2_AERODYN_MX	0x3120
#define PHASE1_STAGE2_AERODYN_MY	0x3130
#define PHASE1_STAGE2_AERODYN_MZ	0x3140

#define PHASE1_STAGE2_AIR_DENSITY				0x3150
#define PHASE1_STAGE2_STATIC_AIR_TEMPERATURE	0x3160
#define PHASE1_STAGE2_STATIC_PRESSURE			0x3170

#define PHASE1_STAGE2_ANGLE_OF_ATTACK	0x3180
#define PHASE1_STAGE2_CD				0x3190
#define PHASE1_STAGE2_CD0				0x31a0
#define PHASE1_STAGE2_RHO				0x31b0

#define PHASE1_STAGE2_Q1				0x31c0
#define PHASE1_STAGE2_Q2				0x31d0
#define PHASE1_STAGE2_Q3				0x31e0
#define PHASE1_STAGE2_Q4				0x31f0

#define PHASE1_STAGE2_MASS				0x3200

#define PHASE1_STAGE2_FLIGHT_PATH_ANGLE		0x3210

#define PHASE1_STAGE2_IXX		0x3220
#define PHASE1_STAGE2_IYY		0x3230
#define PHASE1_STAGE2_IZZ		0x3240

//these params are used for loading into the integrator
#define AERODYN_FX		0x4000
#define AERODYN_FY		0x4010
#define AERODYN_FZ		0x4020

#define AERODYN_MX		0x4030
#define AERODYN_MY		0x4040
#define AERODYN_MZ		0x4050

//define initial positions
#define IC_PX_ECEF		0x5000
#define IC_PY_ECEF		0x5010
#define IC_PZ_ECEF		0x5020

#define IC_VX_ECEF		0x5030
#define IC_VY_ECEF		0x5040
#define IC_VZ_ECEF		0x5050

#define IC_PITCH_RAD	0x5060
#define IC_ROLL_RAD		0x5070
#define IC_YAW_RAD		0x5080

#define IC_WX			0x5090
#define IC_WY			0x50a0
#define IC_WZ			0x50b0

#define IC_MASS_FUEL_FIRST_STAGE		0x50c0
#define IC_IXX			0x50d0
#define IC_IYY			0x50e0
#define IC_IZZ			0x50f0

#define STAGE1_ENGINE_ON	0x5100
#define STAGE2_ENGINE_ON	0x5110

typedef int (__cdecl *_read_int_type) (int addr);
typedef double (__cdecl *_read_double_type) (int addr);
typedef int (__cdecl *_write_int_type) (int addr, int value);
typedef int (__cdecl *_write_double_type) (int addr, double value);
