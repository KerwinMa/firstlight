#include "GameStates.h"
#include "GameApp.h"

void GSExit::onEnter(GameApp *pGame)
{

}

bool GSExit::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	return false;
}

void GSExit::update(GameApp* pGame, float dt)
{
	pGame->setExitApp();
}

void GSExit::postUpdate(GameApp* pGame, float dt)
{
}

void GSExit::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{

}

void GSExit::onExit(GameApp* pGame)
{

}