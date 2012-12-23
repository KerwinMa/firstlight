#include "GameApp.h"
#include "game_states/GameStates.h"
#include "Level.h"


GameApp::GameApp()
	:m_uiMainMenu(0),m_uiInterface(0),m_pCurLevel(0)
{
	Logger::getInstance().enableGroup(MY_PROJECT_NAME,true);
	
	//Logger::getInstance().enableGroup("engineDbg_RS",true);
	Logger::getInstance().enableGroup("engineDbg_Res",true);
	//Logger::getInstance().enableGroup("engineDbg_Batch",true);
	//Logger::getInstance().enableGroup("engineDbg1",true);	
	Logger::getInstance().enableGroup("engineDbg_UI",true);

	m_pFSM = new StateMachine<GameApp>(this);	
};

GameApp::~GameApp()
{
	SAFE_DEL(m_pFSM);

	DBG(MY_PROJECT_NAME,"On delete GameApp");
};

bool GameApp::onInit()
{		
	DBG(MY_PROJECT_NAME,"On init GameApp");

	m_devicePtr->setOrientation(EDO_LandscapeLeft);
	m_devicePtr->setWindowTitle(MY_WIN_NAME);
	m_devicePtr->getRenderer()->setClearColor(renderer::Color(255,255,255,255));	


	//--------------------------------
	//Archive, for win32 working dir is OUT_DIR	
	IArchive* arch = ArchiveManager::getInstance().load("./res", archive_type::FILE_SYSTEM);
	FLT_ASSERT(arch);

	

	// init debug font
	renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();
	XFontManager &fontMgr = XFontManager::getInstance();

	m_debugFont = fontMgr.getRes("font_dbg.xsp");
	m_debugFont->setName("DebugFont");
	m_debugFont->SetTexture(texMgr.getRes("font_dbg.tga"));
	m_debugFont->SetScale(0.6f,0.8f);
	m_debugFont->SetColor(renderer::Color(255,0,0,255));		

	// load sprites
	XSpriteManager &sprMgr = XSpriteManager::getInstance();
	CXSpritePtr spr = sprMgr.getRes("ui.xsp");	

	// load UI
	loadUIs();

	//register events
	EventManager::getInstance().registerReceiver(events_id::EVT_UI_BUTTON_TAPPED, this);
	EventManager::getInstance().registerReceiver(events_id::EVT_UI_SLIDE, this);
	EventManager::getInstance().registerReceiver(events_id::EVT_UI_HOLD, this);
#ifdef FLT_DEVICE_WIN32
	EventManager::getInstance().registerReceiver(events_id::EVT_MOUSE_WHEEL, this);
#endif

	m_pFSM->setFirstState(GSMainMenu::getInstance());//for test

	return true;
}

void GameApp::onExit()
{
	SAFE_DEL(m_pCurLevel);

	unloadUIs();

	DBG(MY_PROJECT_NAME,"On exit GameApp");
}

void GameApp::onSuspend()
{

}

void GameApp::onResume()
{
	if(!m_pFSM->isInState(*GSLevelPause::getInstance()))
		m_pFSM->changeState(GSLevelPause::getInstance());
}

void GameApp::onResizeWindow()
{

}

bool GameApp::onEvent(const IEvent& event)
{
	bool absorb = false;

#ifdef FLT_DEVICE_WIN32
	if(event.getUID()==events_id::EVT_KEYBOARD)
	{
		EvtKeyboard* evtKb = (EvtKeyboard*)&event;		
		if(evtKb->KeyCode==flt::KEY_ESCAPE)
		{
			setExitApp();
			absorb = true;
		}
	}
#endif

	if(!absorb)
		absorb = m_pFSM->processEvent(event);

	return absorb;
}

void GameApp::update(f32 dt)
{
	m_pFSM->update(dt);
}

void GameApp::render()
{
	IRendererPtr& renderer = m_devicePtr->getRenderer();

	renderer->makeCurrent();
	renderer->clearBuffer();
	renderer->beginRender();

	//I am a 2d game.
	renderer->begin2DMode();
	m_pFSM->render(renderer);
	renderer->end2DMode();

	//show fps
	{
		char fpsBuf[256];
		sprintf(fpsBuf,"FPS:%.2f", m_profiler.getFPS());

		renderer->begin2DMode();
		m_debugFont->DrawTextLine(renderer,fpsBuf,0,0);
		renderer->end2DMode();
	}

	renderer->endRender();
	renderer->swapBuffer();
}

void GameApp::postUpdate(f32 dt)
{
	m_pFSM->postUpdate(dt);
}

//---------- UI --------------------
void GameApp::loadUIs()
{
	m_uiMainMenu = ui::UIManager::getInstance().loadUILayerFromXML("mainmenu.xml");
	
	m_uiInterface = ui::UIManager::getInstance().loadUILayerFromXML("interface.xml");
	m_uiInterface->setActive(false);
}

void GameApp::unloadUIs()
{
	SAFE_DEL(m_uiMainMenu);
	SAFE_DEL(m_uiInterface);
}

ui::UILayer* GameApp::getUILayer(UILayerEnum layer)
{
	switch(layer)
	{
	case UI_MAIN_MENU:
		return m_uiMainMenu;

	case UI_INTERFACE:
		return m_uiInterface;

	default:
		return 0;
	}
}

//---------- Level ---------------
void GameApp::initLevelLoading()
{
	m_levelLoadingPercent = 0;
}

void GameApp::loadLevel()
{
	if(m_levelLoadingPercent==0)
	{
		SAFE_DEL(m_pCurLevel);

		m_pCurLevel = new Level();
	}
	else if(m_levelLoadingPercent==2)
	{
		//preload level sprites
		XSpriteManager &xspMgr = XSpriteManager::getInstance();
		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();		

		CXSpritePtr sprObjs = xspMgr.getRes("objs.xsp");		
	}
	else if(m_levelLoadingPercent==5)
	{
		m_pCurLevel->loadWorld("level1_1.xml");
		
		m_levelLoadingPercent = 99;
	}

	m_levelLoadingPercent++;		
}

void GameApp::freeLevel()
{
	XSpriteManager &xspMgr = XSpriteManager::getInstance();
	xspMgr.removeAllUnusedRes();

	renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();	
	texMgr.removeAllUnusedRes();
}