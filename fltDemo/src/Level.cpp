#include "Level.h"
#include "Ball.h"

UITouchZone* g_fullScreenZone = 0;

Level::Level()
	:m_world(0),m_playerBall(0)
{

}

Level::~Level()
{
	releaseLevel();
}

bool Level::loadWorld(const stringc& levelXMLFile)
{
	releaseLevel();

	m_world = new phy2d::Phy2dWorld();
		
	bool success = m_world->loadFromXMLFile(levelXMLFile.c_str());
	if(success)
	{
		m_playerBall = new Ball(m_world->getActor(0));		
	}

	//m_world->setGravity(0.0f,0.0f);

	return success;
}

void Level::releaseLevel()
{
	SAFE_DEL(m_world);
	SAFE_DEL(m_playerBall);
}

void Level::update(f32 dt)
{	
	m_playerBall->update(dt);
	m_world->setCameraFocusOn(m_playerBall->getActor(),true);
	m_world->update(dt);
}

void Level::render(const renderer::IRendererPtr& renderer)
{
	m_world->draw(renderer);
}

void Level::postUpdate(f32 dt)
{

}

void Level::resetLevel()
{
	loadWorld("level1_1.xml");
	
	//m_world->resetToInitial();	
	
}

bool Level::processEvent(const flt::IEvent& event)
{
	//if(event.getUID()==events_id::EVT_UI_SLIDE)
	//{
	//	EvtUISlide* evt = (EvtUISlide*)&event;

	//	m_playerBall->onSlide(evt->Direction, evt->Speed);				
	//}

	if(event.getUID()==events_id::EVT_TOUCH)
	{
		EvtTouch* evt = (EvtTouch*)&event;
		//if(evt->Phase==Touch_Began)
		{
			flt::phy2d::Phy2dActor* actor = m_world->getDynamicActorAtPoint(evt->X,evt->Y);			
			if(actor!=this->m_playerBall->getActor())
				m_world->removeActorFromPhy(actor);

			b2Fixture* fixture = m_world->getStaticFixtureAtPoint(evt->X,evt->Y);						
			m_world->removeStaticFixtureFromPhy(fixture);
		}
	}

	return false;
}
