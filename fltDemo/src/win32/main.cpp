#include "firstlight.h"
#include <iostream>

#ifdef FLT_DEBUG
#include "vld.h"
#endif

#include "GameApp.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

FLT_IMPLEMENT_ENGINE


int main(int argc, char* argv[])
{		
	FLT_IMPLEMENT_APP(GameApp)	

	device::RenderDeviceAttribute rda;
	rda.width = 640;//800;
	rda.height = 960;//600;
	rda.colorBits = 32;
	rda.rendererType = renderer::RENDERER_OPENGLES1;

	device::IDevicePtr device1 = createDevice(rda);


	if(App::TheApp->init(device1, k_max_fps_default, -1))
	{
		//GameApp* app = (GameApp*)App::TheApp.get();

		App::TheApp->run();
	}		

	return 0;
}

