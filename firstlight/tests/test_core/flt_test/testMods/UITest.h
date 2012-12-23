#ifndef UI_TEST_H
#define UI_TEST_H

#include "../ITestMod.h"
#include "./auto_generated/animDefs.h"
#include "./auto_generated/ui_defs.h"

class UITest: public ITestMod, public IEventReceiver
{
public:
	UITest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}

	virtual bool init()
	{
		XSpriteManager &xspMgr = XSpriteManager::getInstance();
		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();
		XFontManager &fontMgr = XFontManager::getInstance();

		CXSpritePtr testXSprite = xspMgr.getRes("ft.xsp");		
		testXSprite->SetTexture(texMgr.getRes("ft.tga"));	


		m_testFont = fontMgr.getRes("font.xsp");	
		m_testFont->setName("TestFont");
		m_testFont->SetTexture(texMgr.getRes("font.tga"));

		m_testLayer = ui::UIManager::getInstance().loadUILayerFromXML("testLayer.xml");	

#ifdef WIN32
		m_testLayer->saveToXMLFile("res/testLayer.xml");
#endif

		EventManager::getInstance().registerReceiver(events_id::EVT_UI_BUTTON_TAPPED, this);
		EventManager::getInstance().registerReceiver(events_id::EVT_UI_BUTTON_SWITCHED, this);

		return true;
	}

	virtual void destroy()
	{
		EventManager::getInstance().unRegisterReceiver(events_id::EVT_UI_BUTTON_SWITCHED, this);
		EventManager::getInstance().unRegisterReceiver(events_id::EVT_UI_BUTTON_TAPPED, this);

		delete m_testLayer;		
	}

	virtual bool onEvent(const IEvent& event)
	{
		if(!m_enable)
			return false;

		if(event.getUID()==events_id::EVT_UI_BUTTON_TAPPED)
		{
			EvtUIButtonTapped* evt = (EvtUIButtonTapped*)&event;
			DBG("test", "button tapped, layerid=%d, objid=%d", evt->LayerID, evt->ButtonID);

			if(evt->LayerID==m_testLayer->getLayerID())
			{
				if(evt->ButtonID == UIID_TEST_LAYER_PLAY_BUTTON)
				{
					App::TheApp->setExitApp();
					return true;
				}
			}
		}
		else if(event.getUID()==events_id::EVT_UI_BUTTON_SWITCHED)
		{
			EvtUIButtonSwitched* evt = (EvtUIButtonSwitched*)&event;

			if(evt->LayerID==m_testLayer->getLayerID())
			{
				if(evt->ButtonID == UIID_TEST_LAYER_SWITCH_BUTTON)
				{
					m_testLayer->playAnimation(!evt->ButtonIsOn?"enter":"exit");					

					return true;
				}
			}
		}
		return false;
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;

		m_testLayer->update(dt);
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		renderer->begin2DMode();		
		
		m_testLayer->draw(renderer);


		renderer->end2DMode();
	}

private:
	CXFontPtr m_testFont;

	ui::UILayer* m_testLayer;
};

#endif //UI_TEST_H

