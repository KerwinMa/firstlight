#ifndef FLTDEMO_GAME_APP
#define FLTDEMO_GAME_APP

#include "firstlight.h"
#include "gamelib/State.h"
#include "gamelib/StateMachine.h"

#define MY_PROJECT_NAME "fltDemo"
#define MY_WIN_NAME L"FLT Demo"

using namespace flt;
using namespace utils;
using namespace renderer;

enum UILayerEnum
{
	UI_MAIN_MENU,
	UI_INTERFACE,
};

class Level;

class GameApp: public App
{
public:
	GameApp();

	~GameApp();	

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

	// state machine
	StateMachine<GameApp>*  getFSM()const { return m_pFSM; }

	// UIs
	void loadUIs();
	void unloadUIs();
	ui::UILayer* getUILayer(UILayerEnum layer);	

	// level
	void initLevelLoading();
	void loadLevel();
	int getLevelLoadingPercent() const { return m_levelLoadingPercent; }
	void freeLevel();

	Level* getCurrentLevel() { return m_pCurLevel; }
	

public:
	CXFontPtr m_debugFont;

private:

	//an instance of the state machine class
	StateMachine<GameApp>*  m_pFSM;

	ui::UILayer *m_uiMainMenu;
	ui::UILayer *m_uiInterface;

	Level *m_pCurLevel;

	int m_levelLoadingPercent;
};

#endif //FLTDEMO_GAME_APP
