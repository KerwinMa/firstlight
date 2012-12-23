#ifndef BOX2D_TEST_H
#define BOX2D_TEST_H

#include "../ITestMod.h"
#include "./auto_generated/animDefs.h"

class Box2DTest: public ITestMod, public IEventReceiver
{
public:
	Box2DTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}	

	virtual void createWorld()
	{
		m_phyWorld = new phy2d::Phy2dWorld();		

#ifdef CREATE_ACTORS_BY_HAND
		phy2d::Phy2dActor* actor = 0;		

		//build ground
		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = -80;
			basicAttr.worldY = 0;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 0;
			basicAttr.worldY = 0;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 80;
			basicAttr.worldY = 0;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 160;
			basicAttr.worldY = 0;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 240;
			basicAttr.worldY = 0;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}
		
		//build some buildings
		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = -70;
			basicAttr.worldY = 50;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 10;
			basicAttr.worldY = 100;
			basicAttr.sizeScaleX = 3.0f;
			//basicAttr.isVisible = false;
			basicAttr.angle = 30.0f;
			gfxAttr.sprName = "ft.xsp";
			gfxAttr.animID = FT_ANIM_BT_P;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}

		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 60;
			basicAttr.worldY = 150;			
			basicAttr.angle = 30.0f;
			gfxAttr.sprName = "birds.xsp";
			gfxAttr.animID = BIRDS_ANIM_OK;
			phyAttr.isBall = true;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}		

		//some dynamic objects
		{
			phy2d::ActorBasicAttr basicAttr;
			phy2d::ActorPhyAttr phyAttr;
			phy2d::ActorGfxAttr gfxAttr;
			
			actor = m_phyWorld->createActor();
			basicAttr.worldX = 150;
			basicAttr.worldY = 300;	
			basicAttr.sizeScaleX = 1.5f;
			basicAttr.angle = 15.0f;
			gfxAttr.sprName = "birds.xsp";
			gfxAttr.animID = BIRDS_ANIM_OK;
			phyAttr.isStatic = false;

			actor->init(basicAttr, phyAttr, gfxAttr);
		}	

		//set camera
		m_phyWorld->setCameraFocusOn(actor,false);

#else

		m_phyWorld->loadFromXMLFile("testWorld.xml");
	
	#ifdef WIN32
		//m_phyWorld->saveToXMLFile("res/testWorld.xml");
	#endif

#endif //CREATE_ACTORS_BY_HAND
				

				
	}

	virtual bool init()
	{
		XSpriteManager &xspMgr = XSpriteManager::getInstance();
		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();
		XFontManager &fontMgr = XFontManager::getInstance();

		CXSpritePtr testXSprite = xspMgr.getRes("ft.xsp");		
		testXSprite->SetTexture(texMgr.getRes("ft.tga"));
		testXSprite = xspMgr.getRes("birds.xsp");		
		testXSprite->SetTexture(texMgr.getRes("birds.png"));//test png


		m_testFont = fontMgr.getRes("font.xsp");	
		m_testFont->setName("TestFont");
		m_testFont->SetTexture(texMgr.getRes("font.tga"));

		//-------------------

		createWorld();

		EventManager::getInstance().registerReceiver(events_id::EVT_TOUCH, this);
		EventManager::getInstance().registerReceiver(events_id::EVT_MOUSE_WHEEL, this);

		//play background music
		//sound::setBackgroundMusicVolume(0.5f);
		//sound::playBackgroundMusic("res/sound/background.mp3", true);

		return true;
	}

	virtual void destroy()
	{	
		delete m_phyWorld;
		EventManager::getInstance().unRegisterReceiver(events_id::EVT_TOUCH, this);
	}

	virtual bool onEvent(const IEvent& event)
	{
		if(!m_enable)
			return false;

		if(event.getUID()==events_id::EVT_TOUCH)
		{
			EvtTouch& touchEvt = (EvtTouch&)event;

			if(touchEvt.Phase==Touch_Began)
			{				
				phy2d::Phy2dActor* actor = 0;
				actor = m_phyWorld->createActor();

				
				phy2d::ActorBasicAttr basicAttr;
				phy2d::ActorPhyAttr phyAttr;
				phy2d::ActorGfxAttr gfxAttr;												
				
				basicAttr.angle = 60.0f;
				gfxAttr.sprName = "birds.xsp";
				gfxAttr.animID = BIRDS_ANIM_OK;
				phyAttr.isStatic = false;
				phyAttr.isBall = true;															

				m_phyWorld->screenToWorldPos(touchEvt.X, touchEvt.Y, basicAttr.worldX, basicAttr.worldY);
				
				actor->init(basicAttr, phyAttr, gfxAttr);

				m_phyWorld->setCameraFocusOn(actor,true);

				//sound::playEffect("res/sound/effect1.wav");
			}
			else if(touchEvt.Phase==Touch_Ended && touchEvt.Y<25 && touchEvt.X<25)
			{
				delete m_phyWorld;
				createWorld();
			}
			return true;
		}
		else if(event.getUID()==events_id::EVT_MOUSE_WHEEL)
		{
			EvtMouseWheel& wheel = (EvtMouseWheel&)event;
			//DBG("test","wheel=%f",wheel.Delta);
			static f32 zoom = 1.0f;
			zoom += wheel.Delta*0.08f*zoom;										
			
			m_phyWorld->setCameraZoom(zoom);
			return true;
		}
		
		return false;
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;

		m_phyWorld->update(dt);		
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		renderer->begin2DMode();		
		
		
		//m_testFont->DrawTextLine(renderer,"Box2D", 10, 10);

		m_phyWorld->draw(renderer);

		renderer->end2DMode();
	}

private:
	CXFontPtr m_testFont;

	phy2d::Phy2dWorld* m_phyWorld;
};

#endif //BOX2D_TEST_H

