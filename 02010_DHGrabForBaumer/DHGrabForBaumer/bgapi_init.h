#ifndef __BGAPI_INIT_H__
#define __BGAPI_INIT_H__

int init_systems(int * system_count, vector<BGAPI::System*> * externppSystem);
int init_camera(int system_count, vector<BGAPI::System*> * externppSystem, int * pCurrSystem, BGAPI::Camera ** externppCamera);
int release_systems( vector<BGAPI::System*> * ppSystem );
int release_images( vector<BGAPI::Image*> * ppImage );

#endif // __BGAPI_INIT_H__
