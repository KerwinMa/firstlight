#ifndef ITEST_MOD_H
#define ITEST_MOD_H

#include "firstlight.h"

using namespace flt;

class ITestMod
{
public:
	ITestMod(const device::IDevicePtr& devicePtr):
	  m_devicePtr(devicePtr),
	  m_enable(true)
	{

	}

	virtual ~ITestMod(){}

	virtual bool init() = 0;

	virtual void destroy() = 0;

	virtual void update(float dt) = 0;

	virtual void render(const renderer::IRendererPtr& renderer) = 0;

	void enable(bool enable)
	{
		m_enable = enable;
	}

	bool isEnable()
	{
		return m_enable;
	}

	void switchEnable()
	{
		m_enable = !m_enable;
	}

protected:
	device::IDevicePtr m_devicePtr;
	bool m_enable;
};

#endif //ITEST_MOD_H
