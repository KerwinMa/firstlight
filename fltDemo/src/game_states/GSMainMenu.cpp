#include "GameStates.h"
#include "GameApp.h"
#include "auto_generated/ui_defs.h"

GSMainMenu::GSMainMenu()
	:m_uiMainMenu(0)
{
}

void GSMainMenu::onEnter(GameApp *pGame)
{
	m_uiMainMenu = pGame->getUILayer(UI_MAIN_MENU);
	m_uiMainMenu->playAnimation("enter");
	if(!m_uiMainMenu->isActive())
		m_uiMainMenu->setActive(true);
}

bool GSMainMenu::processEvent(GameApp* pGame, const flt::IEvent& event)
{	
	if(event.getUID()==events_id::EVT_UI_BUTTON_TAPPED)
	{
		EvtUIButtonTapped* evt = (EvtUIButtonTapped*)&event;

		if(evt->LayerID==m_uiMainMenu->getLayerID())
		{
			if(evt->ButtonID == UIID_MAIN_MENU_PLAY_BUTTON)
			{
				m_uiMainMenu->playAnimation("exit");				
				return true;
			}
		}
	}

	return false;
}

void GSMainMenu::update(GameApp* pGame, float dt)
{	
	m_uiMainMenu->update(dt);

	if(m_uiMainMenu->isCurrentAnimationOver() && m_uiMainMenu->getCurrentAnimationName()=="exit")
	{
		pGame->getFSM()->changeState(GSLevelLoading::getInstance());//temp
	}
}

void GSMainMenu::postUpdate(GameApp* pGame, float dt)
{
}

void GSMainMenu::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{	
	m_uiMainMenu->draw(renderer);
}

void GSMainMenu::onExit(GameApp* pGame)
{
	m_uiMainMenu->setActive(false);
	m_uiMainMenu = 0;
}
