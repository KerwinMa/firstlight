#ifndef FLT_STUB_DEVICE_H
#define FLT_STUB_DEVICE_H

#include "IDevice.h"
#include "renderer/IRenderer.h"

namespace flt
{	
namespace device
{

class StubDevice: public IDevice
{
public:		
	explicit StubDevice(const RenderDeviceAttribute& deviceAttr);
	virtual ~StubDevice(){}
			
	virtual renderer::IRendererPtr& getRenderer();	

	virtual void onWindowSizeChange(u32 width, u32 height);

	virtual u32 getDeviceWidth() const;

	virtual u32 getDeviceHeight() const;

	virtual void resetRand(u32 seed);

	virtual void resetRand();

	virtual s32 getRandS32() const;

	virtual s32 getRandS32(s32 min, s32 max) const;

	virtual f32 getRandClampf() const;

	virtual void setOrientation(EDeviceOrientation newOrientation);	

	virtual EDeviceOrientation getOrientation() const;
	

protected:	
	renderer::IRendererPtr m_renderer; //only support one renderer at runtime
	RenderDeviceAttribute m_deviceAttr;

	static long long s_rndx;
	static long long s_rndy;
	static long long s_rndz;
	static long long s_rndw;	
};

} //end namespace device
} //end namespace flt

#endif //FLT_STUB_DEVICE_H