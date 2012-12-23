#ifndef FLT_APP_H
#define FLT_APP_H

#include "common.h"
#include "device/IDevice.h"
#include "event/IEventReceiver.h"
#include "Profiler.h"

namespace flt
{

FLT_FORWARD_PTR(App)

extern const f32 k_max_fps_default;
extern const f32 k_fixed_update_fps_default;

class App: public BaseObject, public IEventReceiver
{
public:
	App();
	virtual ~App();

	// the unique application object
	static AppPtr TheApp;	

	//maxFPS: limit max FPS to avoid too high CPU occupy if maxFPS>0; if maxFPS<=0, don't limit FPS
	//			and if use fix fps, maxFPS in ignored
	//fixedUpdateFPS: use fixed update dt, if -1 use real update dt
	bool init(const device::IDevicePtr& devicePtr, f32 maxFPS=k_max_fps_default, f32 fixedUpdateFPS=k_fixed_update_fps_default);

	device::IDevicePtr& getDevice() { return m_devicePtr; } ;
	
	void run();	
	
	void stepFrame();
	
	void close();

	void suspend();
	void resume();
	void resizeWindow(u32 newWidth, u32 newHeight);		
	bool isPaused() { return m_bPaused; }
	bool isWantExit() { return m_bWantExit; }
	void setExitApp() { m_bWantExit=true; }

	u32 getWindowWidth() { return m_windowWidth; }
	u32 getWindowHeight() { return m_windowHeight; }	

	virtual bool onInit()=0;
	virtual void onExit()=0;
	virtual void onSuspend()=0;
	virtual void onResume()=0;
	virtual void onResizeWindow()=0;

	virtual void update(f32 dt)=0;
	virtual void render()=0;
	virtual void postUpdate(f32 dt)=0;

	//! IEventReceiver interface
	virtual bool onEvent(const IEvent& event) = 0;

	virtual void onRotate(EDeviceOrientation newOrientation);

	virtual void onPinchGesture(f32 scale){}
		
protected:
	u32 m_windowWidth;
	u32 m_windowHeight;

	device::IDevicePtr m_devicePtr;

	Profiler m_profiler;

private:
	bool m_bWantExit;
	bool m_bPaused;
	f32 m_maxFPS;
	f32 m_minFrameTime; // 1000.0f/m_maxFPS
	f32 m_fixedUpdateDT;
	f64 m_lastFrameSystemTime;

public:
	//for debug
	static u64 s_frameNum;
};

} //end namespace flt

#define FLT_IMPLEMENT_APP(AppClassName) flt::App::TheApp = new AppClassName;


#endif //FLT_APP_H