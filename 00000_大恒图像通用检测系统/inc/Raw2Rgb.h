/*
 *-----------------------------------------------------------------------------
 *       Name				Raw2Rgb.h
 *       Purpose		
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Hujun
 *       Date          		2004/02/26
 *
 *
 *       Description
 *		
 *       
 *-----------------------------------------------------------------------------
 */


#ifndef __RAW_TO_RGB
#define __RAW_TO_RGB



/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
   	


typedef enum tagHV_BAYER_CONVERT_TYPE
{
	BAYER2RGB_ZHC,  
	BAYER2RGB_NEIGHBOUR,  
	BAYER2RGB_BILINER,    
	BAYER2RGB_PATTERN,       
	BAYER2RGB_EDGESENSING,  
	BAYER2RGB_RF,          
	BAYER2RGB_PIXELGROUPING, 
	BAYER2RGB_RESERVERED1,	 
	BAYER2RGB_RESERVERED2,	 
	BAYER2RGB_RESERVERED3	
	
}HV_BAYER_CONVERT_TYPE;


typedef enum tagHV_BAYER_LAYOUT
{
	BAYER_GB =1,   // start from GB
	BAYER_GR =2,    // start from GR
	BAYER_BG =3,    // start from BG
	BAYER_RG =4   // start from RG
}HV_BAYER_LAYOUT;




void __stdcall ConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,
							  BYTE  pLutR[256],  BYTE  pLutG[256],  BYTE  pLutB[256],bool bIsFlip, HV_BAYER_LAYOUT Layout);

void __stdcall GetWhiteBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);



/* extren "C" { */
#ifdef __cplusplus
}

#endif


#endif

