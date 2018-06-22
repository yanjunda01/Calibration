// arcsoft_modulerectify.h

/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary 
and confidential information. 

The information and code contained in this file is only for authorized ArcSoft 
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER 
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy, 
distribute, modify, or take any action in reliance on it. 

If you have received this file in error, please immediately notify ArcSoft and 
permanently delete the original and any copy of any file and any printout 
thereof.
*******************************************************************************/

#ifndef _ARCSOFT_MODULE_RECTITY_
#define _ARCSOFT_MODULE_RECTITY_

#include "asvloffscreen.h"
#include "merror.h"
#include "amcomdef.h"
//#include "arcsoft_modulecommondef.h"

#ifdef ARCSOFT_MODULERECTIFY_EXPORTS
#define MODULERECTIFY_API __declspec(dllexport)
#else
#define MODULERECTIFY_API __declspec(dllimport)
#endif

#ifdef   __cplusplus
extern "C"{                                               
#endif


#define MERR_MR_SDK_CHECK_ERROR			        0X090FF1

#define RET_PARAMS_BASE							0XB000
#define RET_PARAMS_ROTATION_X					(RET_PARAMS_BASE+1)
#define RET_PARAMS_ROTATION_Y					(RET_PARAMS_BASE+2)
#define RET_PARAMS_ROTATION_Z					(RET_PARAMS_BASE+3)
#define RET_PARAMS_SHIFT_X					    (RET_PARAMS_BASE+4)
#define RET_PARAMS_SHIFT_Y					    (RET_PARAMS_BASE+5)
typedef struct _tag_ArcModuleRectifyVersion
{
    MDWord lCodebase;		/**< platform dependent       */
    MDWord lMajor;			/**< major version            */ 
    MDWord lMinor;			/**< minor version            */
    MDWord lBuild;			/**< increasable only         */
    MPChar Version;			/**< version in string format */
    MPChar BuildDate;		/**< latest build Date        */
    MPChar CopyRight;		/**< copyright                */
} ArcCalibrationVersion;

typedef struct _allCalibrationParameters
{
	ASVLOFFSCREEN *leftImg;		//Main camera image
	ASVLOFFSCREEN *rightImg;	//Auxiliary camera image

	MInt32 chessboardWidth;//number of blocks in the direction of x axis
	MInt32 chessboardHeight;// number of blocks in the direction of y axis.
	MInt32 numberOfChessboards;// total number of chessboards in this situation.
	MDouble blockSize; // physical size of block(mm) 
	char* leftImgPath;	//Main camera image path
	char* rightImgPath;	//Auxiliary camera image path
}ArcCalibrationParameters;


typedef MHandle	MR_ENGINE;

MODULERECTIFY_API MR_ENGINE		MR_CreateEngine();

MODULERECTIFY_API MVoid			MR_DestroyEngine(MR_ENGINE hEngine);

MODULERECTIFY_API MRESULT		MR_ModuleCalibration(MR_ENGINE hEngine,
											ArcCalibrationParameters* pParam,
											MByte* lpszOutputParam,
											MUInt32* pOutputParamLength,
											MChar* lpszDeviceID
											);
MODULERECTIFY_API MRESULT       MR_GetCalibrationResultParam(MR_ENGINE hEngine,MLong ltype,MDouble* pRetParam);
MODULERECTIFY_API const ArcCalibrationVersion*   MR_GetVersion();

//////////////////////////////////////////////////////////////////////////
//Helper API for reading parameters from calibration

MODULERECTIFY_API MRESULT	ArcCalibDataReaderInit(MByte* pCalibData, MUInt32 nCalibDataLen, MHandle* phReader);

MODULERECTIFY_API MRESULT   ArcCalibDataReaderGetParam(MHandle hReader,MLong lType,MDouble* pRetParam);

MODULERECTIFY_API MVoid		ArcCalibDataReaderUninit(MHandle hReader);
//////////////////////////////////////////////////////////////////////////

#ifdef   __cplusplus
}                                             
#endif

#endif