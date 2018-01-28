#pragma once
#pragma warning(disable : 4996)
#include <windows.h>
#include <shlwapi.h>

typedef J_STATUS_TYPE (*tJ_Factory_Open)(const int8_t* psPrivateData, FACTORY_HANDLE *pHandle);
typedef J_STATUS_TYPE (*tJ_Factory_Close)(FACTORY_HANDLE hFac);
typedef J_STATUS_TYPE (*tJ_Factory_GetInfo)(J_FACTORY_INFO info, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Factory_UpdateCameraList)(FACTORY_HANDLE hFac, bool8_t *bHasChanged);
typedef J_STATUS_TYPE (*tJ_Factory_GetNumOfCameras)(FACTORY_HANDLE hFac, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Factory_GetCameraIDByIndex)(FACTORY_HANDLE hFac, int32_t iNum, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Factory_GetCameraInfo)(FACTORY_HANDLE hFac, int8_t* pCameraId, J_CAMERA_INFO InfoId, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Factory_EnableForceIp)(FACTORY_HANDLE hFac, uint32_t ForceEnabled);
typedef J_STATUS_TYPE (*tJ_Camera_Open)(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam);
typedef J_STATUS_TYPE (*tJ_Camera_OpenMc)(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam, J_DEVICE_ACCESS_FLAGS iOpenFlags, uint32_t iMcIP);
typedef J_STATUS_TYPE (*tJ_Camera_Close)(CAM_HANDLE hCam);
typedef J_STATUS_TYPE (*tJ_Camera_GetValueInt64)(CAM_HANDLE hCam, int8_t* sNodeName, int64_t *pValue);
typedef J_STATUS_TYPE (*tJ_Camera_GetValueDouble)(CAM_HANDLE hCam, int8_t* sNodeName, double *pValue);
typedef J_STATUS_TYPE (*tJ_Camera_GetValueString)(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Camera_SetValueInt64)(CAM_HANDLE hCam, int8_t* sNodeName, int64_t Value);
typedef J_STATUS_TYPE (*tJ_Camera_SetValueDouble)(CAM_HANDLE hCam, int8_t* sNodeName, double Value);
typedef J_STATUS_TYPE (*tJ_Camera_SetValueString)(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr);
typedef J_STATUS_TYPE (*tJ_Camera_ExecuteCommand)(CAM_HANDLE hCam, int8_t* sNodeName);
typedef J_STATUS_TYPE (*tJ_Camera_ReadData)(CAM_HANDLE hCam, int64_t iAddress, void* pData, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Camera_WriteData)(CAM_HANDLE hCam, int64_t iAddress, const void* pData, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Camera_GetConfigInfo)(CAM_HANDLE hCam, J_CONFIG_INFO cinfo, int8_t* pBuffer, uint32_t* pSize); 
typedef J_STATUS_TYPE (*tJ_Camera_GetNumOfNodes)(CAM_HANDLE hCam, uint32_t* pNum);
typedef J_STATUS_TYPE (*tJ_Camera_GetNodeByIndex)(CAM_HANDLE hCam, uint32_t index, NODE_HANDLE* phNode);
typedef J_STATUS_TYPE (*tJ_Camera_GetNodeByName)(CAM_HANDLE hCam, int8_t* sNodeName, NODE_HANDLE * phNode);
typedef J_STATUS_TYPE (*tJ_Camera_GetNumOfSubFeatures)(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Camera_GetSubFeatureByIndex)(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t index, NODE_HANDLE * phNode);
typedef J_STATUS_TYPE (*tJ_Camera_GetFeatureByName)(CAM_HANDLE hCam, int8_t* sFeatureNodeName, NODE_HANDLE* phNode);
typedef J_STATUS_TYPE (*tJ_Camera_InvalidateNodes)(CAM_HANDLE hCam);
typedef J_STATUS_TYPE (*tJ_Camera_GetNumOfDataStreams)(CAM_HANDLE hCam, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Camera_CreateDataStream)(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS);
typedef J_STATUS_TYPE (*tJ_Camera_CreateDataStreamMc)(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS, DWORD iMcIP);
typedef J_STATUS_TYPE (*tJ_DataStream_AnnounceBuffer)(STREAM_HANDLE hDS, void *pBuffer, uint32_t uiSize, void *pPrivate, BUF_HANDLE *hBuf);
typedef J_STATUS_TYPE (*tJ_DataStream_FlushQueue)(STREAM_HANDLE hDS, J_ACQ_QUEUE_TYPE QueueType);
typedef J_STATUS_TYPE (*tJ_DataStream_StartAcquisition)(STREAM_HANDLE hDS, J_ACQ_START_FLAGS iFlags, uint64_t iNumImages);
typedef J_STATUS_TYPE (*tJ_DataStream_StopAcquisition)(STREAM_HANDLE hDS, J_ACQ_STOP_FLAGS iFlags);
typedef J_STATUS_TYPE (*tJ_DataStream_GetStreamInfo)(STREAM_HANDLE hDS, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_DataStream_GetBufferID)(STREAM_HANDLE hDS, uint32_t iIndex, BUF_HANDLE *hBuf);
typedef J_STATUS_TYPE (*tJ_DataStream_Close)(STREAM_HANDLE hDS);
typedef J_STATUS_TYPE (*tJ_DataStream_RevokeBuffer)(STREAM_HANDLE hDS, BUF_HANDLE hBuf, void **pBuffer, void **pPrivate);
typedef J_STATUS_TYPE (*tJ_DataStream_QueueBuffer)(STREAM_HANDLE hDS, BUF_HANDLE hBuf);
typedef J_STATUS_TYPE (*tJ_DataStream_GetBufferInfo)(STREAM_HANDLE hDS, BUF_HANDLE hBuf, J_BUFFER_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_DataStream_GetParam)(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_DataStream_SetParam)(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_Event_GetData)(EVT_HANDLE pEventHandle, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_Event_GetInfo)(EVT_HANDLE pEventHandle, J_EVENT_INFO_ID iID, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_Event_Flush)(EVT_HANDLE pEventHandle);
typedef J_STATUS_TYPE (*tJ_Event_Close)(EVT_HANDLE pEventHandle);
typedef J_STATUS_TYPE (*tJ_Camera_RegisterEvent)(CAM_HANDLE hCam, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle);
typedef J_STATUS_TYPE (*tJ_Camera_UnRegisterEvent)(CAM_HANDLE hCam, J_EVENT_TYPE iEventType);
typedef J_STATUS_TYPE (*tJ_DataStream_RegisterEvent)(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle);
typedef J_STATUS_TYPE (*tJ_DataStream_UnRegisterEvent)(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType);
typedef J_STATUS_TYPE (*tJ_Event_CreateCondition)(HANDLE* pConditionHandle);
typedef J_STATUS_TYPE (*tJ_Event_SignalCondition)(HANDLE conditionHandle);
typedef J_STATUS_TYPE (*tJ_Event_ExitCondition)(HANDLE conditionHandle);
typedef J_STATUS_TYPE (*tJ_Event_ResetCondition)(HANDLE conditionHandle);
typedef J_STATUS_TYPE (*tJ_Event_WaitForCondition)(HANDLE conditionHandle, DWORD timeout, J_COND_WAIT_RESULT* pWaitResult);
typedef J_STATUS_TYPE (*tJ_Event_WaitForMultipleConditions)(uint32_t nCount, const HANDLE* conditionHandles, DWORD timeout, J_COND_WAIT_RESULT* pWaitResult);
typedef J_STATUS_TYPE (*tJ_Event_CloseCondition)(HANDLE conditionHandle);
typedef J_STATUS_TYPE (*tJ_Node_GetAccessMode)(NODE_HANDLE hNode, J_NODE_ACCESSMODE *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetName)(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize, uint32_t FullQualified);
typedef J_STATUS_TYPE (*tJ_Node_GetNameSpace)(NODE_HANDLE hNode, J_NODE_NAMESPACE *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetVisibility)(NODE_HANDLE hNode, J_NODE_VISIBILITY* pVisibility);
typedef J_STATUS_TYPE (*tJ_Node_Invalidate)(NODE_HANDLE hNode);
typedef J_STATUS_TYPE (*tJ_Node_GetIsCachable)(NODE_HANDLE hNode, uint32_t* pVal);
typedef J_STATUS_TYPE (*tJ_Node_GetCachingMode)(NODE_HANDLE hNode, J_NODE_CACHINGMODE *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetPollingTime)(NODE_HANDLE hNode, int64_t *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetToolTip)(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetDescription)(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetDisplayName)(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetEventID)(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetIsStreamable)(NODE_HANDLE hNode, uint32_t* pVal);
typedef J_STATUS_TYPE (*tJ_Node_GetNumOfProperties)(NODE_HANDLE hNode, uint32_t* pNum);
typedef J_STATUS_TYPE (*tJ_Node_GetPropertyNameByIndex)(NODE_HANDLE hNode, uint32_t index, int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetPropertyByName)(NODE_HANDLE hNode, const int8_t* PropertyName, int8_t* ValueStr, uint32_t* pSizeValueStr, int8_t*AttributeStr, uint32_t* pSizeAttributeStr);
typedef J_STATUS_TYPE (*tJ_Node_ImposeAccessMode)(NODE_HANDLE hNode, J_NODE_ACCESSMODE AccessMode);
typedef J_STATUS_TYPE (*tJ_Node_ImposeVisibility)(NODE_HANDLE hNode, J_NODE_VISIBILITY Visibility);
typedef J_STATUS_TYPE (*tJ_Node_GetAlias)(NODE_HANDLE hNode, NODE_HANDLE* phAliasNode);
typedef J_STATUS_TYPE (*tJ_Node_RegisterCallback)(NODE_HANDLE hNode, J_NODE_CALLBACK_FUNCTION CallBackFunction);
typedef J_STATUS_TYPE (*tJ_Node_GetType)(NODE_HANDLE hNode, J_NODE_TYPE* pNodeType);
typedef J_STATUS_TYPE (*tJ_Node_GetIsSelector)(NODE_HANDLE hNode, uint32_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetNumOfSelectedFeatures)(NODE_HANDLE hNode, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Node_GetSelectedFeatureByIndex)(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectedNode);
typedef J_STATUS_TYPE (*tJ_Node_GetNumOfSelectingFeatures)(NODE_HANDLE hNode, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Node_GetSelectingFeatureByIndex)(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectingNode);
typedef J_STATUS_TYPE (*tJ_Factory_GetGenICamErrorInfo)(tGenICamErrorInfo* gc);
typedef J_STATUS_TYPE (*tJ_Node_GetRepresentation)(NODE_HANDLE hNode, J_NODE_REPRESENTATION *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetUnit)(NODE_HANDLE hNode,  int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetDocuURL)(NODE_HANDLE hNode,  int8_t* pBuffer, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_GetIsDeprecated)(NODE_HANDLE hNode, uint32_t* pVal);
typedef J_STATUS_TYPE (*tJ_Node_GetMinInt64)(NODE_HANDLE hNode, int64_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetMaxInt64)(NODE_HANDLE hNode, int64_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetInc)(NODE_HANDLE hNode,int64_t *pValue);
typedef J_STATUS_TYPE (*tJ_Node_SetValueInt64)(NODE_HANDLE hNode, bool Verify, int64_t Value);
typedef J_STATUS_TYPE (*tJ_Node_GetValueInt64)(NODE_HANDLE hNode, bool Verify, int64_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetFloatAlias)(NODE_HANDLE hNode, NODE_HANDLE* phNode);
typedef J_STATUS_TYPE (*tJ_Node_GetMinDouble)(NODE_HANDLE hNode, double* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetMaxDouble)(NODE_HANDLE hNode, double* pValue);
typedef J_STATUS_TYPE (*tJ_Node_SetValueDouble)(NODE_HANDLE hNode, bool Verify, double Value);
typedef J_STATUS_TYPE (*tJ_Node_GetValueDouble)(NODE_HANDLE hNode, bool Verify, double* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetDisplayNotation)(NODE_HANDLE hNode, J_NODE_DISPLAY_NOTATION* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetDisplayPrecision)(NODE_HANDLE hNode, int64_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetFloatHasInc)(NODE_HANDLE hNode, uint32_t*pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetFloatInc)(NODE_HANDLE hNode, double* pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetIntAlias)(NODE_HANDLE hNode, NODE_HANDLE* phNode);
typedef J_STATUS_TYPE (*tJ_Node_GetEnumAlias)(NODE_HANDLE hNode, NODE_HANDLE* phNode);
typedef J_STATUS_TYPE (*tJ_Node_GetNumOfEnumEntries)(NODE_HANDLE hEnumNode, uint32_t *pNum);
typedef J_STATUS_TYPE (*tJ_Node_GetEnumEntryByIndex)(NODE_HANDLE hEnumNode, uint32_t index, NODE_HANDLE* hEnumEntryNode);
typedef J_STATUS_TYPE (*tJ_Node_GetEnumEntryValue)(NODE_HANDLE hEnumNode, int64_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_ExecuteCommand)(NODE_HANDLE hNode);
typedef J_STATUS_TYPE (*tJ_Node_GetCommandIsDone)(NODE_HANDLE hNode, uint32_t* pValue);
typedef J_STATUS_TYPE (*tJ_Node_SetValueString)(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr);
typedef J_STATUS_TYPE (*tJ_Node_GetValueString)(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr, uint32_t* pSize);
typedef J_STATUS_TYPE (*tJ_Node_SetValueRegister)(NODE_HANDLE hNode, const uint8_t *pBuffer, int64_t Length);
typedef J_STATUS_TYPE (*tJ_Node_GetValueRegister)(NODE_HANDLE hNode, uint8_t *pBuffer, int64_t Length);
typedef J_STATUS_TYPE (*tJ_Node_GetRegisterLength)(NODE_HANDLE hNode, int64_t *pValue);
typedef J_STATUS_TYPE (*tJ_Node_GetRegisterAddress)(NODE_HANDLE hNode, int64_t *pValue);
typedef J_STATUS_TYPE (*tJ_Image_OpenViewWindowA)(LPCSTR pWindowName, POINT *pPoint, SIZE *pMaxSize, VIEW_HANDLE *pWin);
typedef J_STATUS_TYPE (*tJ_Image_OpenViewWindowW)(LPCWSTR pWindowName, POINT *pPoint, SIZE *pMaxSize, VIEW_HANDLE *pWin);
typedef J_STATUS_TYPE (*tJ_Image_OpenViewWindowExA)(J_IVW_WINDOW_TYPE iWindowType, LPCSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin);
typedef J_STATUS_TYPE (*tJ_Image_OpenViewWindowExW)(J_IVW_WINDOW_TYPE iWindowType, LPCWSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin);
typedef J_STATUS_TYPE (*tJ_Image_CloseViewWindow)(VIEW_HANDLE hWin);
typedef J_STATUS_TYPE (*tJ_Image_ShowImage)(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_ShowImageEx)(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_SetImageOffset)(VIEW_HANDLE hWin, POINT *pOffset, int32_t ZoomRatio);
typedef J_STATUS_TYPE (*tJ_Image_SetImageOffsetEx)(VIEW_HANDLE hWin, POINT *pOffset, double ZoomRatioX, double ZoomRatioY);
typedef J_STATUS_TYPE (*tJ_Image_GetImageOffsetEx)(VIEW_HANDLE hWin, POINT *pOffset, double *pZoomRatioX, double *pZoomRatioY);
typedef J_STATUS_TYPE (*tJ_Image_MoveViewWindow)(VIEW_HANDLE hWin, POINT *pPoint);
typedef J_STATUS_TYPE (*tJ_Image_ResizeChildWindow)(VIEW_HANDLE hWin, RECT *pRect);
typedef J_STATUS_TYPE (*tJ_Image_GetViewWindowRect)(VIEW_HANDLE hWin, RECT *pRect);
typedef J_STATUS_TYPE (*tJ_Image_GetFrameWindowRect)(VIEW_HANDLE hWin, RECT *pRect);
typedef J_STATUS_TYPE (*tJ_Image_SetViewWindowTitleA)(VIEW_HANDLE hWin, LPCSTR pWindowTitle);
typedef J_STATUS_TYPE (*tJ_Image_SetViewWindowTitleW)(VIEW_HANDLE hWin, LPCWSTR pWindowTitle);
typedef J_STATUS_TYPE (*tJ_Image_SetViewWindowOption)(VIEW_HANDLE hWin, J_IVW_OPTION Option, void* pOptionValue);
typedef J_STATUS_TYPE (*tJ_Image_GetViewWindowOption)(VIEW_HANDLE hWin, J_IVW_OPTION Option, void* pOptionValue);
typedef J_STATUS_TYPE (*tJ_Image_OpenStream)(CAM_HANDLE hCam, uint32_t iChannel, J_IMG_CALLBACK_OBJECT CBObject, J_IMG_CALLBACK_FUNCTION CBFunction, THRD_HANDLE * phThread, uint32_t iBufferSize, DWORD iMcIP);
typedef J_STATUS_TYPE (*tJ_Image_OpenStreamLight)(CAM_HANDLE hCam, uint32_t iChannel,  THRD_HANDLE* phThread);
typedef J_STATUS_TYPE (*tJ_Image_CloseStream)(THRD_HANDLE hThread);
typedef J_STATUS_TYPE (*tJ_Image_GetStreamInfo)(THRD_HANDLE hThread, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);
typedef J_STATUS_TYPE (*tJ_Image_GetStreamHandle)(THRD_HANDLE hThread, STREAM_HANDLE *phStream);
typedef J_STATUS_TYPE (*tJ_Image_FromRawToDIB)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_FromRawToDIBEx)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_FromRawToImage)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_FromRawToImageEx)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_ConvertImage)(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat);
typedef J_STATUS_TYPE (*tJ_Image_Malloc)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo);
typedef J_STATUS_TYPE (*tJ_Image_MallocDIB)(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo);
typedef J_STATUS_TYPE (*tJ_Image_MallocEx)(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat);
typedef J_STATUS_TYPE (*tJ_Image_Free)(J_tIMAGE_INFO * pBufferInfo);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileA)(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileW)(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileExA)(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath, J_SIF_FILE_FORMAT iFileFormat,  uint8_t iEncoderParameter);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileExW)(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath, J_SIF_FILE_FORMAT iFileFormat,  uint8_t iEncoderParameter);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileRawA)(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_SaveFileRawW)(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_LoadFileRawA)(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_LoadFileRawW)(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_GetPixel)(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel);
typedef J_STATUS_TYPE (*tJ_Image_GetPixelEx)(J_tIMAGE_INFO * pBufferInfo, double dXPos, double dYPos, void * pPixel);
typedef J_STATUS_TYPE (*tJ_Image_SetPixel)(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel);
typedef J_STATUS_TYPE (*tJ_Image_GetAverage)(J_tIMAGE_INFO * pBufferInfo, RECT *pMeasureRect, J_tBGR48 *pRGBAverage);
typedef J_STATUS_TYPE (*tJ_Image_SetGain)(HANDLE hThread, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);
typedef J_STATUS_TYPE (*tJ_Image_GetGain)(HANDLE hThread, uint32_t * piRGain, uint32_t * piGGain, uint32_t * piBGain);
typedef J_STATUS_TYPE (*tJ_Image_ExecuteWhiteBalance)(THRD_HANDLE hThread);
typedef J_STATUS_TYPE (*tJ_Camera_SaveSettingsA)(CAM_HANDLE hCam, LPCSTR sSettingsFileName, J_SAVE_SETTINGS_FLAG saveFlag);
typedef J_STATUS_TYPE (*tJ_Camera_SaveSettingsW)(CAM_HANDLE hCam, LPCWSTR sSettingsFileName, J_SAVE_SETTINGS_FLAG saveFlag );
typedef J_STATUS_TYPE (*tJ_Camera_LoadSettingsA)(CAM_HANDLE hCam, LPCSTR sSettingsFileName, J_LOAD_SETTINGS_FLAG loadFlag);
typedef J_STATUS_TYPE (*tJ_Camera_LoadSettingsW)(CAM_HANDLE hCam, LPCWSTR sSettingsFileName, J_LOAD_SETTINGS_FLAG loadFlag);
typedef J_STATUS_TYPE (*tJ_Camera_GetSettingsValidationErrorInfo)(CAM_HANDLE hCam, int8_t* errorInfoBuffer, uint32_t *pBufferSize);
typedef J_STATUS_TYPE (*tJ_Image_MallocHDR)(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, J_tIMAGE_INFO * pBufferInfo);
typedef J_STATUS_TYPE (*tJ_Image_AnalyzeHDR)(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, uint8_t* pImage1IsBrighter, int32_t *pBlackLevelImage1, int32_t *pBlackLevelImage2, float* pDarkGain);
typedef J_STATUS_TYPE (*tJ_Image_FuseHDR)(J_tIMAGE_INFO* pBrighterImageInfo, J_tIMAGE_INFO* pDarkerImageInfo, J_tIMAGE_INFO* pOutputBufferInfo, int32_t brighterBlackLevel, int32_t darkerBlackLevel, float fDarkGain, float fDualSlopeGain, uint8_t logOutput);
typedef J_STATUS_TYPE (*tJ_Image_MallocLUT)(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo);
typedef J_STATUS_TYPE (*tJ_Image_FreeLUT)(J_tLUT* pLutInfo);
typedef J_STATUS_TYPE (*tJ_Image_ConvertLUT)(J_tLUT* pSourceLutInfo, J_tLUT* pDestinationLutInfo);
typedef J_STATUS_TYPE (*tJ_Image_LUT)(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo);
typedef J_STATUS_TYPE (*tJ_Image_CreateGammaLUT)(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma);
typedef J_STATUS_TYPE (*tJ_Image_ApplyGammaToLUT)(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma);
typedef J_STATUS_TYPE (*tJ_Image_CreateLinearLUT)(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t X1, uint32_t X2, uint32_t Y1, uint32_t Y2);
typedef J_STATUS_TYPE (*tJ_Image_CreateKneeLUT)(J_tLUT* pLutInfo, uint32_t ColorIndex, POINT knee1, POINT knee2);
typedef J_STATUS_TYPE (*tJ_Image_GetLUTValue)(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t* pLUTValue);
typedef J_STATUS_TYPE (*tJ_Image_SetLUTValue)(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t LUTValue);
typedef J_STATUS_TYPE (*tJ_Image_SaveLUTA)(J_tLUT* pLutInfo, LPCSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_SaveLUTW)(J_tLUT* pLutInfo, LPCWSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_LoadLUTA)(J_tLUT* pLutInfo, LPCSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_LoadLUTW)(J_tLUT* pLutInfo, LPCWSTR pPath);
typedef J_STATUS_TYPE (*tJ_Image_MallocHistogram)(J_tIMAGE_INFO* pImageInfo, J_tHIST* pHistogramInfo);
typedef J_STATUS_TYPE (*tJ_Image_FreeHistogram)(J_tHIST* pHistogramInfo);
typedef J_STATUS_TYPE (*tJ_Image_ClearHistogram)(J_tHIST* pHistogramInfo);
typedef J_STATUS_TYPE (*tJ_Image_CreateHistogram)(J_tIMAGE_INFO* pImageInfo, J_tHIST* pHistogramInfo);
typedef J_STATUS_TYPE (*tJ_Image_GetHistogramValue)(J_tHIST* pHistogramInfo, uint32_t ColorIndex, uint32_t HistogramIndex, uint32_t* pHistogramValue);
typedef J_STATUS_TYPE (*tJ_Image_Flip)(J_tIMAGE_INFO* pImageInfo, J_IMAGE_FLIP_TYPE flipType);
typedef J_STATUS_TYPE (*tJ_Image_Rotate)(J_tIMAGE_INFO* pImageInfo, J_IMAGE_ROTATE_TYPE rotateType);
typedef J_STATUS_TYPE (*tJ_Factory_SendActionCommand)(FACTORY_HANDLE hFac, uint32_t iDeviceKey, uint32_t iGroupKey, uint32_t iGroupMask, J_tACTION_CMD_ACK* pAckBuffer, uint32_t* piNumOfAck);
typedef J_STATUS_TYPE (*tJ_Image_Processing)(J_tIMAGE_INFO* pImageInfo, J_IMAGE_PROCESSING iProcessFunction, void* pParameters);
typedef J_STATUS_TYPE (*tJ_Image_ProcessingEx)(J_tIMAGE_INFO* pInputImageInfo, J_tIMAGE_INFO* pOutputImageInfo, J_IMAGE_PROCESSING iProcessFunction, void* pParameters);

HMODULE	PUB_Handle =0;

tJ_Factory_Open                     pJ_Factory_Open = 0;
tJ_Factory_Close                    pJ_Factory_Close = 0;
tJ_Factory_GetInfo                  pJ_Factory_GetInfo = 0;
tJ_Factory_UpdateCameraList         pJ_Factory_UpdateCameraList = 0;
tJ_Factory_GetNumOfCameras          pJ_Factory_GetNumOfCameras = 0;
tJ_Factory_GetCameraIDByIndex       pJ_Factory_GetCameraIDByIndex = 0;
tJ_Factory_GetCameraInfo            pJ_Factory_GetCameraInfo = 0;
tJ_Factory_EnableForceIp            pJ_Factory_EnableForceIp = 0;
tJ_Camera_Open                      pJ_Camera_Open = 0;
tJ_Camera_OpenMc                    pJ_Camera_OpenMc = 0;
tJ_Camera_Close                     pJ_Camera_Close = 0;
tJ_Camera_GetValueInt64             pJ_Camera_GetValueInt64 = 0;
tJ_Camera_GetValueDouble            pJ_Camera_GetValueDouble = 0;
tJ_Camera_GetValueString            pJ_Camera_GetValueString = 0;
tJ_Camera_SetValueInt64             pJ_Camera_SetValueInt64 = 0;
tJ_Camera_SetValueDouble            pJ_Camera_SetValueDouble = 0;
tJ_Camera_SetValueString            pJ_Camera_SetValueString = 0;
tJ_Camera_ExecuteCommand            pJ_Camera_ExecuteCommand = 0;
tJ_Camera_ReadData                  pJ_Camera_ReadData = 0;
tJ_Camera_WriteData                 pJ_Camera_WriteData = 0;
tJ_Camera_GetConfigInfo             pJ_Camera_GetConfigInfo = 0;
tJ_Camera_GetNumOfNodes             pJ_Camera_GetNumOfNodes = 0;
tJ_Camera_GetNodeByIndex            pJ_Camera_GetNodeByIndex = 0;
tJ_Camera_GetNodeByName             pJ_Camera_GetNodeByName = 0;
tJ_Camera_GetNumOfSubFeatures       pJ_Camera_GetNumOfSubFeatures = 0;
tJ_Camera_GetSubFeatureByIndex      pJ_Camera_GetSubFeatureByIndex = 0;
tJ_Camera_GetFeatureByName          pJ_Camera_GetFeatureByName = 0;
tJ_Camera_InvalidateNodes           pJ_Camera_InvalidateNodes = 0;
tJ_Camera_GetNumOfDataStreams       pJ_Camera_GetNumOfDataStreams = 0;
tJ_Camera_CreateDataStream          pJ_Camera_CreateDataStream = 0;
tJ_Camera_CreateDataStreamMc        pJ_Camera_CreateDataStreamMc = 0;
tJ_DataStream_AnnounceBuffer        pJ_DataStream_AnnounceBuffer = 0;
tJ_DataStream_FlushQueue            pJ_DataStream_FlushQueue = 0;
tJ_DataStream_StartAcquisition      pJ_DataStream_StartAcquisition = 0;
tJ_DataStream_StopAcquisition       pJ_DataStream_StopAcquisition = 0;
tJ_DataStream_GetStreamInfo         pJ_DataStream_GetStreamInfo = 0;
tJ_DataStream_GetBufferID           pJ_DataStream_GetBufferID = 0;
tJ_DataStream_Close                 pJ_DataStream_Close = 0;
tJ_DataStream_RevokeBuffer          pJ_DataStream_RevokeBuffer = 0;
tJ_DataStream_QueueBuffer           pJ_DataStream_QueueBuffer = 0;
tJ_DataStream_GetBufferInfo         pJ_DataStream_GetBufferInfo = 0;
tJ_DataStream_GetParam              pJ_DataStream_GetParam = 0;
tJ_DataStream_SetParam              pJ_DataStream_SetParam = 0;
tJ_Event_GetData                    pJ_Event_GetData = 0;
tJ_Event_GetInfo                    pJ_Event_GetInfo = 0;
tJ_Event_Flush                      pJ_Event_Flush = 0;
tJ_Event_Close                      pJ_Event_Close = 0;
tJ_Camera_RegisterEvent             pJ_Camera_RegisterEvent = 0;
tJ_Camera_UnRegisterEvent           pJ_Camera_UnRegisterEvent = 0;
tJ_DataStream_RegisterEvent         pJ_DataStream_RegisterEvent = 0;
tJ_DataStream_UnRegisterEvent       pJ_DataStream_UnRegisterEvent = 0;
tJ_Event_CreateCondition            pJ_Event_CreateCondition = 0;
tJ_Event_SignalCondition            pJ_Event_SignalCondition = 0;
tJ_Event_ExitCondition              pJ_Event_ExitCondition = 0;
tJ_Event_ResetCondition             pJ_Event_ResetCondition = 0;
tJ_Event_WaitForCondition           pJ_Event_WaitForCondition = 0;
tJ_Event_WaitForMultipleConditions  pJ_Event_WaitForMultipleConditions = 0;
tJ_Event_CloseCondition             pJ_Event_CloseCondition = 0;
tJ_Node_GetAccessMode               pJ_Node_GetAccessMode = 0;
tJ_Node_GetName                     pJ_Node_GetName = 0;
tJ_Node_GetNameSpace                pJ_Node_GetNameSpace = 0;
tJ_Node_GetVisibility               pJ_Node_GetVisibility = 0;
tJ_Node_Invalidate                  pJ_Node_Invalidate = 0;
tJ_Node_GetIsCachable               pJ_Node_GetIsCachable = 0;
tJ_Node_GetCachingMode              pJ_Node_GetCachingMode = 0;
tJ_Node_GetPollingTime              pJ_Node_GetPollingTime = 0;
tJ_Node_GetToolTip                  pJ_Node_GetToolTip = 0;
tJ_Node_GetDescription              pJ_Node_GetDescription = 0;
tJ_Node_GetDisplayName              pJ_Node_GetDisplayName = 0;
tJ_Node_GetEventID                  pJ_Node_GetEventID = 0;
tJ_Node_GetIsStreamable             pJ_Node_GetIsStreamable = 0;
tJ_Node_GetNumOfProperties          pJ_Node_GetNumOfProperties = 0;
tJ_Node_GetPropertyNameByIndex      pJ_Node_GetPropertyNameByIndex = 0;
tJ_Node_GetPropertyByName           pJ_Node_GetPropertyByName = 0;
tJ_Node_ImposeAccessMode            pJ_Node_ImposeAccessMode = 0;
tJ_Node_ImposeVisibility            pJ_Node_ImposeVisibility = 0;
tJ_Node_GetAlias                    pJ_Node_GetAlias = 0;
tJ_Node_RegisterCallback            pJ_Node_RegisterCallback = 0;
tJ_Node_GetType                     pJ_Node_GetType = 0;
tJ_Node_GetIsSelector               pJ_Node_GetIsSelector = 0;
tJ_Node_GetNumOfSelectedFeatures    pJ_Node_GetNumOfSelectedFeatures = 0;
tJ_Node_GetSelectedFeatureByIndex   pJ_Node_GetSelectedFeatureByIndex = 0;
tJ_Node_GetNumOfSelectingFeatures   pJ_Node_GetNumOfSelectingFeatures = 0;
tJ_Node_GetSelectingFeatureByIndex  pJ_Node_GetSelectingFeatureByIndex = 0;
tJ_Factory_GetGenICamErrorInfo      pJ_Factory_GetGenICamErrorInfo = 0;
tJ_Node_GetRepresentation           pJ_Node_GetRepresentation = 0;
tJ_Node_GetUnit                     pJ_Node_GetUnit = 0;
tJ_Node_GetDocuURL                  pJ_Node_GetDocuURL = 0;
tJ_Node_GetIsDeprecated             pJ_Node_GetIsDeprecated = 0;
tJ_Node_GetMinInt64                 pJ_Node_GetMinInt64 = 0;
tJ_Node_GetMaxInt64                 pJ_Node_GetMaxInt64 = 0;
tJ_Node_GetInc                      pJ_Node_GetInc = 0;
tJ_Node_SetValueInt64               pJ_Node_SetValueInt64 = 0;
tJ_Node_GetValueInt64               pJ_Node_GetValueInt64 = 0;
tJ_Node_GetFloatAlias               pJ_Node_GetFloatAlias = 0;
tJ_Node_GetMinDouble                pJ_Node_GetMinDouble = 0;
tJ_Node_GetMaxDouble                pJ_Node_GetMaxDouble = 0;
tJ_Node_SetValueDouble              pJ_Node_SetValueDouble = 0;
tJ_Node_GetValueDouble              pJ_Node_GetValueDouble = 0;
tJ_Node_GetDisplayNotation          pJ_Node_GetDisplayNotation = 0;
tJ_Node_GetDisplayPrecision         pJ_Node_GetDisplayPrecision = 0;
tJ_Node_GetFloatHasInc              pJ_Node_GetFloatHasInc = 0;
tJ_Node_GetFloatInc                 pJ_Node_GetFloatInc = 0;
tJ_Node_GetIntAlias                 pJ_Node_GetIntAlias = 0;
tJ_Node_GetEnumAlias                pJ_Node_GetEnumAlias = 0;
tJ_Node_GetNumOfEnumEntries         pJ_Node_GetNumOfEnumEntries = 0;
tJ_Node_GetEnumEntryByIndex         pJ_Node_GetEnumEntryByIndex = 0;
tJ_Node_GetEnumEntryValue           pJ_Node_GetEnumEntryValue = 0;
tJ_Node_ExecuteCommand              pJ_Node_ExecuteCommand = 0;
tJ_Node_GetCommandIsDone            pJ_Node_GetCommandIsDone = 0;
tJ_Node_SetValueString              pJ_Node_SetValueString = 0;
tJ_Node_GetValueString              pJ_Node_GetValueString = 0;
tJ_Node_SetValueRegister            pJ_Node_SetValueRegister = 0;
tJ_Node_GetValueRegister            pJ_Node_GetValueRegister = 0;
tJ_Node_GetRegisterLength           pJ_Node_GetRegisterLength = 0;
tJ_Node_GetRegisterAddress          pJ_Node_GetRegisterAddress = 0;
tJ_Image_OpenViewWindowA            pJ_Image_OpenViewWindowA = 0;
tJ_Image_OpenViewWindowW            pJ_Image_OpenViewWindowW = 0;
tJ_Image_OpenViewWindowExA          pJ_Image_OpenViewWindowExA = 0;
tJ_Image_OpenViewWindowExW          pJ_Image_OpenViewWindowExW = 0;
tJ_Image_CloseViewWindow            pJ_Image_CloseViewWindow = 0;
tJ_Image_ShowImage                  pJ_Image_ShowImage = 0;
tJ_Image_ShowImageEx                pJ_Image_ShowImageEx = 0;
tJ_Image_SetImageOffset             pJ_Image_SetImageOffset = 0;
tJ_Image_SetImageOffsetEx           pJ_Image_SetImageOffsetEx = 0;
tJ_Image_GetImageOffsetEx           pJ_Image_GetImageOffsetEx = 0;
tJ_Image_MoveViewWindow             pJ_Image_MoveViewWindow = 0;
tJ_Image_ResizeChildWindow          pJ_Image_ResizeChildWindow = 0;
tJ_Image_GetViewWindowRect          pJ_Image_GetViewWindowRect = 0;
tJ_Image_GetFrameWindowRect         pJ_Image_GetFrameWindowRect = 0;
tJ_Image_SetViewWindowTitleA        pJ_Image_SetViewWindowTitleA = 0;
tJ_Image_SetViewWindowTitleW        pJ_Image_SetViewWindowTitleW = 0;
tJ_Image_SetViewWindowOption        pJ_Image_SetViewWindowOption = 0;
tJ_Image_GetViewWindowOption        pJ_Image_GetViewWindowOption = 0;
tJ_Image_OpenStream                 pJ_Image_OpenStream = 0;
tJ_Image_OpenStreamLight            pJ_Image_OpenStreamLight = 0;
tJ_Image_CloseStream                pJ_Image_CloseStream = 0;
tJ_Image_GetStreamInfo              pJ_Image_GetStreamInfo = 0;
tJ_Image_GetStreamHandle            pJ_Image_GetStreamHandle = 0;
tJ_Image_FromRawToDIB               pJ_Image_FromRawToDIB = 0;
tJ_Image_FromRawToDIBEx             pJ_Image_FromRawToDIBEx = 0;
tJ_Image_FromRawToImage             pJ_Image_FromRawToImage = 0;
tJ_Image_FromRawToImageEx           pJ_Image_FromRawToImageEx = 0;
tJ_Image_ConvertImage               pJ_Image_ConvertImage = 0;
tJ_Image_Malloc                     pJ_Image_Malloc = 0;
tJ_Image_MallocDIB                  pJ_Image_MallocDIB = 0;
tJ_Image_MallocEx                   pJ_Image_MallocEx = 0;
tJ_Image_Free                       pJ_Image_Free = 0;
tJ_Image_SaveFileA                  pJ_Image_SaveFileA = 0;
tJ_Image_SaveFileW                  pJ_Image_SaveFileW = 0;
tJ_Image_SaveFileExA                pJ_Image_SaveFileExA = 0;
tJ_Image_SaveFileExW                pJ_Image_SaveFileExW = 0;
tJ_Image_SaveFileRawA               pJ_Image_SaveFileRawA = 0;
tJ_Image_SaveFileRawW               pJ_Image_SaveFileRawW = 0;
tJ_Image_LoadFileRawA               pJ_Image_LoadFileRawA = 0;
tJ_Image_LoadFileRawW               pJ_Image_LoadFileRawW = 0;
tJ_Image_GetPixel                   pJ_Image_GetPixel = 0;
tJ_Image_GetPixelEx                 pJ_Image_GetPixelEx = 0;
tJ_Image_SetPixel                   pJ_Image_SetPixel = 0;
tJ_Image_GetAverage                 pJ_Image_GetAverage = 0;
tJ_Image_SetGain                    pJ_Image_SetGain = 0;
tJ_Image_GetGain                    pJ_Image_GetGain = 0;
tJ_Image_ExecuteWhiteBalance        pJ_Image_ExecuteWhiteBalance = 0;
tJ_Camera_SaveSettingsA             pJ_Camera_SaveSettingsA = 0;
tJ_Camera_SaveSettingsW             pJ_Camera_SaveSettingsW = 0;
tJ_Camera_LoadSettingsA             pJ_Camera_LoadSettingsA = 0;
tJ_Camera_LoadSettingsW             pJ_Camera_LoadSettingsW = 0;
tJ_Camera_GetSettingsValidationErrorInfo pJ_Camera_GetSettingsValidationErrorInfo = 0;
tJ_Image_MallocHDR                  pJ_Image_MallocHDR = 0;
tJ_Image_AnalyzeHDR                 pJ_Image_AnalyzeHDR = 0;
tJ_Image_FuseHDR                    pJ_Image_FuseHDR = 0;
tJ_Image_MallocLUT                  pJ_Image_MallocLUT = 0;
tJ_Image_FreeLUT                    pJ_Image_FreeLUT = 0;
tJ_Image_ConvertLUT                 pJ_Image_ConvertLUT = 0;
tJ_Image_LUT                        pJ_Image_LUT = 0;
tJ_Image_CreateGammaLUT             pJ_Image_CreateGammaLUT = 0;
tJ_Image_ApplyGammaToLUT            pJ_Image_ApplyGammaToLUT = 0;
tJ_Image_CreateLinearLUT            pJ_Image_CreateLinearLUT = 0;
tJ_Image_CreateKneeLUT              pJ_Image_CreateKneeLUT = 0;
tJ_Image_GetLUTValue                pJ_Image_GetLUTValue = 0;
tJ_Image_SetLUTValue                pJ_Image_SetLUTValue = 0;
tJ_Image_SaveLUTA                   pJ_Image_SaveLUTA = 0;
tJ_Image_SaveLUTW                   pJ_Image_SaveLUTW = 0;
tJ_Image_LoadLUTA                   pJ_Image_LoadLUTA = 0;
tJ_Image_LoadLUTW                   pJ_Image_LoadLUTW = 0;
tJ_Image_MallocHistogram            pJ_Image_MallocHistogram = 0;
tJ_Image_FreeHistogram              pJ_Image_FreeHistogram = 0;
tJ_Image_ClearHistogram             pJ_Image_ClearHistogram = 0;
tJ_Image_CreateHistogram            pJ_Image_CreateHistogram = 0;
tJ_Image_GetHistogramValue          pJ_Image_GetHistogramValue = 0;
tJ_Image_Flip                       pJ_Image_Flip = 0;
tJ_Image_Rotate                     pJ_Image_Rotate = 0;
tJ_Factory_SendActionCommand        pJ_Factory_SendActionCommand = 0;
tJ_Image_Processing                 pJ_Image_Processing = 0;
tJ_Image_ProcessingEx               pJ_Image_ProcessingEx = 0;

int OpenJaiSdk()
{
	OutputDebugString(_T("OpenJaiSdk Called\r\n"));

   #ifdef _WIN64
	  TCHAR* path = _tgetenv(_T("JAI_SDK_BIN_64"));
   #else
	  TCHAR* path = _tgetenv(_T("JAI_SDK_BIN"));
   #endif
	if (path == NULL)
		return 0;

	if (PathIsDirectory(path) == FALSE)
		return 0;

	TCHAR szPath[MAX_PATH]={0};
	_stprintf(szPath,_T("%s\\Jai_Factory.dll"), path);
	PUB_Handle = LoadLibrary(szPath);
	if (PUB_Handle == NULL)
	{
		PUB_Handle = LoadLibrary(szPath);
		return 0;
	}

    pJ_Factory_Open					    = (tJ_Factory_Open                 )GetProcAddress(PUB_Handle,"J_Factory_Open");
    pJ_Factory_Close                    = (tJ_Factory_Close                )GetProcAddress(PUB_Handle, "J_Factory_Close");
    pJ_Factory_GetInfo                  = (tJ_Factory_GetInfo              )GetProcAddress(PUB_Handle, "J_Factory_GetInfo");
    pJ_Factory_UpdateCameraList         = (tJ_Factory_UpdateCameraList     )GetProcAddress(PUB_Handle, "J_Factory_UpdateCameraList");
    pJ_Factory_GetNumOfCameras          = (tJ_Factory_GetNumOfCameras      )GetProcAddress(PUB_Handle, "J_Factory_GetNumOfCameras");
    pJ_Factory_GetCameraIDByIndex       = (tJ_Factory_GetCameraIDByIndex   )GetProcAddress(PUB_Handle, "J_Factory_GetCameraIDByIndex");
    pJ_Factory_GetCameraInfo            = (tJ_Factory_GetCameraInfo        )GetProcAddress(PUB_Handle, "J_Factory_GetCameraInfo");
    pJ_Factory_EnableForceIp            = (tJ_Factory_EnableForceIp        )GetProcAddress(PUB_Handle, "J_Factory_EnableForceIp");
    pJ_Camera_Open                      = (tJ_Camera_Open                  )GetProcAddress(PUB_Handle, "J_Camera_Open");
    pJ_Camera_OpenMc                    = (tJ_Camera_OpenMc                )GetProcAddress(PUB_Handle, "J_Camera_OpenMc");
    pJ_Camera_Close                     = (tJ_Camera_Close                 )GetProcAddress(PUB_Handle, "J_Camera_Close");
    pJ_Camera_GetValueInt64             = (tJ_Camera_GetValueInt64         )GetProcAddress(PUB_Handle, "J_Camera_GetValueInt64");
    pJ_Camera_GetValueDouble            = (tJ_Camera_GetValueDouble        )GetProcAddress(PUB_Handle, "J_Camera_GetValueDouble");
    pJ_Camera_GetValueString            = (tJ_Camera_GetValueString        )GetProcAddress(PUB_Handle, "J_Camera_GetValueString");
    pJ_Camera_SetValueInt64             = (tJ_Camera_SetValueInt64         )GetProcAddress(PUB_Handle, "J_Camera_SetValueInt64");
    pJ_Camera_SetValueDouble            = (tJ_Camera_SetValueDouble        )GetProcAddress(PUB_Handle, "J_Camera_SetValueDouble");
    pJ_Camera_SetValueString            = (tJ_Camera_SetValueString        )GetProcAddress(PUB_Handle, "J_Camera_SetValueString");
    pJ_Camera_ExecuteCommand            = (tJ_Camera_ExecuteCommand        )GetProcAddress(PUB_Handle, "J_Camera_ExecuteCommand");
    pJ_Camera_ReadData                  = (tJ_Camera_ReadData              )GetProcAddress(PUB_Handle, "J_Camera_ReadData");
    pJ_Camera_WriteData                 = (tJ_Camera_WriteData             )GetProcAddress(PUB_Handle, "J_Camera_WriteData");
    pJ_Camera_GetConfigInfo             = (tJ_Camera_GetConfigInfo         )GetProcAddress(PUB_Handle, "J_Camera_GetConfigInfo");
    pJ_Camera_GetNumOfNodes             = (tJ_Camera_GetNumOfNodes         )GetProcAddress(PUB_Handle, "J_Camera_GetNumOfNodes");
    pJ_Camera_GetNodeByIndex            = (tJ_Camera_GetNodeByIndex        )GetProcAddress(PUB_Handle, "J_Camera_GetNodeByIndex");
    pJ_Camera_GetNodeByName             = (tJ_Camera_GetNodeByName         )GetProcAddress(PUB_Handle, "J_Camera_GetNodeByName");
    pJ_Camera_GetNumOfSubFeatures       = (tJ_Camera_GetNumOfSubFeatures   )GetProcAddress(PUB_Handle, "J_Camera_GetNumOfSubFeatures");
    pJ_Camera_GetSubFeatureByIndex      = (tJ_Camera_GetSubFeatureByIndex  )GetProcAddress(PUB_Handle, "J_Camera_GetSubFeatureByIndex");
    pJ_Camera_GetFeatureByName          = (tJ_Camera_GetFeatureByName      )GetProcAddress(PUB_Handle, "J_Camera_GetFeatureByName");
    pJ_Camera_InvalidateNodes           = (tJ_Camera_InvalidateNodes       )GetProcAddress(PUB_Handle, "J_Camera_InvalidateNodes");
    pJ_Camera_GetNumOfDataStreams       = (tJ_Camera_GetNumOfDataStreams   )GetProcAddress(PUB_Handle, "J_Camera_GetNumOfDataStreams");
    pJ_Camera_CreateDataStream          = (tJ_Camera_CreateDataStream      )GetProcAddress(PUB_Handle, "J_Camera_CreateDataStream");
    pJ_Camera_CreateDataStreamMc        = (tJ_Camera_CreateDataStreamMc    )GetProcAddress(PUB_Handle, "J_Camera_CreateDataStreamMc");
    pJ_DataStream_AnnounceBuffer        = (tJ_DataStream_AnnounceBuffer    )GetProcAddress(PUB_Handle, "J_DataStream_AnnounceBuffer");
    pJ_DataStream_FlushQueue            = (tJ_DataStream_FlushQueue        )GetProcAddress(PUB_Handle, "J_DataStream_FlushQueue");
    pJ_DataStream_StartAcquisition      = (tJ_DataStream_StartAcquisition  )GetProcAddress(PUB_Handle, "J_DataStream_StartAcquisition");
    pJ_DataStream_StopAcquisition       = (tJ_DataStream_StopAcquisition   )GetProcAddress(PUB_Handle, "J_DataStream_StopAcquisition");
    pJ_DataStream_GetStreamInfo         = (tJ_DataStream_GetStreamInfo     )GetProcAddress(PUB_Handle, "J_DataStream_GetStreamInfo");
    pJ_DataStream_GetBufferID           = (tJ_DataStream_GetBufferID       )GetProcAddress(PUB_Handle, "J_DataStream_GetBufferID");
    pJ_DataStream_Close                 = (tJ_DataStream_Close             )GetProcAddress(PUB_Handle, "J_DataStream_Close");
    pJ_DataStream_RevokeBuffer          = (tJ_DataStream_RevokeBuffer      )GetProcAddress(PUB_Handle, "J_DataStream_RevokeBuffer");
    pJ_DataStream_QueueBuffer           = (tJ_DataStream_QueueBuffer       )GetProcAddress(PUB_Handle, "J_DataStream_QueueBuffer");
    pJ_DataStream_GetBufferInfo         = (tJ_DataStream_GetBufferInfo     )GetProcAddress(PUB_Handle, "J_DataStream_GetBufferInfo");
    pJ_DataStream_GetParam              = (tJ_DataStream_GetParam          )GetProcAddress(PUB_Handle, "J_DataStream_GetParam");
    pJ_DataStream_SetParam              = (tJ_DataStream_SetParam          )GetProcAddress(PUB_Handle, "J_DataStream_SetParam");
    pJ_Event_GetData                    = (tJ_Event_GetData                )GetProcAddress(PUB_Handle, "J_Event_GetData");
    pJ_Event_GetInfo                    = (tJ_Event_GetInfo                )GetProcAddress(PUB_Handle, "J_Event_GetInfo");
    pJ_Event_Flush                      = (tJ_Event_Flush                  )GetProcAddress(PUB_Handle, "J_Event_Flush");
    pJ_Event_Close                      = (tJ_Event_Close                  )GetProcAddress(PUB_Handle, "J_Event_Close");
    pJ_Camera_RegisterEvent             = (tJ_Camera_RegisterEvent         )GetProcAddress(PUB_Handle, "J_Camera_RegisterEvent");
    pJ_Camera_UnRegisterEvent           = (tJ_Camera_UnRegisterEvent       )GetProcAddress(PUB_Handle, "J_Camera_UnRegisterEvent");
    pJ_DataStream_RegisterEvent         = (tJ_DataStream_RegisterEvent     )GetProcAddress(PUB_Handle, "J_DataStream_RegisterEvent");
    pJ_DataStream_UnRegisterEvent       = (tJ_DataStream_UnRegisterEvent   )GetProcAddress(PUB_Handle, "J_DataStream_UnRegisterEvent");
    pJ_Event_CreateCondition            = (tJ_Event_CreateCondition        )GetProcAddress(PUB_Handle, "J_Event_CreateCondition");
    pJ_Event_SignalCondition            = (tJ_Event_SignalCondition        )GetProcAddress(PUB_Handle, "J_Event_SignalCondition");
    pJ_Event_ExitCondition              = (tJ_Event_ExitCondition          )GetProcAddress(PUB_Handle, "J_Event_ExitCondition");
    pJ_Event_ResetCondition             = (tJ_Event_ResetCondition         )GetProcAddress(PUB_Handle, "J_Event_ResetCondition");
    pJ_Event_WaitForCondition           = (tJ_Event_WaitForCondition       )GetProcAddress(PUB_Handle, "J_Event_WaitForCondition");
    pJ_Event_WaitForMultipleConditions  = (tJ_Event_WaitForMultipleConditions)GetProcAddress(PUB_Handle, "J_Event_WaitForMultipleConditions");
    pJ_Event_CloseCondition             = (tJ_Event_CloseCondition         )GetProcAddress(PUB_Handle, "J_Event_CloseCondition");
    pJ_Node_GetAccessMode               = (tJ_Node_GetAccessMode           )GetProcAddress(PUB_Handle, "J_Node_GetAccessMode");
    pJ_Node_GetName                     = (tJ_Node_GetName                 )GetProcAddress(PUB_Handle, "J_Node_GetName");
    pJ_Node_GetNameSpace                = (tJ_Node_GetNameSpace            )GetProcAddress(PUB_Handle, "J_Node_GetNameSpace");
    pJ_Node_GetVisibility               = (tJ_Node_GetVisibility           )GetProcAddress(PUB_Handle, "J_Node_GetVisibility");
    pJ_Node_Invalidate                  = (tJ_Node_Invalidate              )GetProcAddress(PUB_Handle, "J_Node_Invalidate");
    pJ_Node_GetIsCachable               = (tJ_Node_GetIsCachable           )GetProcAddress(PUB_Handle, "J_Node_GetIsCachable");
    pJ_Node_GetCachingMode              = (tJ_Node_GetCachingMode          )GetProcAddress(PUB_Handle, "J_Node_GetCachingMode");
    pJ_Node_GetPollingTime              = (tJ_Node_GetPollingTime          )GetProcAddress(PUB_Handle, "J_Node_GetPollingTime");
    pJ_Node_GetToolTip                  = (tJ_Node_GetToolTip              )GetProcAddress(PUB_Handle, "J_Node_GetToolTip");
    pJ_Node_GetDescription              = (tJ_Node_GetDescription          )GetProcAddress(PUB_Handle, "J_Node_GetDescription");
    pJ_Node_GetDisplayName              = (tJ_Node_GetDisplayName          )GetProcAddress(PUB_Handle, "J_Node_GetDisplayName");
    pJ_Node_GetEventID                  = (tJ_Node_GetEventID              )GetProcAddress(PUB_Handle, "J_Node_GetEventID");
    pJ_Node_GetIsStreamable             = (tJ_Node_GetIsStreamable         )GetProcAddress(PUB_Handle, "J_Node_GetIsStreamable");
    pJ_Node_GetNumOfProperties          = (tJ_Node_GetNumOfProperties      )GetProcAddress(PUB_Handle, "J_Node_GetNumOfProperties");
    pJ_Node_GetPropertyNameByIndex      = (tJ_Node_GetPropertyNameByIndex  )GetProcAddress(PUB_Handle, "J_Node_GetPropertyNameByIndex");
    pJ_Node_GetPropertyByName           = (tJ_Node_GetPropertyByName       )GetProcAddress(PUB_Handle, "J_Node_GetPropertyByName");
    pJ_Node_ImposeAccessMode            = (tJ_Node_ImposeAccessMode        )GetProcAddress(PUB_Handle, "J_Node_ImposeAccessMode");
    pJ_Node_ImposeVisibility            = (tJ_Node_ImposeVisibility        )GetProcAddress(PUB_Handle, "J_Node_ImposeVisibility");
    pJ_Node_GetAlias                    = (tJ_Node_GetAlias                )GetProcAddress(PUB_Handle, "J_Node_GetAlias");
    pJ_Node_RegisterCallback            = (tJ_Node_RegisterCallback        )GetProcAddress(PUB_Handle, "J_Node_RegisterCallback");
    pJ_Node_GetType                     = (tJ_Node_GetType                 )GetProcAddress(PUB_Handle, "J_Node_GetType");
    pJ_Node_GetIsSelector               = (tJ_Node_GetIsSelector           )GetProcAddress(PUB_Handle, "J_Node_GetIsSelector");
    pJ_Node_GetNumOfSelectedFeatures    = (tJ_Node_GetNumOfSelectedFeatures )GetProcAddress(PUB_Handle, "J_Node_GetNumOfSelectedFeatures");
    pJ_Node_GetSelectedFeatureByIndex   = (tJ_Node_GetSelectedFeatureByIndex )GetProcAddress(PUB_Handle, "J_Node_GetSelectedFeatureByIndex");
    pJ_Node_GetNumOfSelectingFeatures   = (tJ_Node_GetNumOfSelectingFeatures )GetProcAddress(PUB_Handle, "J_Node_GetNumOfSelectingFeatures");
    pJ_Node_GetSelectingFeatureByIndex  = (tJ_Node_GetSelectingFeatureByIndex )GetProcAddress(PUB_Handle, "J_Node_GetSelectingFeatureByIndex");
    pJ_Factory_GetGenICamErrorInfo      = (tJ_Factory_GetGenICamErrorInfo  )GetProcAddress(PUB_Handle, "J_Factory_GetGenICamErrorInfo");
    pJ_Node_GetRepresentation           = (tJ_Node_GetRepresentation       )GetProcAddress(PUB_Handle, "J_Node_GetRepresentation");
    pJ_Node_GetUnit                     = (tJ_Node_GetUnit                 )GetProcAddress(PUB_Handle, "J_Node_GetUnit");
    pJ_Node_GetDocuURL                  = (tJ_Node_GetDocuURL              )GetProcAddress(PUB_Handle, "J_Node_GetDocuURL");
    pJ_Node_GetIsDeprecated             = (tJ_Node_GetIsDeprecated         )GetProcAddress(PUB_Handle, "J_Node_GetIsDeprecated");
    pJ_Node_GetMinInt64                 = (tJ_Node_GetMinInt64             )GetProcAddress(PUB_Handle, "J_Node_GetMinInt64");
    pJ_Node_GetMaxInt64                 = (tJ_Node_GetMaxInt64             )GetProcAddress(PUB_Handle, "J_Node_GetMaxInt64");
    pJ_Node_GetInc                      = (tJ_Node_GetInc                  )GetProcAddress(PUB_Handle, "J_Node_GetInc");
    pJ_Node_SetValueInt64               = (tJ_Node_SetValueInt64           )GetProcAddress(PUB_Handle, "J_Node_SetValueInt64");
    pJ_Node_GetValueInt64               = (tJ_Node_GetValueInt64           )GetProcAddress(PUB_Handle, "J_Node_GetValueInt64");
    pJ_Node_GetFloatAlias               = (tJ_Node_GetFloatAlias           )GetProcAddress(PUB_Handle, "J_Node_GetFloatAlias");
    pJ_Node_GetMinDouble                = (tJ_Node_GetMinDouble            )GetProcAddress(PUB_Handle, "J_Node_GetMinDouble");
    pJ_Node_GetMaxDouble                = (tJ_Node_GetMaxDouble            )GetProcAddress(PUB_Handle, "J_Node_GetMaxDouble");
    pJ_Node_SetValueDouble              = (tJ_Node_SetValueDouble          )GetProcAddress(PUB_Handle, "J_Node_SetValueDouble");
    pJ_Node_GetValueDouble              = (tJ_Node_GetValueDouble          )GetProcAddress(PUB_Handle, "J_Node_GetValueDouble");
    pJ_Node_GetDisplayNotation          = (tJ_Node_GetDisplayNotation      )GetProcAddress(PUB_Handle, "J_Node_GetDisplayNotation");
    pJ_Node_GetDisplayPrecision         = (tJ_Node_GetDisplayPrecision     )GetProcAddress(PUB_Handle, "J_Node_GetDisplayPrecision");
    pJ_Node_GetFloatHasInc              = (tJ_Node_GetFloatHasInc          )GetProcAddress(PUB_Handle, "J_Node_GetFloatHasInc");
    pJ_Node_GetFloatInc                 = (tJ_Node_GetFloatInc             )GetProcAddress(PUB_Handle, "J_Node_GetFloatInc");
    pJ_Node_GetIntAlias                 = (tJ_Node_GetIntAlias             )GetProcAddress(PUB_Handle, "J_Node_GetIntAlias");
    pJ_Node_GetEnumAlias                = (tJ_Node_GetEnumAlias            )GetProcAddress(PUB_Handle, "J_Node_GetEnumAlias");
    pJ_Node_GetNumOfEnumEntries         = (tJ_Node_GetNumOfEnumEntries     )GetProcAddress(PUB_Handle, "J_Node_GetNumOfEnumEntries");
    pJ_Node_GetEnumEntryByIndex         = (tJ_Node_GetEnumEntryByIndex     )GetProcAddress(PUB_Handle, "J_Node_GetEnumEntryByIndex");
    pJ_Node_GetEnumEntryValue           = (tJ_Node_GetEnumEntryValue       )GetProcAddress(PUB_Handle, "J_Node_GetEnumEntryValue");
    pJ_Node_ExecuteCommand              = (tJ_Node_ExecuteCommand          )GetProcAddress(PUB_Handle, "J_Node_ExecuteCommand");
    pJ_Node_GetCommandIsDone            = (tJ_Node_GetCommandIsDone        )GetProcAddress(PUB_Handle, "J_Node_GetCommandIsDone");
    pJ_Node_SetValueString              = (tJ_Node_SetValueString          )GetProcAddress(PUB_Handle, "J_Node_SetValueString");
    pJ_Node_GetValueString              = (tJ_Node_GetValueString          )GetProcAddress(PUB_Handle, "J_Node_GetValueString");
    pJ_Node_SetValueRegister            = (tJ_Node_SetValueRegister        )GetProcAddress(PUB_Handle, "J_Node_SetValueRegister");
    pJ_Node_GetValueRegister            = (tJ_Node_GetValueRegister        )GetProcAddress(PUB_Handle, "J_Node_GetValueRegister");
    pJ_Node_GetRegisterLength           = (tJ_Node_GetRegisterLength       )GetProcAddress(PUB_Handle, "J_Node_GetRegisterLength");
    pJ_Node_GetRegisterAddress          = (tJ_Node_GetRegisterAddress      )GetProcAddress(PUB_Handle, "J_Node_GetRegisterAddress");
    pJ_Image_OpenViewWindowA            = (tJ_Image_OpenViewWindowA        )GetProcAddress(PUB_Handle, "J_Image_OpenViewWindowA");
    pJ_Image_OpenViewWindowW            = (tJ_Image_OpenViewWindowW        )GetProcAddress(PUB_Handle, "J_Image_OpenViewWindowW");
    pJ_Image_OpenViewWindowExA          = (tJ_Image_OpenViewWindowExA      )GetProcAddress(PUB_Handle, "J_Image_OpenViewWindowExA");
    pJ_Image_OpenViewWindowExW          = (tJ_Image_OpenViewWindowExW      )GetProcAddress(PUB_Handle, "J_Image_OpenViewWindowExW");
    pJ_Image_CloseViewWindow            = (tJ_Image_CloseViewWindow        )GetProcAddress(PUB_Handle, "J_Image_CloseViewWindow");
    pJ_Image_ShowImage                  = (tJ_Image_ShowImage              )GetProcAddress(PUB_Handle, "J_Image_ShowImage");
    pJ_Image_ShowImageEx                = (tJ_Image_ShowImageEx            )GetProcAddress(PUB_Handle, "J_Image_ShowImageEx");
    pJ_Image_SetImageOffset             = (tJ_Image_SetImageOffset         )GetProcAddress(PUB_Handle, "J_Image_SetImageOffset");
    pJ_Image_SetImageOffsetEx           = (tJ_Image_SetImageOffsetEx       )GetProcAddress(PUB_Handle, "J_Image_SetImageOffsetEx");
    pJ_Image_GetImageOffsetEx           = (tJ_Image_GetImageOffsetEx       )GetProcAddress(PUB_Handle, "J_Image_GetImageOffsetEx");
    pJ_Image_MoveViewWindow             = (tJ_Image_MoveViewWindow         )GetProcAddress(PUB_Handle, "J_Image_MoveViewWindow");
    pJ_Image_ResizeChildWindow          = (tJ_Image_ResizeChildWindow      )GetProcAddress(PUB_Handle, "J_Image_ResizeChildWindow");
    pJ_Image_GetViewWindowRect          = (tJ_Image_GetViewWindowRect      )GetProcAddress(PUB_Handle, "J_Image_GetViewWindowRect");
    pJ_Image_GetFrameWindowRect         = (tJ_Image_GetFrameWindowRect     )GetProcAddress(PUB_Handle, "J_Image_GetFrameWindowRect");
    pJ_Image_SetViewWindowTitleA        = (tJ_Image_SetViewWindowTitleA    )GetProcAddress(PUB_Handle, "J_Image_SetViewWindowTitleA");
    pJ_Image_SetViewWindowTitleW        = (tJ_Image_SetViewWindowTitleW    )GetProcAddress(PUB_Handle, "J_Image_SetViewWindowTitleW");
    pJ_Image_SetViewWindowOption        = (tJ_Image_SetViewWindowOption    )GetProcAddress(PUB_Handle, "J_Image_SetViewWindowOption");
    pJ_Image_GetViewWindowOption        = (tJ_Image_GetViewWindowOption    )GetProcAddress(PUB_Handle, "J_Image_GetViewWindowOption");
    pJ_Image_OpenStream                 = (tJ_Image_OpenStream             )GetProcAddress(PUB_Handle, "J_Image_OpenStream");
    pJ_Image_OpenStreamLight            = (tJ_Image_OpenStreamLight        )GetProcAddress(PUB_Handle, "J_Image_OpenStreamLight");
    pJ_Image_CloseStream                = (tJ_Image_CloseStream            )GetProcAddress(PUB_Handle, "J_Image_CloseStream");
    pJ_Image_GetStreamInfo              = (tJ_Image_GetStreamInfo          )GetProcAddress(PUB_Handle, "J_Image_GetStreamInfo");
    pJ_Image_GetStreamHandle            = (tJ_Image_GetStreamHandle        )GetProcAddress(PUB_Handle, "J_Image_GetStreamHandle");
    pJ_Image_FromRawToDIB               = (tJ_Image_FromRawToDIB           )GetProcAddress(PUB_Handle, "J_Image_FromRawToDIB");
    pJ_Image_FromRawToDIBEx             = (tJ_Image_FromRawToDIBEx         )GetProcAddress(PUB_Handle, "J_Image_FromRawToDIBEx");
    pJ_Image_FromRawToImage             = (tJ_Image_FromRawToImage         )GetProcAddress(PUB_Handle, "J_Image_FromRawToImage");
    pJ_Image_FromRawToImageEx           = (tJ_Image_FromRawToImageEx       )GetProcAddress(PUB_Handle, "J_Image_FromRawToImageEx");
    pJ_Image_ConvertImage               = (tJ_Image_ConvertImage           )GetProcAddress(PUB_Handle, "J_Image_ConvertImage");
    pJ_Image_Malloc                     = (tJ_Image_Malloc                 )GetProcAddress(PUB_Handle, "J_Image_Malloc");
    pJ_Image_MallocDIB                  = (tJ_Image_MallocDIB              )GetProcAddress(PUB_Handle, "J_Image_MallocDIB");
    pJ_Image_MallocEx                   = (tJ_Image_MallocEx               )GetProcAddress(PUB_Handle, "J_Image_MallocEx");
    pJ_Image_Free                       = (tJ_Image_Free                   )GetProcAddress(PUB_Handle, "J_Image_Free");
    pJ_Image_SaveFileA                  = (tJ_Image_SaveFileA              )GetProcAddress(PUB_Handle, "J_Image_SaveFileA");
    pJ_Image_SaveFileW                  = (tJ_Image_SaveFileW              )GetProcAddress(PUB_Handle, "J_Image_SaveFileW");
    pJ_Image_SaveFileExA                = (tJ_Image_SaveFileExA            )GetProcAddress(PUB_Handle, "J_Image_SaveFileExA");
    pJ_Image_SaveFileExW                = (tJ_Image_SaveFileExW            )GetProcAddress(PUB_Handle, "J_Image_SaveFileExW");
    pJ_Image_SaveFileRawA               = (tJ_Image_SaveFileRawA           )GetProcAddress(PUB_Handle, "J_Image_SaveFileRawA");
    pJ_Image_SaveFileRawW               = (tJ_Image_SaveFileRawW           )GetProcAddress(PUB_Handle, "J_Image_SaveFileRawW");
    pJ_Image_LoadFileRawA               = (tJ_Image_LoadFileRawA           )GetProcAddress(PUB_Handle, "J_Image_LoadFileRawA");
    pJ_Image_LoadFileRawW               = (tJ_Image_LoadFileRawW           )GetProcAddress(PUB_Handle, "J_Image_LoadFileRawW");
    pJ_Image_GetPixel                   = (tJ_Image_GetPixel               )GetProcAddress(PUB_Handle, "J_Image_GetPixel");
    pJ_Image_GetPixelEx                 = (tJ_Image_GetPixelEx             )GetProcAddress(PUB_Handle, "J_Image_GetPixelEx");
    pJ_Image_SetPixel                   = (tJ_Image_SetPixel               )GetProcAddress(PUB_Handle, "J_Image_SetPixel");
    pJ_Image_GetAverage                 = (tJ_Image_GetAverage             )GetProcAddress(PUB_Handle, "J_Image_GetAverage");
    pJ_Image_SetGain                    = (tJ_Image_SetGain                )GetProcAddress(PUB_Handle, "J_Image_SetGain");
    pJ_Image_GetGain                    = (tJ_Image_GetGain                )GetProcAddress(PUB_Handle, "J_Image_GetGain");
    pJ_Image_ExecuteWhiteBalance        = (tJ_Image_ExecuteWhiteBalance    )GetProcAddress(PUB_Handle, "J_Image_ExecuteWhiteBalance");
    pJ_Camera_SaveSettingsA             = (tJ_Camera_SaveSettingsA         )GetProcAddress(PUB_Handle, "J_Camera_SaveSettingsA");
    pJ_Camera_SaveSettingsW             = (tJ_Camera_SaveSettingsW         )GetProcAddress(PUB_Handle, "J_Camera_SaveSettingsW");
    pJ_Camera_LoadSettingsA             = (tJ_Camera_LoadSettingsA         )GetProcAddress(PUB_Handle, "J_Camera_LoadSettingsA");
    pJ_Camera_LoadSettingsW             = (tJ_Camera_LoadSettingsW         )GetProcAddress(PUB_Handle, "J_Camera_LoadSettingsW");
    pJ_Camera_GetSettingsValidationErrorInfo = (tJ_Camera_GetSettingsValidationErrorInfo )GetProcAddress(PUB_Handle, "J_Camera_GetSettingsValidationErrorInfo");
    pJ_Image_MallocHDR                  = (tJ_Image_MallocHDR              )GetProcAddress(PUB_Handle, "J_Image_MallocHDR");
    pJ_Image_AnalyzeHDR                 = (tJ_Image_AnalyzeHDR             )GetProcAddress(PUB_Handle, "J_Image_AnalyzeHDR");
    pJ_Image_FuseHDR                    = (tJ_Image_FuseHDR                )GetProcAddress(PUB_Handle, "J_Image_FuseHDR");
    pJ_Image_MallocLUT                  = (tJ_Image_MallocLUT              )GetProcAddress(PUB_Handle, "J_Image_MallocLUT");
    pJ_Image_FreeLUT                    = (tJ_Image_FreeLUT                )GetProcAddress(PUB_Handle, "J_Image_FreeLUT");
    pJ_Image_ConvertLUT                 = (tJ_Image_ConvertLUT             )GetProcAddress(PUB_Handle, "J_Image_ConvertLUT");
    pJ_Image_LUT                        = (tJ_Image_LUT                    )GetProcAddress(PUB_Handle, "J_Image_LUT");
    pJ_Image_CreateGammaLUT             = (tJ_Image_CreateGammaLUT         )GetProcAddress(PUB_Handle, "J_Image_CreateGammaLUT");
    pJ_Image_ApplyGammaToLUT            = (tJ_Image_ApplyGammaToLUT        )GetProcAddress(PUB_Handle, "J_Image_ApplyGammaToLUT");
    pJ_Image_CreateLinearLUT            = (tJ_Image_CreateLinearLUT        )GetProcAddress(PUB_Handle, "J_Image_CreateLinearLUT");
    pJ_Image_CreateKneeLUT              = (tJ_Image_CreateKneeLUT          )GetProcAddress(PUB_Handle, "J_Image_CreateKneeLUT");
    pJ_Image_GetLUTValue                = (tJ_Image_GetLUTValue            )GetProcAddress(PUB_Handle, "J_Image_GetLUTValue");
    pJ_Image_SetLUTValue                = (tJ_Image_SetLUTValue            )GetProcAddress(PUB_Handle, "J_Image_SetLUTValue");
    pJ_Image_SaveLUTA                   = (tJ_Image_SaveLUTA               )GetProcAddress(PUB_Handle, "J_Image_SaveLUTA");
    pJ_Image_SaveLUTW                   = (tJ_Image_SaveLUTW               )GetProcAddress(PUB_Handle, "J_Image_SaveLUTW");
    pJ_Image_LoadLUTA                   = (tJ_Image_LoadLUTA               )GetProcAddress(PUB_Handle, "J_Image_LoadLUTA");
    pJ_Image_LoadLUTW                   = (tJ_Image_LoadLUTW               )GetProcAddress(PUB_Handle, "J_Image_LoadLUTW");
    pJ_Image_MallocHistogram            = (tJ_Image_MallocHistogram        )GetProcAddress(PUB_Handle, "J_Image_MallocHistogram");
    pJ_Image_FreeHistogram              = (tJ_Image_FreeHistogram          )GetProcAddress(PUB_Handle, "J_Image_FreeHistogram");
    pJ_Image_ClearHistogram             = (tJ_Image_ClearHistogram         )GetProcAddress(PUB_Handle, "J_Image_ClearHistogram");
    pJ_Image_CreateHistogram            = (tJ_Image_CreateHistogram        )GetProcAddress(PUB_Handle, "J_Image_CreateHistogram");
    pJ_Image_GetHistogramValue          = (tJ_Image_GetHistogramValue      )GetProcAddress(PUB_Handle, "J_Image_GetHistogramValue");
    pJ_Image_Flip                       = (tJ_Image_Flip                   )GetProcAddress(PUB_Handle, "J_Image_Flip");
    pJ_Image_Rotate                     = (tJ_Image_Rotate                 )GetProcAddress(PUB_Handle, "J_Image_Rotate");
    pJ_Factory_SendActionCommand        = (tJ_Factory_SendActionCommand    )GetProcAddress(PUB_Handle, "J_Factory_SendActionCommand");
    pJ_Image_Processing                 = (tJ_Image_Processing             )GetProcAddress(PUB_Handle, "J_Image_Processing");
    pJ_Image_ProcessingEx               = (tJ_Image_ProcessingEx           )GetProcAddress(PUB_Handle, "J_Image_ProcessingEx");

    if (!pJ_Factory_Open ||
        !pJ_Factory_Close ||                    
        !pJ_Factory_GetInfo ||
        !pJ_Factory_UpdateCameraList || 
        !pJ_Factory_GetNumOfCameras || 
        !pJ_Factory_GetCameraIDByIndex || 
        !pJ_Factory_GetCameraInfo || 
        !pJ_Factory_EnableForceIp || 
        !pJ_Camera_Open || 
        !pJ_Camera_OpenMc || 
        !pJ_Camera_Close || 
        !pJ_Camera_GetValueInt64 || 
        !pJ_Camera_GetValueDouble || 
        !pJ_Camera_GetValueString  || 
        !pJ_Camera_SetValueInt64 || 
        !pJ_Camera_SetValueDouble || 
        !pJ_Camera_SetValueString || 
        !pJ_Camera_ExecuteCommand || 
        !pJ_Camera_ReadData || 
        !pJ_Camera_WriteData || 
        !pJ_Camera_GetConfigInfo || 
        !pJ_Camera_GetNumOfNodes || 
        !pJ_Camera_GetNodeByIndex || 
        !pJ_Camera_GetNodeByName || 
        !pJ_Camera_GetNumOfSubFeatures || 
        !pJ_Camera_GetSubFeatureByIndex || 
        !pJ_Camera_GetFeatureByName || 
        !pJ_Camera_InvalidateNodes || 
        !pJ_Camera_GetNumOfDataStreams || 
        !pJ_Camera_CreateDataStream || 
        !pJ_Camera_CreateDataStreamMc || 
        !pJ_DataStream_AnnounceBuffer || 
        !pJ_DataStream_FlushQueue || 
        !pJ_DataStream_StartAcquisition || 
        !pJ_DataStream_StopAcquisition || 
        !pJ_DataStream_GetStreamInfo || 
        !pJ_DataStream_GetBufferID || 
        !pJ_DataStream_Close || 
        !pJ_DataStream_RevokeBuffer || 
        !pJ_DataStream_QueueBuffer || 
        !pJ_DataStream_GetBufferInfo || 
        !pJ_DataStream_GetParam || 
        !pJ_DataStream_SetParam || 
        !pJ_Event_GetData || 
        !pJ_Event_GetInfo || 
        !pJ_Event_Flush || 
        !pJ_Event_Close || 
        !pJ_Camera_RegisterEvent || 
        !pJ_Camera_UnRegisterEvent || 
        !pJ_DataStream_RegisterEvent || 
        !pJ_DataStream_UnRegisterEvent || 
        !pJ_Event_CreateCondition || 
        !pJ_Event_SignalCondition || 
        !pJ_Event_ExitCondition || 
        !pJ_Event_ResetCondition || 
        !pJ_Event_WaitForCondition || 
        !pJ_Event_WaitForMultipleConditions || 
        !pJ_Event_CloseCondition || 
        !pJ_Node_GetAccessMode || 
        !pJ_Node_GetName || 
        !pJ_Node_GetNameSpace || 
        !pJ_Node_GetVisibility || 
        !pJ_Node_Invalidate || 
        !pJ_Node_GetIsCachable || 
        !pJ_Node_GetCachingMode || 
        !pJ_Node_GetPollingTime || 
        !pJ_Node_GetToolTip || 
        !pJ_Node_GetDescription || 
        !pJ_Node_GetDisplayName || 
        !pJ_Node_GetEventID || 
        !pJ_Node_GetIsStreamable || 
        !pJ_Node_GetNumOfProperties || 
        !pJ_Node_GetPropertyNameByIndex || 
        !pJ_Node_GetPropertyByName || 
        !pJ_Node_ImposeAccessMode || 
        !pJ_Node_ImposeVisibility || 
        !pJ_Node_GetAlias || 
        !pJ_Node_RegisterCallback || 
        !pJ_Node_GetType  || 
        !pJ_Node_GetIsSelector || 
        !pJ_Node_GetNumOfSelectedFeatures || 
        !pJ_Node_GetSelectedFeatureByIndex || 
        !pJ_Node_GetNumOfSelectingFeatures || 
        !pJ_Node_GetSelectingFeatureByIndex || 
        !pJ_Factory_GetGenICamErrorInfo || 
        !pJ_Node_GetRepresentation || 
        !pJ_Node_GetUnit || 
        !pJ_Node_GetDocuURL || 
        !pJ_Node_GetIsDeprecated || 
        !pJ_Node_GetMinInt64 || 
        !pJ_Node_GetMaxInt64 || 
        !pJ_Node_GetInc || 
        !pJ_Node_SetValueInt64 || 
        !pJ_Node_GetValueInt64 || 
        !pJ_Node_GetFloatAlias || 
        !pJ_Node_GetMinDouble || 
        !pJ_Node_GetMaxDouble || 
        !pJ_Node_SetValueDouble || 
        !pJ_Node_GetValueDouble || 
        !pJ_Node_GetDisplayNotation || 
        !pJ_Node_GetDisplayPrecision || 
        !pJ_Node_GetFloatHasInc || 
        !pJ_Node_GetFloatInc || 
        !pJ_Node_GetIntAlias || 
        !pJ_Node_GetEnumAlias || 
        !pJ_Node_GetNumOfEnumEntries || 
        !pJ_Node_GetEnumEntryByIndex || 
        !pJ_Node_GetEnumEntryValue || 
        !pJ_Node_ExecuteCommand || 
        !pJ_Node_GetCommandIsDone || 
        !pJ_Node_SetValueString || 
        !pJ_Node_GetValueString || 
        !pJ_Node_SetValueRegister || 
        !pJ_Node_GetValueRegister || 
        !pJ_Node_GetRegisterLength || 
        !pJ_Node_GetRegisterAddress || 
        !pJ_Image_OpenViewWindowA ||
        !pJ_Image_OpenViewWindowW || 
        !pJ_Image_OpenViewWindowExA || 
        !pJ_Image_OpenViewWindowExW || 
        !pJ_Image_CloseViewWindow || 
        !pJ_Image_ShowImage || 
        !pJ_Image_ShowImageEx || 
        !pJ_Image_SetImageOffset || 
        !pJ_Image_SetImageOffsetEx || 
        !pJ_Image_GetImageOffsetEx || 
        !pJ_Image_MoveViewWindow || 
        !pJ_Image_ResizeChildWindow || 
        !pJ_Image_GetViewWindowRect || 
        !pJ_Image_GetFrameWindowRect || 
        !pJ_Image_SetViewWindowTitleA || 
        !pJ_Image_SetViewWindowTitleW || 
        !pJ_Image_SetViewWindowOption || 
        !pJ_Image_GetViewWindowOption || 
        !pJ_Image_OpenStream || 
        !pJ_Image_OpenStreamLight || 
        !pJ_Image_CloseStream || 
        !pJ_Image_GetStreamInfo || 
        !pJ_Image_GetStreamHandle || 
        !pJ_Image_FromRawToDIB || 
        !pJ_Image_FromRawToDIBEx || 
        !pJ_Image_FromRawToImage || 
        !pJ_Image_FromRawToImageEx || 
        !pJ_Image_ConvertImage || 
        !pJ_Image_Malloc || 
        !pJ_Image_MallocDIB || 
        !pJ_Image_MallocEx || 
        !pJ_Image_Free || 
        !pJ_Image_SaveFileA || 
        !pJ_Image_SaveFileW || 
        !pJ_Image_SaveFileExA  || 
        !pJ_Image_SaveFileExW || 
        !pJ_Image_SaveFileRawA || 
        !pJ_Image_SaveFileRawW || 
        !pJ_Image_LoadFileRawA || 
        !pJ_Image_LoadFileRawW || 
        !pJ_Image_GetPixel || 
        !pJ_Image_GetPixelEx || 
        !pJ_Image_SetPixel || 
        !pJ_Image_GetAverage || 
        !pJ_Image_SetGain || 
        !pJ_Image_GetGain || 
        !pJ_Image_ExecuteWhiteBalance ||
        !pJ_Camera_SaveSettingsA || 
        !pJ_Camera_SaveSettingsW || 
        !pJ_Camera_LoadSettingsA || 
        !pJ_Camera_LoadSettingsW || 
        !pJ_Camera_GetSettingsValidationErrorInfo ||
        !pJ_Image_MallocHDR ||
        !pJ_Image_AnalyzeHDR || 
        !pJ_Image_FuseHDR || 
        !pJ_Image_MallocLUT || 
        !pJ_Image_FreeLUT || 
        !pJ_Image_ConvertLUT || 
        !pJ_Image_LUT || 
        !pJ_Image_CreateGammaLUT || 
        !pJ_Image_ApplyGammaToLUT || 
        !pJ_Image_CreateLinearLUT || 
        !pJ_Image_CreateKneeLUT ||
        !pJ_Image_GetLUTValue || 
        !pJ_Image_SetLUTValue || 
        !pJ_Image_SaveLUTA || 
        !pJ_Image_SaveLUTW || 
        !pJ_Image_LoadLUTA || 
        !pJ_Image_LoadLUTW || 
        !pJ_Image_MallocHistogram || 
        !pJ_Image_FreeHistogram || 
        !pJ_Image_ClearHistogram || 
        !pJ_Image_CreateHistogram || 
        !pJ_Image_GetHistogramValue || 
        !pJ_Image_Flip || 
        !pJ_Image_Rotate ||
        !pJ_Factory_SendActionCommand ||
        !pJ_Image_Processing ||
        !pJ_Image_ProcessingEx)
	{
		OutputDebugString(_T("GetProcAddress() failed\r\n"));
		FreeLibrary(PUB_Handle);
		return 0;
	}

	return 1;
}

int CloseJaiSdk()
{
	OutputDebugString(_T("CloseJaiSdk Called\r\n"));

    if (PUB_Handle != NULL)
    {
		FreeLibrary(PUB_Handle);
        PUB_Handle=NULL;
    }

    return 1;
}

J_STATUS_TYPE J_Factory_Open(const int8_t* psPrivateData, FACTORY_HANDLE *pHandle)
{
    if (PUB_Handle == NULL)
    {
        if (!OpenJaiSdk())
        {
            return J_ST_ERROR;
        }
    }
    if (pJ_Factory_Open)
        return pJ_Factory_Open(psPrivateData, pHandle);

    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_Close(FACTORY_HANDLE hFac)
{
    J_STATUS_TYPE retsta = J_ST_ERROR;

    if (pJ_Factory_Close != NULL)
    {
        retsta = pJ_Factory_Close(hFac);

        if (!CloseJaiSdk())
        {
            return J_ST_ERROR;
        }
    }

    return retsta;
}

J_STATUS_TYPE J_Factory_GetInfo(J_FACTORY_INFO info, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Factory_GetInfo)
        return pJ_Factory_GetInfo(info, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_UpdateCameraList(FACTORY_HANDLE hFac, bool8_t *bHasChanged)
{
    if (pJ_Factory_UpdateCameraList)
        return pJ_Factory_UpdateCameraList(hFac, bHasChanged);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_GetNumOfCameras(FACTORY_HANDLE hFac, uint32_t *pNum)
{
    if (pJ_Factory_GetNumOfCameras)
        return pJ_Factory_GetNumOfCameras(hFac, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_GetCameraIDByIndex(FACTORY_HANDLE hFac, int32_t iNum, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Factory_GetCameraIDByIndex)
        return pJ_Factory_GetCameraIDByIndex(hFac, iNum, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_GetCameraInfo(FACTORY_HANDLE hFac, int8_t* pCameraId, J_CAMERA_INFO InfoId, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Factory_GetCameraInfo)
        return pJ_Factory_GetCameraInfo(hFac, pCameraId, InfoId, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_EnableForceIp(FACTORY_HANDLE hFac, uint32_t ForceEnabled)
{
    if (pJ_Factory_EnableForceIp)
        return pJ_Factory_EnableForceIp(hFac, ForceEnabled);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_Open(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam)
{
    if (pJ_Camera_Open)
        return pJ_Camera_Open(hFac, pCameraID, hCam);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_OpenMc(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam, J_DEVICE_ACCESS_FLAGS iOpenFlags, uint32_t iMcIP)
{
    if (pJ_Camera_OpenMc)
        return pJ_Camera_OpenMc(hFac, pCameraID, hCam, iOpenFlags, iMcIP);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_Close(CAM_HANDLE hCam)
{
    if (pJ_Camera_Close)
        return pJ_Camera_Close(hCam);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetValueInt64(CAM_HANDLE hCam, int8_t* sNodeName, int64_t *pValue)
{
    if (pJ_Camera_GetValueInt64)
        return pJ_Camera_GetValueInt64(hCam, sNodeName, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetValueDouble(CAM_HANDLE hCam, int8_t* sNodeName, double *pValue)
{
    if (pJ_Camera_GetValueDouble)
        return pJ_Camera_GetValueDouble(hCam, sNodeName, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetValueString(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr, uint32_t* pSize)
{
    if (pJ_Camera_GetValueString)
        return pJ_Camera_GetValueString(hCam, sNodeName, ValueStr, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_SetValueInt64(CAM_HANDLE hCam, int8_t* sNodeName, int64_t Value)
{
    if (pJ_Camera_SetValueInt64)
        return pJ_Camera_SetValueInt64(hCam, sNodeName, Value);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_SetValueDouble(CAM_HANDLE hCam, int8_t* sNodeName, double Value)
{
    if (pJ_Camera_SetValueDouble)
        return pJ_Camera_SetValueDouble(hCam, sNodeName, Value);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_SetValueString(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr)
{
    if (pJ_Camera_SetValueString)
        return pJ_Camera_SetValueString(hCam, sNodeName, ValueStr);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_ExecuteCommand(CAM_HANDLE hCam, int8_t* sNodeName)
{
    if (pJ_Camera_ExecuteCommand)
        return pJ_Camera_ExecuteCommand(hCam, sNodeName);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_ReadData(CAM_HANDLE hCam, int64_t iAddress, void* pData, uint32_t* pSize)
{
    if (pJ_Camera_ReadData)
        return pJ_Camera_ReadData(hCam, iAddress, pData, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_WriteData(CAM_HANDLE hCam, int64_t iAddress, const void* pData, uint32_t* pSize)
{
    if (pJ_Camera_WriteData)
        return pJ_Camera_WriteData(hCam, iAddress, pData, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetConfigInfo(CAM_HANDLE hCam, J_CONFIG_INFO cinfo, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Camera_GetConfigInfo)
        return pJ_Camera_GetConfigInfo(hCam, cinfo, pBuffer, pSize);
        return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetNumOfNodes(CAM_HANDLE hCam, uint32_t* pNum)
{
    if (pJ_Camera_GetNumOfNodes)
        return pJ_Camera_GetNumOfNodes(hCam, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetNodeByIndex(CAM_HANDLE hCam, uint32_t index, NODE_HANDLE* phNode)
{
    if (pJ_Camera_GetNodeByIndex)
        return pJ_Camera_GetNodeByIndex(hCam, index, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetNodeByName(CAM_HANDLE hCam, int8_t* sNodeName, NODE_HANDLE * phNode)
{
    if (pJ_Camera_GetNodeByName)
        return pJ_Camera_GetNodeByName(hCam, sNodeName, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetNumOfSubFeatures(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t *pNum)
{
    if (pJ_Camera_GetNumOfSubFeatures)
        return pJ_Camera_GetNumOfSubFeatures(hCam, sParentNodeName, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetSubFeatureByIndex(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t index, NODE_HANDLE * phNode)
{
    if (pJ_Camera_GetSubFeatureByIndex)
        return pJ_Camera_GetSubFeatureByIndex(hCam, sParentNodeName, index, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetFeatureByName(CAM_HANDLE hCam, int8_t* sFeatureNodeName, NODE_HANDLE* phNode)
{
    if (pJ_Camera_GetFeatureByName)
        return pJ_Camera_GetFeatureByName(hCam, sFeatureNodeName, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_InvalidateNodes(CAM_HANDLE hCam)
{
    if (pJ_Camera_InvalidateNodes)
        return pJ_Camera_InvalidateNodes(hCam);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetNumOfDataStreams(CAM_HANDLE hCam, uint32_t *pNum)
{
    if (pJ_Camera_GetNumOfDataStreams)
        return pJ_Camera_GetNumOfDataStreams(hCam, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_CreateDataStream(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS)
{
    if (pJ_Camera_CreateDataStream)
        return pJ_Camera_CreateDataStream(hCam, iChannel, pDS);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_CreateDataStreamMc(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS, DWORD iMcIP)
{
    if (pJ_Camera_CreateDataStreamMc)
        return pJ_Camera_CreateDataStreamMc(hCam, iChannel, pDS, iMcIP);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_AnnounceBuffer(STREAM_HANDLE hDS, void *pBuffer, uint32_t uiSize, void *pPrivate, BUF_HANDLE *hBuf)
{
    if (pJ_DataStream_AnnounceBuffer)
        return pJ_DataStream_AnnounceBuffer(hDS, pBuffer, uiSize, pPrivate, hBuf);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_FlushQueue(STREAM_HANDLE hDS, J_ACQ_QUEUE_TYPE QueueType)
{
    if (pJ_DataStream_FlushQueue)
        return pJ_DataStream_FlushQueue(hDS, QueueType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_StartAcquisition(STREAM_HANDLE hDS, J_ACQ_START_FLAGS iFlags, uint64_t iNumImages)
{
    if (pJ_DataStream_StartAcquisition)
        return pJ_DataStream_StartAcquisition(hDS, iFlags, iNumImages);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_StopAcquisition(STREAM_HANDLE hDS, J_ACQ_STOP_FLAGS iFlags)
{
    if (pJ_DataStream_StopAcquisition)
        return pJ_DataStream_StopAcquisition(hDS, iFlags);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_GetStreamInfo(STREAM_HANDLE hDS, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize)
{
    if (pJ_DataStream_GetStreamInfo)
        return pJ_DataStream_GetStreamInfo(hDS, iCmd, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_GetBufferID(STREAM_HANDLE hDS, uint32_t iIndex, BUF_HANDLE *hBuf)
{
    if (pJ_DataStream_GetBufferID)
        return pJ_DataStream_GetBufferID(hDS, iIndex, hBuf);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_Close(STREAM_HANDLE hDS)
{
    if (pJ_DataStream_Close)
        return pJ_DataStream_Close(hDS);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_RevokeBuffer(STREAM_HANDLE hDS, BUF_HANDLE hBuf, void **pBuffer, void **pPrivate)
{
    if (pJ_DataStream_RevokeBuffer)
        return pJ_DataStream_RevokeBuffer(hDS, hBuf, pBuffer, pPrivate);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_QueueBuffer(STREAM_HANDLE hDS, BUF_HANDLE hBuf)
{
    if (pJ_DataStream_QueueBuffer)
        return pJ_DataStream_QueueBuffer(hDS, hBuf);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_GetBufferInfo(STREAM_HANDLE hDS, BUF_HANDLE hBuf, J_BUFFER_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize)
{
    if (pJ_DataStream_GetBufferInfo)
        return pJ_DataStream_GetBufferInfo(hDS, hBuf, iCmd, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_GetParam(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize)
{
    if (pJ_DataStream_GetParam)
        return pJ_DataStream_GetParam(hDS, iCmd, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_SetParam(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize)
{
    if (pJ_DataStream_SetParam)
        return pJ_DataStream_SetParam(hDS, iCmd, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_GetData(EVT_HANDLE pEventHandle, void *pBuffer, uint32_t *pSize)
{
    if (pJ_Event_GetData)
        return pJ_Event_GetData(pEventHandle, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_GetInfo(EVT_HANDLE pEventHandle, J_EVENT_INFO_ID iID, void *pBuffer, uint32_t *pSize)
{
    if (pJ_Event_GetInfo)
        return pJ_Event_GetInfo(pEventHandle, iID, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_Flush(EVT_HANDLE pEventHandle)
{
    if (pJ_Event_Flush)
        return pJ_Event_Flush(pEventHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_Close(EVT_HANDLE pEventHandle)
{
    if (pJ_Event_Close)
        return pJ_Event_Close(pEventHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_RegisterEvent(CAM_HANDLE hCam, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle)
{
    if (pJ_Camera_RegisterEvent)
        return pJ_Camera_RegisterEvent(hCam, iEventType, hEvent, pEventHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_UnRegisterEvent(CAM_HANDLE hCam, J_EVENT_TYPE iEventType)
{
    if (pJ_Camera_UnRegisterEvent)
        return pJ_Camera_UnRegisterEvent(hCam, iEventType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_RegisterEvent(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle)
{
    if (pJ_DataStream_RegisterEvent)
        return pJ_DataStream_RegisterEvent(hDS, iEventType, hEvent, pEventHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_DataStream_UnRegisterEvent(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType)
{
    if (pJ_DataStream_UnRegisterEvent)
        return pJ_DataStream_UnRegisterEvent(hDS, iEventType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_CreateCondition(HANDLE* pConditionHandle)
{
    if (pJ_Event_CreateCondition)
        return pJ_Event_CreateCondition(pConditionHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_SignalCondition(HANDLE conditionHandle)
{
    if (pJ_Event_SignalCondition)
        return pJ_Event_SignalCondition(conditionHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_ExitCondition(HANDLE conditionHandle)
{
    if (pJ_Event_ExitCondition)
        return pJ_Event_ExitCondition(conditionHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_ResetCondition(HANDLE conditionHandle)
{
    if (pJ_Event_ResetCondition)
        return pJ_Event_ResetCondition(conditionHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_WaitForCondition(HANDLE conditionHandle, DWORD timeout, J_COND_WAIT_RESULT* pWaitResult)
{
    if (pJ_Event_WaitForCondition)
        return pJ_Event_WaitForCondition(conditionHandle, timeout, pWaitResult);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_WaitForMultipleConditions(uint32_t nCount, const HANDLE* conditionHandles, DWORD timeout, J_COND_WAIT_RESULT* pWaitResult)
{
    if (pJ_Event_WaitForMultipleConditions)
        return pJ_Event_WaitForMultipleConditions(nCount, conditionHandles, timeout, pWaitResult);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Event_CloseCondition(HANDLE conditionHandle)
{
    if (pJ_Event_CloseCondition)
        return pJ_Event_CloseCondition(conditionHandle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetAccessMode(NODE_HANDLE hNode, J_NODE_ACCESSMODE *pValue)
{
    if (pJ_Node_GetAccessMode)
        return pJ_Node_GetAccessMode(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetName(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize, uint32_t FullQualified)
{
    if (pJ_Node_GetName)
        return pJ_Node_GetName(hNode, pBuffer, pSize, FullQualified);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetNameSpace(NODE_HANDLE hNode, J_NODE_NAMESPACE *pValue)
{
    if (pJ_Node_GetNameSpace)
        return pJ_Node_GetNameSpace(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetVisibility(NODE_HANDLE hNode, J_NODE_VISIBILITY* pVisibility)
{
    if (pJ_Node_GetVisibility)
        return pJ_Node_GetVisibility(hNode, pVisibility);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_Invalidate(NODE_HANDLE hNode)
{
    if (pJ_Node_Invalidate)
        return pJ_Node_Invalidate(hNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetIsCachable(NODE_HANDLE hNode, uint32_t* pVal)
{
    if (pJ_Node_GetIsCachable)
        return pJ_Node_GetIsCachable(hNode, pVal);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetCachingMode(NODE_HANDLE hNode, J_NODE_CACHINGMODE *pValue)
{
    if (pJ_Node_GetCachingMode)
        return pJ_Node_GetCachingMode(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetPollingTime(NODE_HANDLE hNode, int64_t *pValue)
{
    if (pJ_Node_GetPollingTime)
        return pJ_Node_GetPollingTime(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetToolTip(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetToolTip)
        return pJ_Node_GetToolTip(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetDescription(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetDescription)
        return pJ_Node_GetDescription(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetDisplayName(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetDisplayName)
        return pJ_Node_GetDisplayName(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetEventID(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetEventID)
        return pJ_Node_GetEventID(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetIsStreamable(NODE_HANDLE hNode, uint32_t* pVal)
{
    if (pJ_Node_GetIsStreamable)
        return pJ_Node_GetIsStreamable(hNode, pVal);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetNumOfProperties(NODE_HANDLE hNode, uint32_t* pNum)
{
    if (pJ_Node_GetNumOfProperties)
        return pJ_Node_GetNumOfProperties(hNode, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetPropertyNameByIndex(NODE_HANDLE hNode, uint32_t index, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetPropertyNameByIndex)
        return pJ_Node_GetPropertyNameByIndex(hNode, index, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetPropertyByName(NODE_HANDLE hNode, const int8_t* PropertyName, int8_t* ValueStr, uint32_t* pSizeValueStr, int8_t*AttributeStr, uint32_t* pSizeAttributeStr)
{
    if (pJ_Node_GetPropertyByName)
        return pJ_Node_GetPropertyByName(hNode, PropertyName, ValueStr, pSizeValueStr, AttributeStr, pSizeAttributeStr);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_ImposeAccessMode(NODE_HANDLE hNode, J_NODE_ACCESSMODE AccessMode)
{
    if (pJ_Node_ImposeAccessMode)
        return pJ_Node_ImposeAccessMode(hNode, AccessMode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_ImposeVisibility(NODE_HANDLE hNode, J_NODE_VISIBILITY Visibility)
{
    if (pJ_Node_ImposeVisibility)
        return pJ_Node_ImposeVisibility(hNode, Visibility);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetAlias(NODE_HANDLE hNode, NODE_HANDLE* phAliasNode)
{
    if (pJ_Node_GetAlias)
        return pJ_Node_GetAlias(hNode, phAliasNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_RegisterCallback(NODE_HANDLE hNode, J_NODE_CALLBACK_FUNCTION CallBackFunction)
{
    if (pJ_Node_RegisterCallback)
        return pJ_Node_RegisterCallback(hNode, CallBackFunction);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetType(NODE_HANDLE hNode, J_NODE_TYPE* pNodeType)
{
    if (pJ_Node_GetType)
        return pJ_Node_GetType(hNode, pNodeType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetIsSelector(NODE_HANDLE hNode, uint32_t* pValue)
{
    if (pJ_Node_GetIsSelector)
        return pJ_Node_GetIsSelector(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetNumOfSelectedFeatures(NODE_HANDLE hNode, uint32_t *pNum)
{
    if (pJ_Node_GetNumOfSelectedFeatures)
        return pJ_Node_GetNumOfSelectedFeatures(hNode, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetSelectedFeatureByIndex(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectedNode)
{
    if (pJ_Node_GetSelectedFeatureByIndex)
        return pJ_Node_GetSelectedFeatureByIndex(hNode, index, hSelectedNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetNumOfSelectingFeatures(NODE_HANDLE hNode, uint32_t *pNum)
{
    if (pJ_Node_GetNumOfSelectingFeatures)
        return pJ_Node_GetNumOfSelectingFeatures(hNode, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetSelectingFeatureByIndex(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectingNode)
{
    if (pJ_Node_GetSelectingFeatureByIndex)
        return pJ_Node_GetSelectingFeatureByIndex(hNode, index, hSelectingNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_GetGenICamErrorInfo(tGenICamErrorInfo* gc)
{
    if (pJ_Factory_GetGenICamErrorInfo)
        return pJ_Factory_GetGenICamErrorInfo(gc);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetRepresentation(NODE_HANDLE hNode, J_NODE_REPRESENTATION *pValue)
{
    if (pJ_Node_GetRepresentation)
        return pJ_Node_GetRepresentation(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetUnit(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetUnit)
        return pJ_Node_GetUnit(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetDocuURL(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize)
{
    if (pJ_Node_GetDocuURL)
        return pJ_Node_GetDocuURL(hNode, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetIsDeprecated(NODE_HANDLE hNode, uint32_t* pVal)
{
    if (pJ_Node_GetIsDeprecated)
        return pJ_Node_GetIsDeprecated(hNode, pVal);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetMinInt64(NODE_HANDLE hNode, int64_t* pValue)
{
    if (pJ_Node_GetMinInt64)
        return pJ_Node_GetMinInt64(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetMaxInt64(NODE_HANDLE hNode, int64_t* pValue)
{
    if (pJ_Node_GetMaxInt64)
        return pJ_Node_GetMaxInt64(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetInc(NODE_HANDLE hNode,int64_t *pValue)
{
    if (pJ_Node_GetInc)
        return pJ_Node_GetInc(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_SetValueInt64(NODE_HANDLE hNode, bool Verify, int64_t Value)
{
    if (pJ_Node_SetValueInt64)
        return pJ_Node_SetValueInt64(hNode, Verify, Value);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetValueInt64(NODE_HANDLE hNode, bool Verify, int64_t* pValue)
{
    if (pJ_Node_GetValueInt64)
        return pJ_Node_GetValueInt64(hNode, Verify, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetFloatAlias(NODE_HANDLE hNode, NODE_HANDLE* phNode)
{
    if (pJ_Node_GetFloatAlias)
        return pJ_Node_GetFloatAlias(hNode, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetMinDouble(NODE_HANDLE hNode, double* pValue)
{
    if (pJ_Node_GetMinDouble)
        return pJ_Node_GetMinDouble(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetMaxDouble(NODE_HANDLE hNode, double* pValue)
{
    if (pJ_Node_GetMaxDouble)
        return pJ_Node_GetMaxDouble(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_SetValueDouble(NODE_HANDLE hNode, bool Verify, double Value)
{
    if (pJ_Node_SetValueDouble)
        return pJ_Node_SetValueDouble(hNode, Verify, Value);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetValueDouble(NODE_HANDLE hNode, bool Verify, double* pValue)
{
    if (pJ_Node_GetValueDouble)
        return pJ_Node_GetValueDouble(hNode, Verify, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetDisplayNotation(NODE_HANDLE hNode, J_NODE_DISPLAY_NOTATION *pValue)
{
    if (pJ_Node_GetDisplayNotation)
        return pJ_Node_GetDisplayNotation(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetDisplayPrecision(NODE_HANDLE hNode, int64_t *pValue)
{
    if (pJ_Node_GetDisplayPrecision)
        return pJ_Node_GetDisplayPrecision(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetFloatHasInc(NODE_HANDLE hNode, uint32_t*pValue)
{
    if (pJ_Node_GetFloatHasInc)
        return pJ_Node_GetFloatHasInc(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetFloatInc(NODE_HANDLE hNode, double* pValue)
{
    if (pJ_Node_GetFloatInc)
        return pJ_Node_GetFloatInc(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetIntAlias(NODE_HANDLE hNode, NODE_HANDLE* phNode)
{
    if (pJ_Node_GetIntAlias)
        return pJ_Node_GetIntAlias(hNode, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetEnumAlias(NODE_HANDLE hNode, NODE_HANDLE* phNode)
{
    if (pJ_Node_GetEnumAlias)
        return pJ_Node_GetEnumAlias(hNode, phNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetNumOfEnumEntries(NODE_HANDLE hEnumNode, uint32_t *pNum)
{
    if (pJ_Node_GetNumOfEnumEntries)
        return pJ_Node_GetNumOfEnumEntries(hEnumNode, pNum);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetEnumEntryByIndex(NODE_HANDLE hEnumNode, uint32_t index, NODE_HANDLE* hEnumEntryNode)
{
    if (pJ_Node_GetEnumEntryByIndex)
        return pJ_Node_GetEnumEntryByIndex(hEnumNode, index, hEnumEntryNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetEnumEntryValue(NODE_HANDLE hEnumNode, int64_t* pValue)
{
    if (pJ_Node_GetEnumEntryValue)
        return pJ_Node_GetEnumEntryValue(hEnumNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_ExecuteCommand(NODE_HANDLE hNode)
{
    if (pJ_Node_ExecuteCommand)
        return pJ_Node_ExecuteCommand(hNode);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetCommandIsDone(NODE_HANDLE hNode, uint32_t* pValue)
{
    if (pJ_Node_GetCommandIsDone)
        return pJ_Node_GetCommandIsDone(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_SetValueString(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr)
{
    if (pJ_Node_SetValueString)
        return pJ_Node_SetValueString(hNode, Verify, ValueStr);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetValueString(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr, uint32_t* pSize)
{
    if (pJ_Node_GetValueString)
        return pJ_Node_GetValueString(hNode, Verify, ValueStr, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_SetValueRegister(NODE_HANDLE hNode, const uint8_t *pBuffer, int64_t Length)
{
    if (pJ_Node_SetValueRegister)
        return pJ_Node_SetValueRegister(hNode, pBuffer, Length);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetValueRegister(NODE_HANDLE hNode, uint8_t *pBuffer, int64_t Length)
{
    if (pJ_Node_GetValueRegister)
        return pJ_Node_GetValueRegister(hNode, pBuffer, Length);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetRegisterLength(NODE_HANDLE hNode, int64_t *pValue)
{
    if (pJ_Node_GetRegisterLength)
        return pJ_Node_GetRegisterLength(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Node_GetRegisterAddress(NODE_HANDLE hNode, int64_t *pValue)
{
    if (pJ_Node_GetRegisterAddress)
        return pJ_Node_GetRegisterAddress(hNode, pValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenViewWindowA(LPCSTR pWindowName, POINT *pPoint, SIZE *pMaxSize, VIEW_HANDLE *pWin)
{
    if (pJ_Image_OpenViewWindowA)
        return pJ_Image_OpenViewWindowA(pWindowName, pPoint, pMaxSize, pWin);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenViewWindowW(LPCWSTR pWindowName, POINT *pPoint, SIZE *pMaxSize, VIEW_HANDLE *pWin)
{
    if (pJ_Image_OpenViewWindowW)
        return pJ_Image_OpenViewWindowW(pWindowName, pPoint, pMaxSize, pWin);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenViewWindowExA(J_IVW_WINDOW_TYPE iWindowType, LPCSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin)
{
    if (pJ_Image_OpenViewWindowExA)
        return pJ_Image_OpenViewWindowExA(iWindowType, pWindowName, pFrameRect, pMaxSize, hParent, pWin);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenViewWindowExW(J_IVW_WINDOW_TYPE iWindowType, LPCWSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin)
{
    if (pJ_Image_OpenViewWindowExW)
        return pJ_Image_OpenViewWindowExW(iWindowType, pWindowName, pFrameRect, pMaxSize, hParent, pWin);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CloseViewWindow(VIEW_HANDLE hWin)
{
    if (pJ_Image_CloseViewWindow)
        return pJ_Image_CloseViewWindow(hWin);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ShowImage(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_ShowImage)
        return pJ_Image_ShowImage(hWin, pAqImageInfo, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ShowImageEx(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_ShowImageEx)
        return pJ_Image_ShowImageEx(hWin, pAqImageInfo, iBayerAlgorithm, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetImageOffset(VIEW_HANDLE hWin, POINT *pOffset, int32_t ZoomRatio)
{
    if (pJ_Image_SetImageOffset)
        return pJ_Image_SetImageOffset(hWin, pOffset, ZoomRatio);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetImageOffsetEx(VIEW_HANDLE hWin, POINT *pOffset, double ZoomRatioX, double ZoomRatioY)
{
    if (pJ_Image_SetImageOffsetEx)
        return pJ_Image_SetImageOffsetEx(hWin, pOffset, ZoomRatioX, ZoomRatioY);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetImageOffsetEx(VIEW_HANDLE hWin, POINT *pOffset, double *pZoomRatioX, double *pZoomRatioY)
{
    if (pJ_Image_GetImageOffsetEx)
        return pJ_Image_GetImageOffsetEx(hWin, pOffset, pZoomRatioX, pZoomRatioY);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MoveViewWindow(VIEW_HANDLE hWin, POINT *pPoint)
{
    if (pJ_Image_MoveViewWindow)
        return pJ_Image_MoveViewWindow(hWin, pPoint);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ResizeChildWindow(VIEW_HANDLE hWin, RECT *pRect)
{
    if (pJ_Image_ResizeChildWindow)
        return pJ_Image_ResizeChildWindow(hWin, pRect);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetViewWindowRect(VIEW_HANDLE hWin, RECT *pRect)
{
    if (pJ_Image_GetViewWindowRect)
        return pJ_Image_GetViewWindowRect(hWin, pRect);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetFrameWindowRect(VIEW_HANDLE hWin, RECT *pRect)
{
    if (pJ_Image_GetFrameWindowRect)
        return pJ_Image_GetFrameWindowRect(hWin, pRect);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetViewWindowTitleA(VIEW_HANDLE hWin, LPCSTR pWindowTitle)
{
    if (pJ_Image_SetViewWindowTitleA)
        return pJ_Image_SetViewWindowTitleA(hWin, pWindowTitle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetViewWindowTitleW(VIEW_HANDLE hWin, LPCWSTR pWindowTitle)
{
    if (pJ_Image_SetViewWindowTitleW)
        return pJ_Image_SetViewWindowTitleW(hWin, pWindowTitle);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetViewWindowOption(VIEW_HANDLE hWin, J_IVW_OPTION Option, void* pOptionValue)
{
    if (pJ_Image_SetViewWindowOption)
        return pJ_Image_SetViewWindowOption(hWin, Option, pOptionValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetViewWindowOption(VIEW_HANDLE hWin, J_IVW_OPTION Option, void* pOptionValue)
{
    if (pJ_Image_GetViewWindowOption)
        return pJ_Image_GetViewWindowOption(hWin, Option, pOptionValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenStream(CAM_HANDLE hCam, uint32_t iChannel, J_IMG_CALLBACK_OBJECT CBObject, J_IMG_CALLBACK_FUNCTION CBFunction, THRD_HANDLE * phThread, uint32_t iBufferSize, DWORD iMcIP)
{
    if (pJ_Image_OpenStream)
        return pJ_Image_OpenStream(hCam, iChannel, CBObject, CBFunction, phThread, iBufferSize, iMcIP);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_OpenStreamLight(CAM_HANDLE hCam, uint32_t iChannel,  THRD_HANDLE* phThread)
{
    if (pJ_Image_OpenStreamLight)
        return pJ_Image_OpenStreamLight(hCam, iChannel,  phThread);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CloseStream(THRD_HANDLE hThread)
{
    if (pJ_Image_CloseStream)
        return pJ_Image_CloseStream(hThread);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetStreamInfo(THRD_HANDLE hThread, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize)
{
    if (pJ_Image_GetStreamInfo)
        return pJ_Image_GetStreamInfo(hThread, iCmd, pBuffer, pSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetStreamHandle(THRD_HANDLE hThread, STREAM_HANDLE *phStream)
{
    if (pJ_Image_GetStreamHandle)
        return pJ_Image_GetStreamHandle(hThread, phStream);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FromRawToDIB(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_FromRawToDIB)
        return pJ_Image_FromRawToDIB(pAqImageInfo, pBufferInfo, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FromRawToDIBEx(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_FromRawToDIBEx)
        return pJ_Image_FromRawToDIBEx(pAqImageInfo, pBufferInfo, iBayerAlgorithm, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FromRawToImage(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_FromRawToImage)
        return pJ_Image_FromRawToImage(pAqImageInfo, pBufferInfo, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FromRawToImageEx(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_FromRawToImageEx)
        return pJ_Image_FromRawToImageEx(pAqImageInfo, pBufferInfo, iBayerAlgorithm, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ConvertImage(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat)
{
    if (pJ_Image_ConvertImage)
        return pJ_Image_ConvertImage(pInputBufferInfo, pOutputBufferInfo, iOutputFormat);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_Malloc(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo)
{
    if (pJ_Image_Malloc)
        return pJ_Image_Malloc(pAqImageInfo, pBufferInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MallocDIB(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo)
{
    if (pJ_Image_MallocDIB)
        return pJ_Image_MallocDIB(pAqImageInfo, pBufferInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MallocEx(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat)
{
    if (pJ_Image_MallocEx)
        return pJ_Image_MallocEx(pInputBufferInfo, pOutputBufferInfo, iOutputFormat);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_Free(J_tIMAGE_INFO * pBufferInfo)
{
    if (pJ_Image_Free)
        return pJ_Image_Free(pBufferInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileA(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath)
{
    if (pJ_Image_SaveFileA)
        return pJ_Image_SaveFileA(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileW(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath)
{
    if (pJ_Image_SaveFileW)
        return pJ_Image_SaveFileW(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileExA(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath, J_SIF_FILE_FORMAT iFileFormat,  uint8_t iEncoderParameter)
{
    if (pJ_Image_SaveFileExA)
        return pJ_Image_SaveFileExA(pBufferInfo, pPath, iFileFormat,  iEncoderParameter);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileExW(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath, J_SIF_FILE_FORMAT iFileFormat,  uint8_t iEncoderParameter)
{
    if (pJ_Image_SaveFileExW)
        return pJ_Image_SaveFileExW(pBufferInfo, pPath, iFileFormat, iEncoderParameter);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileRawA(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath)
{
    if (pJ_Image_SaveFileRawA)
        return pJ_Image_SaveFileRawA(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveFileRawW(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath)
{
    if (pJ_Image_SaveFileRawW)
        return pJ_Image_SaveFileRawW(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_LoadFileRawA(J_tIMAGE_INFO * pBufferInfo, LPCSTR pPath)
{
    if (pJ_Image_LoadFileRawA)
        return pJ_Image_LoadFileRawA(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_LoadFileRawW(J_tIMAGE_INFO * pBufferInfo, LPCWSTR pPath)
{
    if (pJ_Image_LoadFileRawW)
        return pJ_Image_LoadFileRawW(pBufferInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetPixel(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel)
{
    if (pJ_Image_GetPixel)
        return pJ_Image_GetPixel(pBufferInfo, pPoint, pPixel);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetPixelEx(J_tIMAGE_INFO * pBufferInfo, double dXPos, double dYPos, void * pPixel)
{
    if (pJ_Image_GetPixelEx)
        return pJ_Image_GetPixelEx(pBufferInfo, dXPos, dYPos, pPixel);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetPixel(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel)
{
    if (pJ_Image_SetPixel)
        return pJ_Image_SetPixel(pBufferInfo, pPoint, pPixel);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetAverage(J_tIMAGE_INFO * pBufferInfo, RECT *pMeasureRect, J_tBGR48 *pRGBAverage)
{
    if (pJ_Image_GetAverage)
        return pJ_Image_GetAverage(pBufferInfo, pMeasureRect, pRGBAverage);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetGain(HANDLE hThread, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain)
{
    if (pJ_Image_SetGain)
        return pJ_Image_SetGain(hThread, iRGain, iGGain, iBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetGain(HANDLE hThread, uint32_t * piRGain, uint32_t * piGGain, uint32_t * piBGain)
{
    if (pJ_Image_GetGain)
        return pJ_Image_GetGain(hThread, piRGain, piGGain, piBGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ExecuteWhiteBalance(THRD_HANDLE hThread)
{
    if (pJ_Image_ExecuteWhiteBalance)
        return pJ_Image_ExecuteWhiteBalance(hThread);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_SaveSettingsA(CAM_HANDLE hCam, LPCSTR sSettingsFileName, J_SAVE_SETTINGS_FLAG saveFlag)
{
    if (pJ_Camera_SaveSettingsA)
        return pJ_Camera_SaveSettingsA(hCam, sSettingsFileName, saveFlag);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_SaveSettingsW(CAM_HANDLE hCam, LPCWSTR sSettingsFileName, J_SAVE_SETTINGS_FLAG saveFlag)
{
    if (pJ_Camera_SaveSettingsW)
        return pJ_Camera_SaveSettingsW(hCam, sSettingsFileName, saveFlag);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_LoadSettingsA(CAM_HANDLE hCam, LPCSTR sSettingsFileName, J_LOAD_SETTINGS_FLAG loadFlag)
{
    if (pJ_Camera_LoadSettingsA)
        return pJ_Camera_LoadSettingsA(hCam, sSettingsFileName, loadFlag);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_LoadSettingsW(CAM_HANDLE hCam, LPCWSTR sSettingsFileName, J_LOAD_SETTINGS_FLAG loadFlag)
{
    if (pJ_Camera_LoadSettingsW)
        return pJ_Camera_LoadSettingsW(hCam, sSettingsFileName, loadFlag);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Camera_GetSettingsValidationErrorInfo(CAM_HANDLE hCam, int8_t* errorInfoBuffer, uint32_t *pBufferSize)
{
    if (pJ_Camera_GetSettingsValidationErrorInfo)
        return pJ_Camera_GetSettingsValidationErrorInfo(hCam, errorInfoBuffer, pBufferSize);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MallocHDR(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, J_tIMAGE_INFO * pBufferInfo)
{
    if (pJ_Image_MallocHDR)
        return pJ_Image_MallocHDR(pImageInfo1, pImageInfo2, pBufferInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_AnalyzeHDR(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, uint8_t* pImage1IsBrighter, int32_t *pBlackLevelImage1, int32_t *pBlackLevelImage2, float* pDarkGain)
{
    if (pJ_Image_AnalyzeHDR)
        return pJ_Image_AnalyzeHDR(pImageInfo1, pImageInfo2, pImage1IsBrighter, pBlackLevelImage1, pBlackLevelImage2, pDarkGain);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FuseHDR(J_tIMAGE_INFO* pBrighterImageInfo, J_tIMAGE_INFO* pDarkerImageInfo, J_tIMAGE_INFO* pOutputBufferInfo, int32_t brighterBlackLevel, int32_t darkerBlackLevel, float fDarkGain, float fDualSlopeGain, uint8_t logOutput)
{
    if (pJ_Image_FuseHDR)
        return pJ_Image_FuseHDR(pBrighterImageInfo, pDarkerImageInfo, pOutputBufferInfo, brighterBlackLevel, darkerBlackLevel, fDarkGain, fDualSlopeGain, logOutput);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MallocLUT(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo)
{
    if (pJ_Image_MallocLUT)
        return pJ_Image_MallocLUT(pImageInfo, pLutInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FreeLUT(J_tLUT* pLutInfo)
{
    if (pJ_Image_FreeLUT)
        return pJ_Image_FreeLUT(pLutInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ConvertLUT(J_tLUT* pSourceLutInfo, J_tLUT* pDestinationLutInfo)
{
    if (pJ_Image_FreeLUT)
        return pJ_Image_ConvertLUT(pSourceLutInfo, pDestinationLutInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_LUT(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo)
{
    if (pJ_Image_LUT)
        return pJ_Image_LUT(pImageInfo, pLutInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CreateGammaLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma)
{
    if (pJ_Image_CreateGammaLUT)
        return pJ_Image_CreateGammaLUT(pLutInfo, ColorIndex, gamma);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ApplyGammaToLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma)
{
    if (pJ_Image_ApplyGammaToLUT)
        return pJ_Image_ApplyGammaToLUT(pLutInfo, ColorIndex, gamma);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CreateLinearLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t X1, uint32_t X2, uint32_t Y1, uint32_t Y2)
{
    if (pJ_Image_CreateLinearLUT)
        return pJ_Image_CreateLinearLUT(pLutInfo, ColorIndex, X1, X2, Y1, Y2);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CreateKneeLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, POINT knee1, POINT knee2)
{
    if (pJ_Image_CreateKneeLUT)
        return pJ_Image_CreateKneeLUT(pLutInfo, ColorIndex, knee1, knee2);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetLUTValue(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t* pLUTValue)
{
    if (pJ_Image_GetLUTValue)
        return pJ_Image_GetLUTValue(pLutInfo, ColorIndex, LUTIndex, pLUTValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SetLUTValue(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t LUTValue)
{
    if (pJ_Image_SetLUTValue)
        return pJ_Image_SetLUTValue(pLutInfo, ColorIndex, LUTIndex, LUTValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveLUTA(J_tLUT* pLutInfo, LPCSTR pPath)
{
    if (pJ_Image_SaveLUTA)
        return pJ_Image_SaveLUTA(pLutInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_SaveLUTW(J_tLUT* pLutInfo, LPCWSTR pPath)
{
    if (pJ_Image_SaveLUTW)
        return pJ_Image_SaveLUTW(pLutInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_LoadLUTA(J_tLUT* pLutInfo, LPCSTR pPath)
{
    if (pJ_Image_LoadLUTA)
        return pJ_Image_LoadLUTA(pLutInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_LoadLUTW(J_tLUT* pLutInfo, LPCWSTR pPath)
{
    if (pJ_Image_LoadLUTW)
        return pJ_Image_LoadLUTW(pLutInfo, pPath);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_MallocHistogram(J_tIMAGE_INFO* pImageInfo, J_tHIST* pHistogramInfo)
{
    if (pJ_Image_MallocHistogram)
        return pJ_Image_MallocHistogram(pImageInfo, pHistogramInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_FreeHistogram(J_tHIST* pHistogramInfo)
{
    if (pJ_Image_FreeHistogram)
        return pJ_Image_FreeHistogram(pHistogramInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ClearHistogram(J_tHIST* pHistogramInfo)
{
    if (pJ_Image_ClearHistogram)
        return pJ_Image_ClearHistogram(pHistogramInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_CreateHistogram(J_tIMAGE_INFO* pImageInfo, J_tHIST* pHistogramInfo)
{
    if (pJ_Image_CreateHistogram)
        return pJ_Image_CreateHistogram(pImageInfo, pHistogramInfo);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_GetHistogramValue(J_tHIST* pHistogramInfo, uint32_t ColorIndex, uint32_t HistogramIndex, uint32_t* pHistogramValue)
{
    if (pJ_Image_GetHistogramValue)
        return pJ_Image_GetHistogramValue(pHistogramInfo, ColorIndex, HistogramIndex, pHistogramValue);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_Flip(J_tIMAGE_INFO* pImageInfo, J_IMAGE_FLIP_TYPE flipType)
{
    if (pJ_Image_Flip)
        return pJ_Image_Flip(pImageInfo, flipType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_Rotate(J_tIMAGE_INFO* pImageInfo, J_IMAGE_ROTATE_TYPE rotateType)
{
    if (pJ_Image_Rotate)
        return pJ_Image_Rotate(pImageInfo, rotateType);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Factory_SendActionCommand(FACTORY_HANDLE hFac, uint32_t iDeviceKey, uint32_t iGroupKey, uint32_t iGroupMask, J_tACTION_CMD_ACK* pAckBuffer, uint32_t* piNumOfAck)
{
    if (pJ_Factory_SendActionCommand)
        return pJ_Factory_SendActionCommand(hFac, iDeviceKey, iGroupKey, iGroupMask, pAckBuffer, piNumOfAck);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_Processing(J_tIMAGE_INFO* pImageInfo, J_IMAGE_PROCESSING iProcessFunction, void * pParameters)
{
    if (pJ_Image_Processing)
        return pJ_Image_Processing(pImageInfo, iProcessFunction, pParameters);
    return J_ST_ERROR;
}

J_STATUS_TYPE J_Image_ProcessingEx(J_tIMAGE_INFO* pInputImageInfo, J_tIMAGE_INFO* pOutputImageInfo, J_IMAGE_PROCESSING iProcessFunction, void * pParameters)
{
    if (pJ_Image_ProcessingEx)
        return pJ_Image_ProcessingEx(pInputImageInfo, pOutputImageInfo, iProcessFunction, pParameters);
    return J_ST_ERROR;
}
