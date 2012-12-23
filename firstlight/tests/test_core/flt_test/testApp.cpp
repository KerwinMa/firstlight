#include "testApp.h"

//test mods
#include "testMods/MaterialTest.h"
#include "testMods/XLibTest.h"
#include "testMods/2DTest.h"
#include "testMods/PicDancerTest.h"
#include "testMods/UITest.h"
#include "testMods/Box2DTest.h"
#include "testMods/TerrianTest.h"

#define ADD_TEST(tmClass) \
	testMod = new tmClass(m_devicePtr); \
	m_testMods.push_back(testMod);

//============================

TestApp::TestApp()
{
	Logger::getInstance().enableGroup("test",true);
	//Logger::getInstance().enableGroup("engineDbg_RS",true);
	Logger::getInstance().enableGroup("engineDbg_Res",true);
	//Logger::getInstance().enableGroup("engineDbg_Batch",true);
	//Logger::getInstance().enableGroup("engineDbg1",true);	
	Logger::getInstance().enableGroup("engineDbg_UI",true);
};

TestApp::~TestApp()
{
	DBGL("test","delete TestApp");
};

bool TestApp::onInit()
{		
	DBG("test","on init TestApp");

	m_devicePtr->setWindowTitle(L"FLT Test");
	m_devicePtr->getRenderer()->setClearColor(renderer::Color(255,100,100,200));
	for(int i=0; i<m_otherDevices.size(); ++i)
		m_otherDevices[i]->getRenderer()->setClearColor(renderer::Color(255,255,255,200));


	//---------Install test mods-------------------------
	ITestMod *testMod=0;

	//ADD_TEST(MaterialTest);

	//ADD_TEST(PicDancerTest);

	//ADD_TEST(Flt2DTest);	

	//ADD_TEST(XLibTest);	

	//ADD_TEST(UITest);

	//ADD_TEST(Box2DTest);

	ADD_TEST(TerrianTest);

	DBG("test","FLT Test App,%d test mods installed.",m_testMods.size());
	
	
	//--------------------------------
	//‘ÿ»ÎRes Archive
	//ArchiveManager::getInstance().load(".", archive_type::FILE_SYSTEM);
	ArchiveManager::getInstance().load("./res", archive_type::FILE_SYSTEM);
	
	//-----------init test mods-------------

	array_t<ITestMod*>::iterator iter = m_testMods.begin();
	for(;iter!=m_testMods.end();++iter)
	{
		ITestMod *tm = *iter;
		tm->init();
	}

	// init debug font
	renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();
	XFontManager &fontMgr = XFontManager::getInstance();

	m_debugFont = fontMgr.getRes("font.xsp");
	m_debugFont->setName("DebugFont");
	m_debugFont->SetTexture(texMgr.getRes("font.tga"));
	m_debugFont->SetScale(0.8f,0.8f);
	m_debugFont->SetColor(renderer::Color(255,255,255,255));

	return true;
}

void TestApp::onExit()
{
	
	array_t<ITestMod*>::iterator iter = m_testMods.begin();
	for(;iter!=m_testMods.end();++iter)
	{
		ITestMod *tm = *iter;
		tm->destroy();
		delete tm;
	}

	m_testMods.clear();

	DBG("test","on exit TestApp");
}

void TestApp::onSuspend()
{

}

void TestApp::onResume()
{

}

void TestApp::onResizeWindow()
{

}

bool TestApp::onEvent(const IEvent& event)
{		

	if(event.getUID()==events_id::EVT_KEYBOARD)
	{

		EvtKeyboard* evtKb = (EvtKeyboard*)&event;
		//DBG("test","key code=%d, pressed=%s, char=%c",evtKb->KeyCode, evtKb->Pressed?"true":"false", evtKb->Char);
		if(evtKb->KeyCode==flt::KEY_ESCAPE)
		{
			setExitApp();
		}
		else if(evtKb->KeyCode==flt::KEY_KEY_A)
		{
			EvtGame evt_game(0,0,0);
			EventManager::getInstance().raiseEvent(evt_game);
		}
		
		if(evtKb->Pressed==false)
		{
			if(evtKb->KeyCode>=flt::KEY_KEY_1 && evtKb->KeyCode<=flt::KEY_KEY_9)
			{
				int idx = evtKb->KeyCode - flt::KEY_KEY_1;
				if(idx<m_testMods.size())
				{
					m_testMods[idx]->switchEnable();
				}
			}
		}
	}
	else if(event.getUID()==events_id::EVT_TOUCH)
	{
		//EvtTouch* evtTouch = (EvtTouch*)&event;
		//DBG("test","Touch event: id:%ld phase:%d, x:%d, y:%d", (long)evtTouch->ID, evtTouch->Phase, evtTouch->X, evtTouch->Y);
		return false;
	}

	//else if(event.getUID()!=events_id::EVT_MOUSE_MOVE)
	//	DBG("test","event-id:%d,size:%d",event.getUID(),event.getSize());

	return true;//absorb event
}

void TestApp::update(f32 dt)
{
	array_t<ITestMod*>::iterator iter = m_testMods.begin();
	for(;iter!=m_testMods.end();++iter)
	{
		ITestMod *tm = *iter;
		tm->update(dt);		
	}	
}

void TestApp::render()
{
	render(m_devicePtr->getRenderer());

	for(int i=0; i<m_otherDevices.size(); ++i)
		render(m_otherDevices[i]->getRenderer());		
}

void TestApp::render(const flt::renderer::IRendererPtr& renderer)
{
	renderer->makeCurrent();
	renderer->clearBuffer();
	renderer->beginRender();


	array_t<ITestMod*>::iterator iter = m_testMods.begin();
	for(;iter!=m_testMods.end();++iter)
	{
		ITestMod *tm = *iter;
		tm->render(renderer);
	}

#ifdef FLT_DEBUG
	////show fps
	//{
	//	char fpsBuf[256];
	//	sprintf(fpsBuf,"FPS:%.2f", m_profiler.getFPS());
	//
	//	renderer->begin2DMode();
	//	m_debugFont->DrawTextLine(renderer,fpsBuf,0,0);
	//	renderer->end2DMode();
	//}
#endif

	renderer->endRender();
	renderer->swapBuffer();
}

void TestApp::postUpdate(f32 dt)
{

}