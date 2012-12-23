#ifndef FLT_DEVICE_DEFS_H
#define FLT_DEVICE_DEFS_H

#include "common.h"
#include "renderer/RendererDefs.h"

namespace flt
{	
namespace device
{

struct RenderDeviceAttribute
{
	renderer::RENDERER_TYPE rendererType;
	u32 width;
	u32 height;
	u32 colorBits;
	bool fullscreen;
	bool vsync;
	void* nativeWindowHandle;
	void* shareDevice;

	RenderDeviceAttribute():
		rendererType(renderer::RENDERER_OPENGLES1),
		width(320),
		height(240),
		colorBits(16),
		fullscreen(false),
		vsync(false),
		nativeWindowHandle(0),
		shareDevice(0)
	{
	}
};		

} //end namespace device
} //end namespace flt

#endif //FLT_DEVICE_DEFS_H