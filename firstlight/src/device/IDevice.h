#ifndef FLT_I_DEVICE_H
#define FLT_I_DEVICE_H

#include "common.h"
#include "BaseObject.h"
#include "device/DeviceDefs.h"
#include "sharePtr.h"

namespace flt
{	

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)
}

namespace device
{	
	FLT_FORWARD_PTR(IDevice)	

	class IDevice: public BaseObject
	{
	public:		
		virtual ~IDevice(){}				

		virtual bool run()=0;

		virtual void close()=0;

		virtual renderer::IRendererPtr& getRenderer() = 0;

		virtual void onWindowSizeChange(u32 width, u32 height) = 0;

		virtual u32 getDeviceWidth() const =0;

		virtual u32 getDeviceHeight() const =0;

		virtual s32 getWindowID() const =0;

		virtual void getWindowRect(s32& left, s32& top, s32& width, s32& height) const = 0;

		virtual f64 getSystemTime() const = 0;

		virtual void sleep(u32 timeMs) = 0;

		virtual void setWindowTitle(const stringw& title) = 0;

		//random number support

		virtual void resetRand(u32 seed) = 0;

		virtual void resetRand() = 0;

		virtual s32 getRandS32() const = 0;

		virtual s32 getRandS32(s32 min, s32 max) const = 0;

		virtual f32 getRandClampf() const = 0;		

		virtual void setOrientation(EDeviceOrientation newOrientation) = 0;	

		virtual EDeviceOrientation getOrientation() const = 0;
	};

} //end namespace device
} //end namespace flt

#endif //FLT_I_DEVICE_H