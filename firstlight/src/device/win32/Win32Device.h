#ifndef FLT_WIN32_DEVICE_H
#define FLT_WIN32_DEVICE_H

#include "config.h"

#if defined(FLT_DEVICE_WIN32)

#include "device/StubDevice.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace flt
{	
namespace device
{
class Win32Device: public StubDevice
{
public:		

	explicit Win32Device(const RenderDeviceAttribute& deviceAttr);

	virtual ~Win32Device();	

	virtual s32 getWindowID() const;

	virtual void getWindowRect(s32& left, s32& top, s32& width, s32& height) const;

	virtual bool run();

	virtual void close();

	virtual f64 getSystemTime() const;

	virtual void sleep(u32 timeMs);

	virtual void setWindowTitle(const stringw& title);

	virtual void setOrientation(EDeviceOrientation newOrientation);	

protected:

	void createRenderer();

	bool m_useExternalWindow;

	HWND m_hWnd;

	s32 m_winLeft, m_winTop, m_winWidth, m_winHeight;

	f64 m_secsPerTick;

	void adjustWindowSize(u32 newWidth, u32 newHeight);

};

} //end namespace device
} //end namespace flt

#endif //FLT_DEVICE_WIN32
#endif //FLT_WIN32_DEVICE_H