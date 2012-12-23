#include "GameStates.h"
#include "GameApp.h"

void GSLevelLoading::onEnter(GameApp *pGame)
{
	pGame->initLevelLoading();
}

bool GSLevelLoading::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	return false;
}

void GSLevelLoading::update(GameApp* pGame, float dt)
{
	pGame->loadLevel();	
}

void GSLevelLoading::postUpdate(GameApp* pGame, float dt)
{
	if(pGame->getLevelLoadingPercent()>=100)
		pGame->getFSM()->changeState(GSLevel::getInstance());
}

void GSLevelLoading::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{
	//char tmp[256];
	//sprintf(tmp,"loading :%d%%", pGame->getLevelLoadingPercent());

	//pGame->m_debugFont->DrawTextLine(renderer,tmp,30,30);
}

void GSLevelLoading::onExit(GameApp* pGame)
{

}
