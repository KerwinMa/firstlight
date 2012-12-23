#include "GameStates.h"
#include "GameApp.h"
#include "Level.h"
#include "auto_generated/ui_defs.h"

GSLevel::GSLevel()
	:m_uiInterface(0),m_level(0),m_world(0)
{
}

void GSLevel::onEnter(GameApp *pGame)
{
	m_uiInterface = pGame->getUILayer(UI_INTERFACE);	
	m_uiInterface->setActive(true);
	m_level = pGame->getCurrentLevel();
	m_world = m_level->getWorld();

	UITouchZone* zone = (UITouchZone*)m_uiInterface->getUIObjectByID(UIID_INTERFACE_FULLSCREENZONE);
	g_fullScreenZone = zone;
}

bool GSLevel::processEvent(GameApp* pGame, const flt::IEvent& event)
{
	if(event.getUID()==events_id::EVT_UI_BUTTON_TAPPED)
	{
		EvtUIButtonTapped* evt = (EvtUIButtonTapped*)&event;		

		if(evt->LayerID == m_uiInterface->getLayerID())
		{
			if(evt->ButtonID == UIID_INTERFACE_RETRY)
			{
				m_level->resetLevel();
				m_world = m_level->getWorld();
				return true;
			}
			else if(evt->ButtonID == UIID_INTERFACE_PAUSE)
			{
				App::TheApp->setExitApp();
				return true;
			}
		}
	}
	else
	{
		m_level->processEvent(event);
	}
	

#ifdef FLT_DEVICE_WIN32
	if(event.getUID()==events_id::EVT_MOUSE_WHEEL)
	{
		EvtMouseWheel& wheel = (EvtMouseWheel&)event;
		//DBG("test","wheel=%f",wheel.Delta);
		static f32 zoom = 1.0f;
		zoom += wheel.Delta*0.08f*zoom;										
		
		m_world->setCameraZoom(zoom);
		return true;
	}
#endif

	return false;
}

void GSLevel::update(GameApp* pGame, float dt)
{
	m_level->update(dt);
	
	m_uiInterface->update(dt);
}

void GSLevel::postUpdate(GameApp* pGame, float dt)
{
	m_level->postUpdate(dt);
}

void GSLevel::render(GameApp *pGame, const flt::renderer::IRendererPtr& renderer)
{
	//draw level
	m_level->render(renderer);

	//draw ui	
	m_uiInterface->draw(renderer);	
}

void GSLevel::onExit(GameApp* pGame)
{
	m_uiInterface->setActive(false);
	m_uiInterface = 0;
	m_level = 0;
	m_world = 0;
}
