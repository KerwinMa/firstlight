#ifndef __TEST_APP__
#define __TEST_APP__

#include "ITestMod.h"

using namespace flt;
using namespace utils;
using namespace renderer;

class TestApp: public App
{
public:
	TestApp();

	~TestApp();	

	virtual bool onInit();	
	
	virtual void onExit();

	virtual void onSuspend();

	virtual void onResume();

	virtual void onResizeWindow();

	//! IEventReceiver interface
	virtual bool onEvent(const IEvent& event);

	virtual void update(f32 dt);
	virtual void render();	
	virtual void postUpdate(f32 dt);

	void addDevice(flt::device::IDevicePtr device)
	{
		m_otherDevices.push_back(device);
	}

private:
	

	//multi-device (window) support
	//only for test here, app is for game and  not support multi-windows

	void render(const flt::renderer::IRendererPtr& renderer);

	array_t<flt::device::IDevicePtr> m_otherDevices;

	array_t<ITestMod*> m_testMods;	

	CXFontPtr m_debugFont;
};

#endif //__TEST_APP__