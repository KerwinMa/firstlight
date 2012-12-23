#include "app/App.h"
#include "event/EventManager.h"
#include "event/EventsDef.h"
#include "utils/Logger.h"

namespace flt
{

AppPtr App::TheApp;

const f32 k_max_fps_default = 80;
const f32 k_fixed_update_fps_default = 32;

u64 App::s_frameNum = 0;

App::App()
	:m_bWantExit(false),
	 m_bPaused(false),
	 m_maxFPS(k_max_fps_default),
	 m_minFrameTime(1000.0f/k_max_fps_default),
	 m_fixedUpdateDT(-1),
	 m_lastFrameSystemTime(0.0)
{
}

App::~App()
{
}

bool App::init(const device::IDevicePtr& devicePtr, f32 maxFPS, f32 fixedUpdateFPS)
{
	m_devicePtr = devicePtr;
	m_windowWidth = m_devicePtr->getDeviceWidth();
	m_windowHeight = m_devicePtr->getDeviceHeight();

	if(maxFPS>0)
	{
		m_maxFPS = maxFPS;
		m_minFrameTime = 1000.0f/m_maxFPS;
	}

	if(fixedUpdateFPS>0)
	{
		m_fixedUpdateDT = 1000.0f/fixedUpdateFPS;
	}
	
	EventManager& eventMgr = EventManager::getInstance();

#ifdef FLT_DEVICE_WIN32	
	eventMgr.registerReceiver(events_id::EVT_KEYBOARD, this);
	//eventMgr.registerReceiver(events_id::EVT_MOUSE_MOVE, this);
	//eventMgr.registerReceiver(events_id::EVT_MOUSE_BUTTON, this);
	//eventMgr.registerReceiver(events_id::EVT_MOUSE_WHEEL, this);
#endif
	eventMgr.registerReceiver(events_id::EVT_TOUCH, this);
	eventMgr.registerReceiver(events_id::EVT_GAME, this);
	
	//init last frame time
	m_lastFrameSystemTime = m_devicePtr->getSystemTime();

	return onInit();
}
	
void App::stepFrame()
{
	f64 frameTimeBegin = m_devicePtr->getSystemTime();
	
	EventManager::getInstance().update();
	
	if(m_fixedUpdateDT>0)
	{
		//use fixed update time
		update(m_fixedUpdateDT);
		render();
		postUpdate(m_fixedUpdateDT);
	}
	else
	{
		//update use real time
		f32 dt = static_cast<f32>(frameTimeBegin-m_lastFrameSystemTime);
		if(dt<0)
			dt=0.0f;
		m_lastFrameSystemTime = frameTimeBegin;
		
		update(dt);
		render();
		postUpdate(dt);
	}

	s_frameNum++;

	m_profiler.onFrame(m_devicePtr->getSystemTime());
	
	//limit FPS	
	{
		f64 frameTime = m_devicePtr->getSystemTime()-frameTimeBegin;

		f32 minTime = m_minFrameTime;
		if(m_fixedUpdateDT>0)
			minTime = m_fixedUpdateDT;

		u32 sleepTime = static_cast<u32>((frameTime>=minTime)?0:(minTime-frameTime));
		if(sleepTime<=0)
			sleepTime = 5;//sleep 5ms at least		
		m_devicePtr->sleep(sleepTime);		
	}	
}	
	
void App::close()
{
	onExit();
	
	EventManager& eventMgr = EventManager::getInstance();
	eventMgr.unRegisterAllReceivers();
	
	m_devicePtr->close();
}

void App::run()
{
	while(m_devicePtr->run() && !m_bWantExit)
	{
		stepFrame();
	}

	close();
}


void App::suspend()
{
	if(m_bPaused)
		return;

	m_bPaused = true;
	onSuspend();
}

void App::resume()
{
	if(!m_bPaused)
		return;

	m_bPaused = false;
	onResume();
}

void App::resizeWindow(u32 newWidth, u32 newHeight)
{
	if(newWidth<=0 || newHeight<=0)
		return;

	m_windowWidth = newWidth;
	m_windowHeight = newHeight;	

	onResizeWindow();
}

void App::onRotate(EDeviceOrientation newOrientation)
{
	m_devicePtr->setOrientation(newOrientation);
}

} //end namespace flt