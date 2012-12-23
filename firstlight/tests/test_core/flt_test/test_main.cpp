#include "firstlight.h"
#include <iostream>

#ifdef FLT_DEBUG
#include "vld.h"
#endif

#include "testApp.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

FLT_IMPLEMENT_ENGINE

//#define TEST_MULTI_DEVICE

int main(int argc, char* argv[])
{		
	FLT_IMPLEMENT_APP(TestApp)	

	device::RenderDeviceAttribute rda;
	rda.width = 960;//800;
	rda.height = 640;//600;
	rda.colorBits = 32;
	rda.rendererType = renderer::RENDERER_OPENGLES1;

	device::IDevicePtr device1 = createDevice(rda);

#ifdef TEST_MULTI_DEVICE
	rda.shareDevice = device1;
	rda.width = 480;
	rda.height = 320;
	
	device::IDevicePtr device2 = createDevice(rda);

	rda.width = 320;
	rda.height = 480;
	device::IDevicePtr device3 = createDevice(rda);
#endif

	if(App::TheApp->init(device1, k_max_fps_default, -1))
	{
		TestApp* app = (TestApp*)App::TheApp.get();

#ifdef TEST_MULTI_DEVICE
		app->addDevice(device2);
		app->addDevice(device3);
#endif

		App::TheApp->run();
	}		
	
	return 0;
}

