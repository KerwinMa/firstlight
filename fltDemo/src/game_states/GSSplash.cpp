#include "GameStates.h"
#include "GameApp.h"

void GSSplash::onEnter(GameApp *pGame)
{
	m_timer = 0.0f;
}

bool GSSplash::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	return false;
}

void GSSplash::update(GameApp* pGame, float dt)
{
	m_timer += dt;	

	if(m_timer>1000)
		pGame->getFSM()->changeState(GSMainMenu::getInstance());
}

void GSSplash::postUpdate(GameApp* pGame, float dt)
{
}

void GSSplash::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{
	pGame->m_debugFont->DrawTextLine(renderer,"splash",50,50);
}

void GSSplash::onExit(GameApp* pGame)
{

}
