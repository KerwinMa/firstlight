#include "GameStates.h"
#include "GameApp.h"

void GSLevelPause::onEnter(GameApp *pGame)
{

}

bool GSLevelPause::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	return false;
}

void GSLevelPause::update(GameApp* pGame, float dt)
{
	
}

void GSLevelPause::postUpdate(GameApp* pGame, float dt)
{
}

void GSLevelPause::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{

}

void GSLevelPause::onExit(GameApp* pGame)
{

}
