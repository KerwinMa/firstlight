#ifndef FLT_IOS_DEVICE_H
#define FLT_IOS_DEVICE_H

#include "config.h"

#if defined(FLT_DEVICE_IOS)

#include "device/StubDevice.h"


namespace flt
{	
namespace device
{
class iOSDevice: public StubDevice
{
public:		

	explicit iOSDevice(const RenderDeviceAttribute& deviceAttr);

	virtual ~iOSDevice();	

	virtual bool run();

	virtual void close();

	virtual f64 getSystemTime() const;

	virtual void sleep(u32 timeMs);

	virtual void setWindowTitle(const stringw& title);
	
	virtual s32 getWindowID() const;
	
	virtual void getWindowRect(s32& left, s32& top, s32& width, s32& height) const;

protected:

	void createRenderer();

};

} //end namespace device
} //end namespace flt

#endif //FLT_DEVICE_IOS
#endif //FLT_IOS_DEVICE_H