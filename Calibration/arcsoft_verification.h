// arcsoft_modulecheck.h

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

#ifndef _ARCSOFT_MODULE_CHECK_
#define _ARCSOFT_MODULE_CHECK_

#include "asvloffscreen.h"
#include "merror.h"
#include "amcomdef.h"
//#include "arcsoft_modulecommondef.h"

#ifdef ARCSOFT_MODULECHECK_EXPORTS
#define MODULECHECK_API		__declspec(dllexport)
#else
#define MODULECHECK_API		__declspec(dllimport)
#endif

#ifdef   __cplusplus
extern "C"{                                               
#endif

#define MERR_MC_SDK_CHECK_ERROR			    0X090FF1

#define VERIFY_RES_SIZE                     3
#define VERIFY_INDEX_AVERAGE_ERR            0
#define VERIFY_INDEX_MAX_ERR                1
#define VERIFY_INDEX_ERR_RANGE              2

typedef struct _tag_ArcModuleCheckVersion
{
    MDWord lCodebase;		/**< platform dependent       */
    MDWord lMajor;			/**< major version            */ 
    MDWord lMinor;			/**< minor version            */
    MDWord lBuild;			/**< increasable only         */
    MPChar Version;			/**< version in string format */
    MPChar BuildDate;		/**< latest build Date        */
    MPChar CopyRight;		/**< copyright                */
} ArcVerificationVersion;

typedef struct _ArcStereoImageData
{
	ASVLOFFSCREEN *leftImg;		//Main camera image
	ASVLOFFSCREEN *rightImg;	//Auxiliary camera image
	int chessboardWidth;//number of blocks in the direction of x axis
	int chessboardHeight;// number of blocks in the direction of y axis.
	char* leftImgPath;		//Main camera image path
	char* rightImgPath;		//Auxiliary camera image path
}ArcStereoImageData;

typedef MHandle	MC_ENGINE;

MODULECHECK_API MC_ENGINE		MC_CreateEngine();
MODULECHECK_API MVoid			MC_DestroyEngine(MC_ENGINE hEngine);
MODULECHECK_API	MRESULT			MC_ModuleVerification(MC_ENGINE hEngine, 
													ArcStereoImageData *pImages, 
													MByte* allPara,
													MDouble verifyResult[VERIFY_RES_SIZE],
													MChar* lpszDeviceID
													);

MODULECHECK_API const ArcVerificationVersion*   MC_GetVersion(); 

#ifdef   __cplusplus
}                                               
#endif

#endif