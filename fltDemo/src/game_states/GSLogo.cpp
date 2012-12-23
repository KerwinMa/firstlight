#include "GameStates.h"
#include "GameApp.h"

using namespace flt;

void GSLogo::onEnter(GameApp *pGame)
{
	m_timer = 0.0f;

	XSpriteManager &xspMgr = XSpriteManager::getInstance();
	renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();

	CXSpritePtr logoSpr = xspMgr.getRes("logo.xsp");		
	logoSpr->SetTexture(texMgr.getRes("logo.tga"));
}

bool GSLogo::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	return false;
}

void GSLogo::update(GameApp* pGame, float dt)
{	
	m_timer += dt;

	//DBG("fltDemo","logo timer=%f",m_timer);

	if(m_timer>2000)
		pGame->getFSM()->changeState(GSMainMenu::getInstance());
}

void GSLogo::postUpdate(GameApp* pGame, float dt)
{
}

void GSLogo::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{			
	XSpriteManager &xspMgr = XSpriteManager::getInstance();
	CXSpritePtr logoSpr = xspMgr.getRes("logo.xsp");	
	logoSpr->DrawAframe(renderer,0,0,pGame->getDevice()->getDeviceWidth()/2, pGame->getDevice()->getDeviceHeight()/2);
}

void GSLogo::onExit(GameApp* pGame)
{
	XSpriteManager &xspMgr = XSpriteManager::getInstance();
	xspMgr.removeRes("logo.xsp");
}