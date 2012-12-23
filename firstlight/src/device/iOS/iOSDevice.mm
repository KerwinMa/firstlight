#include "config.h"
#include "utils/Logger.h"

#if defined(FLT_DEVICE_IOS)

#include "iOSDevice.h"
#import <sys/time.h>
#include "core/dimension2d.h"
//#include "event/EvtMouse.h"
//#include "event/EvtKeyboard.h"
#include "event/EventManager.h"


namespace flt
{	

namespace renderer
{

#if defined(FLT_COMPILE_WITH_OPENGLES1)
	IRendererPtr createOpenGLES1Renderer(const core::dimension2di& screenSize);
#endif

}

namespace device
{

iOSDevice::iOSDevice(const RenderDeviceAttribute& deviceAttr)
	:StubDevice(deviceAttr)
{
	createRenderer();	

	resetRand();

	LOG("engine","iOSDevice created.");
	LOG("engine","screen=%dX%d, bits=%d",m_deviceAttr.width,m_deviceAttr.height,m_deviceAttr.colorBits);
}

iOSDevice::~iOSDevice()
{
	LOG("engine","iOSDevice shut down.");
}

void iOSDevice::createRenderer()
{		
	switch(m_deviceAttr.rendererType)
	{

	case renderer::RENDERER_OPENGLES1:
	{
#if defined(FLT_COMPILE_WITH_OPENGLES1)
		
		m_renderer = renderer::createOpenGLES1Renderer(core::dimension2di(m_deviceAttr.width,m_deviceAttr.height));
		
		if(m_renderer.isValid())
		{
			LOG("engine","OpenGLES1 renderer created.");
		}
		else
		{
			LOG("engine","Failed to create OpenGLES1 renderer!");
		}
#else
		LOG_OUT("engine","OpenGL ES1.1 renderer not compiled!");
#endif
	}
		break;

	default:
		LOG("engine","Error: renderer not supported by this device!");
		break;

	}
		
}

bool iOSDevice::run()
{

	bool quit = false;
	
	return !quit;
}

void iOSDevice::close()
{

}

f64 iOSDevice::getSystemTime() const
{
	timeval tv;
	gettimeofday(&tv, 0);
	static unsigned long s_timeOfGameStarted = (unsigned long)tv.tv_sec;
	unsigned long second = tv.tv_sec - s_timeOfGameStarted;
	return (f64)((second*1000)+(tv.tv_usec/1000.0));
}

void iOSDevice::sleep(u32 timeMs)
{
	timespec ts;
	ts.tv_sec = (time_t)(timeMs/1000);
	ts.tv_nsec = (long)(timeMs%1000)*1000000;
	
	nanosleep(&ts, 0);
}

void iOSDevice::setWindowTitle(const stringw& title)
{
    //Not support
}
	
s32 iOSDevice::getWindowID() const 
{ 
	return 0; 
}
	
void iOSDevice::getWindowRect(s32& left, s32& top, s32& width, s32& height) const
{
	//Not support
}

} //end namespace device


device::IDevicePtr createDevice(const device::RenderDeviceAttribute& renderDeviceAttr)
{
	device::IDevicePtr devicePtr = new device::iOSDevice(renderDeviceAttr);	

	return devicePtr;
}


} //end namespace flt

#endif //FLT_DEVICE_IOS
