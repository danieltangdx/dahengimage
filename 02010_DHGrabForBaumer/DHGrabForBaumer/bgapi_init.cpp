
#include "bgapi.hpp"
#include <vector>
using namespace std;
#include "bgapi_init.h"
#include <stdio.h>
#include <stdlib.h>

int init_systems(int * system_count, vector<BGAPI::System*> * externppSystem)
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	int i = 0;

	//this is the base call to find the bgapi_system modules which are necessary to perform any further action
	//every BGPAPI_Function returns a BGAPI_RESULT
	res = BGAPI::countSystems( system_count );

	//You should always check the result to make sure everything works fine
	if( res != BGAPI_RESULT_OK )
	{
		//in case of error you will get a result different from BGAPI_RESULT_OK 
		//all resultcodes are defined in bgapiresult.h and are returned for special reasons
		printf( "BGAPI_CountSystems Errorcode: %d system_count %d\n", res, *system_count );
		return res;
	}

	for( i = 0; i < *system_count; i++ )
	{
		BGAPI::System * pSystem = NULL;
		res = BGAPI::createSystem( i, &pSystem );
		if( res != BGAPI_RESULT_OK )
		{
			printf("BGAPI::CreateSystem Errorcode: %d Systemnumber %d SysPointer 0x%p\n", res, i, (void*)pSystem );
			externppSystem->clear();
			return res;
		}
		res = pSystem->open();		
		if( res != BGAPI_RESULT_OK )
		{
			printf( "BGAPI::System::open Systemnumber %d Errorcode: %d\n", i, res );
			externppSystem->clear();
			return res;
		}
		externppSystem->push_back( pSystem );
	}
	return res;
}

int init_camera( int system_count, vector<BGAPI::System*> * externppSystem, int * pCurrSystem, BGAPI::Camera ** externppCamera )
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	int cam = 0;
	int camera_count = 0;
	vector<int> cameras;
//	vector<int>::iterator camIter;
	BGAPI_FeatureState state;
	BGAPIX_CameraInfo cameradeviceinfo;
	int inputVal = 0;
	vector<BGAPI::System*>::iterator systemIter;

	for( systemIter = externppSystem->begin(); systemIter != externppSystem->end(); systemIter++ )
	{		
		int count = 0;
		//this is an example how to count available cameras for all available systems
		res = (*systemIter)->countCameras( &count );		
		if( res != BGAPI_RESULT_OK )
		{
			printf( "BGAPI::System::countCameras Systemnumber %d Errorcode: %d\n", systemIter - externppSystem->begin(), res );
			return res;
		}
		cameras.push_back( count );

		for( cam = 0; cam < count; cam++ )
		{
			camera_count++;

			//this is an example how to create a camera
			res = (*systemIter)->createCamera( cam, externppCamera );
			if( res != BGAPI_RESULT_OK )
			{
				printf("\n");
				printf( "BGAPI::System::createCamera Systemnumber %d Errorcode: %d\n", systemIter - externppSystem->begin(), res );
				return res;
			}

			//this is an example how to get the device information for a camera
			state.cbSize = sizeof( BGAPI_FeatureState );
			cameradeviceinfo.cbSize = sizeof( BGAPIX_CameraInfo );
			res = (*externppCamera)->getDeviceInformation( &state, &cameradeviceinfo );
			if( res != BGAPI_RESULT_OK )
			{	
				printf("\n");
				printf( "BGAPI::Camera::getDeviceInformation Errorcode: %d\n", res );
				return res;
			}
			printf("%d select Camera %d of system %d - %s SN: %s\n", camera_count, cam, systemIter - externppSystem->begin(), cameradeviceinfo.modelName, cameradeviceinfo.serialNumber );
			(*systemIter)->releaseCamera( *externppCamera );
		}
	}

	do
	{
		fflush( stdin );fflush( stdout );
		scanf( "%d", &inputVal );
	}
	while( inputVal < 0 || inputVal > camera_count );

	camera_count = 0;
	for( systemIter = externppSystem->begin(); systemIter != externppSystem->end(); systemIter++ )
	{		
		for( cam = 0; cam < cameras[systemIter - externppSystem->begin()]; cam++ )
		{
			camera_count++;
			if( camera_count == inputVal )
			{	
				*pCurrSystem = (int)(systemIter - externppSystem->begin());

				//this is an example how to create a camera
				res = (*externppSystem)[*pCurrSystem]->createCamera( cam, externppCamera );
				if( res != BGAPI_RESULT_OK )
				{
					printf("\n");
					printf( "BGAPI::System::createCamera Systemnumber %d Errorcode: %d\n", systemIter - externppSystem->begin(), res );
					return res;
				}

				//this is an example how to open a camera
				res = (*externppCamera)->open();
				if( res != BGAPI_RESULT_OK )
				{
					printf("\n");
					printf( "BGAPI::Camera::open Systemnumber %d Errorcode: %d\n", *pCurrSystem, res );
					return res;
				}
				break;
			}
		}
	}
	return res;
}

int release_systems( vector<BGAPI::System*> * externppSystem )
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	vector<BGAPI::System*>::iterator systemIter;

	for( systemIter = externppSystem->begin(); systemIter != externppSystem->end(); systemIter++ )
	{
		res = (*systemIter)->release();
		if( res != BGAPI_RESULT_OK )
		{
			printf( "BGAPI::System::release %d Errorcode: %d\n", systemIter - externppSystem->begin(), (int)res );
		}
	}
	externppSystem->clear();
	return res;
}
int release_images( vector<BGAPI::Image*> * ppImage )
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	vector<BGAPI::Image*>::iterator imageIter;
	bool tmpExtern = false;
	unsigned char* tmpBuffer = NULL;
	
	for( imageIter = ppImage->begin(); imageIter != ppImage->end(); imageIter++ )
	{
		res = ((BGAPI::Image*)(*imageIter))->isExternBuffer( &tmpExtern );
		if( res != BGAPI_RESULT_OK )
		{
			printf( "BGAPI::Image::isExternBuffer %d Errorcode: %d\n", imageIter - ppImage->begin(), (int)res );
		}
		
		if( tmpExtern )
		{
			res = ((BGAPI::Image*)(*imageIter))->getBuffer( &tmpBuffer );
			if( res != BGAPI_RESULT_OK )
			{
				printf( "BGAPI::Image::getBuffer %d Errorcode: %d\n", imageIter - ppImage->begin(), (int)res );
			}
			else
			{
				free( tmpBuffer );
			}
		}
		res = BGAPI::releaseImage( *imageIter );
		if( res != BGAPI_RESULT_OK )
		{
			printf( "BGAPI::releaseImage %d Errorcode: %d\n", imageIter - ppImage->begin(), (int)res );
		}
	}
	ppImage->clear();
	return res;
}
