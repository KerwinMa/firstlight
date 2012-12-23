#include "StubDevice.h"
#include "ui/UIManager.h"

namespace flt
{	
namespace device
{

long long StubDevice::s_rndx;
long long StubDevice::s_rndy;
long long StubDevice::s_rndz;
long long StubDevice::s_rndw;

StubDevice::StubDevice(const RenderDeviceAttribute& deviceAttr)
:m_deviceAttr(deviceAttr)
{	
}

renderer::IRendererPtr& StubDevice::getRenderer()
{
	return m_renderer;
}

void StubDevice::onWindowSizeChange(u32 width, u32 height)
{
	m_deviceAttr.width = width;
	m_deviceAttr.height = height;
	m_renderer->onSizeChange(width, height);
	flt::ui::UIManager::getInstance().onScreenSizeChanged(width, height);
}

u32 StubDevice::getDeviceWidth() const
{
	return m_deviceAttr.width;
}

u32 StubDevice::getDeviceHeight() const
{
	return m_deviceAttr.height;
}

void StubDevice::resetRand(u32 seed)
{
	s_rndx = seed;
	s_rndy = 842502087L;
	s_rndz = 3579807591L;
	s_rndw = 273326509L;
}

void StubDevice::resetRand()
{
	resetRand(getSystemTime());
}

s32 StubDevice::getRandS32() const
{
	long long t	=(s_rndx^(s_rndx<<11));
	s_rndx	= s_rndy; s_rndy = s_rndz; s_rndz = s_rndw;
	s_rndw	=(s_rndw^(s_rndw>>19))^(t^(t>>8));

	long long rtn = s_rndw&0x7FFFFFFF;
	if (rtn == 0x7FFFFFFF)
		return getRandS32();

	return (s32)rtn;	
}

s32 StubDevice::getRandS32(s32 min, s32 max) const
{
	if (min > max)
	{
		int t		= min;
		min	= max;
		max	= t;
	}

	long long rnd	= getRandS32() >> 2;
	s32	 range		= max - min;
	
	s32 res = (s32)(min + (rnd % range));
	FLT_ASSERT(res >= min && res < max);

	return res;
}

f32 StubDevice::getRandClampf() const
{
	return getRandS32(0,256)/255.0f;
}

void StubDevice::setOrientation(EDeviceOrientation newOrientation)
{	
	m_renderer->setOrientation(newOrientation);
}

EDeviceOrientation StubDevice::getOrientation() const
{
	return m_renderer->getOrientation();
}


} //end namespace device
} //end namespace flt