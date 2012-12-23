#ifndef __GAME_STATES_H__
#define __GAME_STATES_H__

#include "firstlight.h"
#include "gamelib/State.h"

using namespace flt;

class GameApp;
class Level;

#define DECLARE_GAME_STATE_FUNCS(StateClassName) \
	static StateClassName* getInstance() \
	{ \
		static StateClassName instance; \
		return &instance; \
	} \
	virtual void onEnter(GameApp* pGame); \
	virtual bool processEvent(GameApp* pGame, const flt::IEvent& event); \
	virtual void update(GameApp* pGame, float dt); \
	virtual void postUpdate(GameApp* pGame, float dt); \
	virtual void render(GameApp* pGame, const flt::renderer::IRendererPtr& renderer); \
	virtual void onExit(GameApp* pGame);


class GSLogo: public State<GameApp>
{
private:
	GSLogo():m_timer(0.0f){}	

public:	
	DECLARE_GAME_STATE_FUNCS(GSLogo)

private:
	float m_timer;
};

class GSSplash: public State<GameApp>
{
private:
	GSSplash():m_timer(0.0f){}	

public:
	DECLARE_GAME_STATE_FUNCS(GSSplash)

private:
	float m_timer;
};

class GSMainMenu: public State<GameApp>
{
private:
	GSMainMenu();
	
public:
	DECLARE_GAME_STATE_FUNCS(GSMainMenu)

	ui::UILayer* m_uiMainMenu;
};

class GSLevelLoading: public State<GameApp>
{
private:
	GSLevelLoading(){}	

public:
	DECLARE_GAME_STATE_FUNCS(GSLevelLoading)
};

class GSLevel: public State<GameApp>
{
private:
	GSLevel();

public:
	DECLARE_GAME_STATE_FUNCS(GSLevel)

	ui::UILayer* m_uiInterface;
	Level* m_level;
	phy2d::Phy2dWorld* m_world;	
};

class GSLevelPause: public State<GameApp>
{
private:
	GSLevelPause(){}

public:
	DECLARE_GAME_STATE_FUNCS(GSLevelPause)
};

class GSExit: public State<GameApp>
{
private:
	GSExit(){}	

public:
	DECLARE_GAME_STATE_FUNCS(GSExit)
};

#endif //__GAME_STATES_H__