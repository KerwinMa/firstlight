#include "phy2dWorld.h"
#include "phy2dActor.h"
#include "renderer/IRenderer.h"
#include "utils/Logger.h"
#include "app/App.h"

namespace flt
{
namespace phy2d
{

	class QueryCallback : public b2QueryCallback
	{
	public:
		QueryCallback(const b2Vec2& point)
		{
			m_point = point;
			m_fixture = NULL;
		}

		bool ReportFixture(b2Fixture* fixture)
		{
			b2Body* body = fixture->GetBody();
			if (body->GetType() == b2_dynamicBody)
			{
				bool inside = fixture->TestPoint(m_point);
				if (inside)
				{
					m_fixture = fixture;

					// We are done, terminate the query.
					return false;
				}
			}

			// Continue the query.
			return true;
		}

		b2Vec2 m_point;
		b2Fixture* m_fixture;
	};

	class StaticShapeQueryCallback : public b2QueryCallback
	{
	public:
		StaticShapeQueryCallback(const b2Vec2& point)
		{
			m_point = point;
			m_fixture = NULL;
		}

		bool ReportFixture(b2Fixture* fixture)
		{
			b2Body* body = fixture->GetBody();
			if (body->GetType() == b2_staticBody)
			{
				bool inside = fixture->TestPoint(m_point);
				if (inside)
				{
					m_fixture = fixture;

					// We are done, terminate the query.
					return false;
				}
			}

			// Continue the query.
			return true;
		}

		b2Vec2 m_point;
		b2Fixture* m_fixture;
	};

	Phy2dWorld::Phy2dWorld()
		:m_ratio(20.0f),
		 m_cameraX(0.0f),m_cameraY(0.0f),
		 m_cameraOldX(0.0f),m_cameraOldY(0.0f),
		 m_cameraDestX(0.0f),m_cameraDestY(0.0f),
		 m_cameraZoom(1.0f),
		 m_cameraZoomMin(0.1f),m_cameraZoomMax(10.0f)
	{
		if(App::TheApp.isValid())
		{
			m_cameraRectOnScreen.set(0.0f, 0.0f, (f32)App::TheApp->getWindowWidth(), (f32)App::TheApp->getWindowHeight() ); 						
			setCameraRectOnScreen(m_cameraRectOnScreen);
		}
		else
		{
			setCameraRectOnScreen(core::rectf(0,0,300,300));
		}

		createPhyWorld();			
	}		
	
	Phy2dWorld::~Phy2dWorld()
	{
		releaseActors();
		
		delete m_world;
	}

	void Phy2dWorld::setGravity(f32 x, f32 y)
	{
		if(m_world)
		{
			m_world->SetGravity(b2Vec2(x,y));
		}
	}

	void Phy2dWorld::createPhyWorld()
	{
		b2Vec2 gravity(0.0f, -9.8f);
		bool doSleep = true;

		//world
		m_world = new b2World(gravity, doSleep);
		
		b2BodyDef bodyDef;
		bodyDef.position.Set(0,0);	
		bodyDef.type = b2_staticBody;
		m_staticBody = m_world->CreateBody(&bodyDef);	
	}

	void Phy2dWorld::releaseActors()
	{
		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			delete m_actors[i];
		}
		m_actors.clear();
	}
	
	void Phy2dWorld::reloadWorld()
	{
		SAFE_DEL(m_world);

		releaseActors();

		createPhyWorld();

		loadFromXMLFile(m_xmlFileName.c_str());
	}

	bool Phy2dWorld::loadFromXMLFile(const char* xmlFile)
	{
		TiXmlDocument doc(xmlFile);
		if (!doc.LoadFile()) 
			return false;

		TiXmlHandle hDoc(&doc);
		
		TiXmlElement* pDocRoot = hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pDocRoot) 
			return false;	

		stringc docRootName=pDocRoot->Value();
		if(docRootName != "FLTResXML")
			return false;

		TiXmlHandle hDocRoot(pDocRoot);
		TiXmlElement* pXMLNodePhy2DWorld = hDocRoot.FirstChild("Phy2DWorld").Element();

		if(pXMLNodePhy2DWorld==0)
			return false;

		bool bLoaded = loadFromXML(pXMLNodePhy2DWorld);

		if(bLoaded)
		{
			m_xmlFileName = xmlFile;			
		}

		return bLoaded;	
	}	

	bool Phy2dWorld::loadFromXML(TiXmlElement *xmlNodePhy2DWorld)
	{
		releaseActors();

		TiXmlHandle hXmlNode=TiXmlHandle(xmlNodePhy2DWorld);
				
		// load actors
		TiXmlElement* pActorNode = hXmlNode.FirstChild( "Actors" ).FirstChild("Actor").Element();
		for( ; pActorNode; pActorNode=pActorNode->NextSiblingElement())
		{
			Phy2dActor *actor = createActor();
			actor->loadFromXML(pActorNode);
			actor->init();
		}		
				
		return true;
	}

	bool lessActorLayer(const Phy2dActor* const actor1, const Phy2dActor* const actor2)
	{
		return actor1->getLayer() < actor2->getLayer();
	}

	void Phy2dWorld::sortActors(array_t<Phy2dActor*> &actors)
	{
		// sort actors by layer
		std::sort(actors.begin(), actors.end(), lessActorLayer);
	}
	
	void Phy2dWorld::saveXMLFile()
	{
		if(m_xmlFileName.size()>0)
			saveToXMLFile(m_xmlFileName.c_str());
	}

	void Phy2dWorld::saveToXMLFile(const char* otherXMLFile, bool setAsDocFile)
	{
		TiXmlDocument doc;  		

		TiXmlElement * pDocRoot = new TiXmlElement( "FLTResXML" );  
		doc.LinkEndChild( pDocRoot ); 

		TiXmlElement * pXMLNodePhy2DWorld = new TiXmlElement( "Phy2DWorld" );  
		pDocRoot->LinkEndChild( pXMLNodePhy2DWorld ); 

		saveToXML(pXMLNodePhy2DWorld);		

		doc.SaveFile(otherXMLFile);

		if(setAsDocFile)
		{
			m_xmlFileName = otherXMLFile;
		}
	}

	void Phy2dWorld::saveToXML(TiXmlElement *xmlNodePhy2DWorld)
	{		
		TiXmlElement *actorsNode = new TiXmlElement("Actors");
		xmlNodePhy2DWorld->LinkEndChild(actorsNode);

		int actorNum =  (int)m_actors.size();

		for(int i=0; i<actorNum; i++)
		{
			TiXmlElement * node = new TiXmlElement("Actor");
			m_actors[i]->saveToXML(node);
			actorsNode->LinkEndChild(node);
		}	
	}	

	void Phy2dWorld::resetToInitial()
	{
		SAFE_DEL(m_world);		

		createPhyWorld();

		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			m_actors[i]->resetToInitial();		
		}	
	}

	void Phy2dWorld::setCameraRectOnScreen(const core::rectf &camRect)
	{
		m_cameraRectOnScreen = camRect;

		//clip inside screen
		if(App::TheApp.isValid())
		{			
			m_cameraRectOnScreen.clipAgainst(core::rectf(0.0f,0.0f,(f32)App::TheApp->getWindowWidth(),(f32)App::TheApp->getWindowHeight()));
		}

		m_cameraHalfWidth = m_cameraRectOnScreen.getWidth()/2.0f;
		m_cameraHalfHeight = m_cameraRectOnScreen.getHeight()/2.0f;
	}

	void Phy2dWorld::setCameraWorldPos(f32 x, f32 y, bool smooth)
	{		
		m_cameraDestX = x;
		m_cameraDestY = y;
		if(smooth)
		{
			m_cameraOldX = m_cameraX;
			m_cameraOldY = m_cameraY;
		}
		else
		{
			m_cameraOldX = x;
			m_cameraOldY = y;
		}
	}

	void Phy2dWorld::setCameraFocusOn(Phy2dActor* actor, bool smooth)
	{
		setCameraWorldPos(actor->getWorldX(), actor->getWorldY(), smooth);		
	}

	void Phy2dWorld::getCameraWorldPos(f32& x, f32 &y)
	{
		x = m_cameraX;
		y = m_cameraY;
	}

	void Phy2dWorld::setCameraZoomLimit(f32 minZoom, f32 maxZoom)
	{
		FLT_ASSERT(minZoom<=maxZoom && minZoom>0);
		
		m_cameraZoomMin = minZoom;	
		m_cameraZoomMax = maxZoom;
	}

	void Phy2dWorld::setCameraZoom(f32 zoom) 
	{ 		
		m_cameraZoom = core::clamp(zoom, m_cameraZoomMin, m_cameraZoomMax);
	}

	Phy2dActor* Phy2dWorld::createActor()
	{
		Phy2dActor *actor = new Phy2dActor(this);
		addActor(actor);
		return actor;
	}

	bool Phy2dWorld::removeActor(Phy2dActor* actor)
	{
		array_t<Phy2dActor*>::iterator iter = find(m_actors.begin(), m_actors.end(), actor);
		if(iter!=m_actors.end())
		{
			Phy2dActor* actor = *iter;			
			m_actors.erase(iter);
			return true;
		}
		else
		{
			return false;
		}
	}

	Phy2dActor* Phy2dWorld::getDynamicActorAtPoint(f32 screenX, f32 screenY)
	{
		b2Vec2 b2Pos;

		screenToPhysicsPos(screenX, screenY, b2Pos);

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = b2Pos - d;
		aabb.upperBound = b2Pos + d;

		// Query the world for overlapping shapes.
		QueryCallback callback(b2Pos);
		m_world->QueryAABB(&callback, aabb);

		b2Body* body = 0;

		if (callback.m_fixture)
		{
			body = callback.m_fixture->GetBody();			
		}

		if(body!=0)
		{
			return (Phy2dActor*)body->GetUserData();
		}
		else
		{
			return 0;
		}
	}	

	void Phy2dWorld::removeActorFromPhy(Phy2dActor* actor)
	{
		if(m_world==0)
			return;

		array_t<Phy2dActor*>::iterator iter = find(m_actors.begin(), m_actors.end(), actor);
		if(iter!=m_actors.end())
		{
			Phy2dActor* actor = *iter;
			if(!actor->getPhyAttr().isStatic && actor->getBody()!=0)			
			{				
				actor->destroyBody(m_world);
				actor->getPhyAttr().isEnable = false;

				actor->getBasicAttr().isVisible = false;
			}			
		}
	}

	b2Fixture* Phy2dWorld::getStaticFixtureAtPoint(f32 screenX, f32 screenY)
	{
		b2Vec2 b2Pos;

		screenToPhysicsPos(screenX, screenY, b2Pos);

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = b2Pos - d;
		aabb.upperBound = b2Pos + d;

		// Query the world for overlapping shapes.
		StaticShapeQueryCallback callback(b2Pos);
		m_world->QueryAABB(&callback, aabb);		

		return callback.m_fixture;		
	}

	void Phy2dWorld::removeStaticFixtureFromPhy(b2Fixture* fixture)
	{
		if(fixture==0 || m_world==0 || m_staticBody==0)
			return;

		Phy2dActor* actor = (Phy2dActor*)fixture->GetUserData();		
		
		array_t<Phy2dActor*>::iterator iter = find(m_actors.begin(), m_actors.end(), actor);
		if(iter!=m_actors.end())
		{
			Phy2dActor* actor = *iter;
			if(actor->getPhyAttr().isStatic)			
			{								
				actor->getPhyAttr().isEnable = false;
				actor->getBasicAttr().isVisible = false;
			}			
		}

		m_staticBody->DestroyFixture(fixture);
	}

	///sub calss can override this method to do it's own camera logic, such as map edge blocking
	void Phy2dWorld::updateCamera(f32 dt)
	{
		f32 smooth = 0.95f;
		m_cameraX = m_cameraOldX*smooth + m_cameraDestX*(1-smooth);
		m_cameraOldX = m_cameraX;
		m_cameraY = m_cameraOldY*smooth + m_cameraDestY*(1-smooth);
		m_cameraOldY = m_cameraY;		
	}

	void Phy2dWorld::update(f32 dt, bool phyUpdate)
	{
		if(phyUpdate)
		{
			// Prepare for simulation. Typically we use a time step of 1/60 of a
			// second (60Hz) and 10 iterations. This provides a high quality simulation
			// in most game scenarios.
			f32 timeStep = 1.0f / 60.0f;
			s32 velocityIterations = 6;
			s32 positionIterations = 2;
			
			// Instruct the world to perform a single step of simulation.
			// It is generally best to keep the time step and iterations fixed.
			m_world->Step(timeStep, velocityIterations, positionIterations);

		}
		// Clear applied body forces. We didn't apply any forces, but you
		// should know about this function.
		m_world->ClearForces();	

		//update camera
		updateCamera(dt);

		//update actors
		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			m_actors[i]->update(dt);			
		}		
	}

	void Phy2dWorld::draw(const renderer::IRendererPtr& renderer)
	{	
		m_renderList.clear();

		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{			
			if(m_actors[i]->getBasicAttr().isVisible)
				m_renderList.push_back(m_actors[i]);
		}

		sortActors(m_renderList);


		for(int i=0, is=(int)m_renderList.size(); i<is; i++)
		{			
			m_renderList[i]->draw(renderer);
		}
	}

	//////////////////////////////
	// for editor
	void Phy2dWorld::clearEditorSelection()
	{
		for(int i=0, is=(int)m_editorSelectedActors.size(); i<is; i++)
		{
			m_editorSelectedActors[i]->setSelected(false);
		}
		m_editorSelectedActors.clear();
	}

	void Phy2dWorld::selectedActor(Phy2dActor* actor)
	{
		FLT_ASSERT(actor!=0);
		FLT_ASSERT(!actor->isSelected());

		if(actor!=0)
		{
			actor->setSelected(true);
			m_editorSelectedActors.push_back(actor);
		}
	}

	void Phy2dWorld::deselectedActor(Phy2dActor* actor)
	{
		FLT_ASSERT(actor!=0);
		FLT_ASSERT(actor->isSelected());

		if(actor!=0)
		{
			actor->setSelected(false);
			array_t<Phy2dActor*>::iterator iter = find(m_editorSelectedActors.begin(), m_editorSelectedActors.end(), actor);
			m_editorSelectedActors.erase(iter);
		}
	}

	Phy2dActor* Phy2dWorld::findTopMostEditorActor(f32 screenX, f32 screenY)
	{
		Phy2dActor* topMostSeledActor = 0;		

		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			if(m_actors[i]->isEditorPointIn(screenX,screenY))
			{				
				if(topMostSeledActor==0 || m_actors[i]->getLayer()>=topMostSeledActor->getLayer())
				{
					topMostSeledActor = m_actors[i];					
				}
			}
		}			

		return topMostSeledActor;
	}

	void Phy2dWorld::selectEditorActors(const core::rectf& rect)
	{
		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			if(m_actors[i]->isEditorInRect(rect))
			{				
				if(!m_actors[i]->isSelected())
				{
					selectedActor(m_actors[i]);
				}
			}
		}
	}

	int Phy2dWorld::getEditorSelectedActorsNum()
	{
		return (int)m_editorSelectedActors.size();
	}

	array_t<Phy2dActor*>& Phy2dWorld::getEditorSelectedActors()
	{
		return m_editorSelectedActors;
	}

	void Phy2dWorld::deleteSelectedActors()
	{
		for(int i=0, is=(int)m_editorSelectedActors.size(); i<is; i++)
		{
			if(removeActor(m_editorSelectedActors[i]))
			{
				delete m_editorSelectedActors[i];
			}
		}

		m_editorSelectedActors.clear();
	}

	bool Phy2dWorld::isActorSelected(Phy2dActor* actor)
	{
		if(actor==0)
			return false;

		array_t<Phy2dActor*>::iterator iter = find(m_editorSelectedActors.begin(), m_editorSelectedActors.end(), actor);
		return iter!=m_editorSelectedActors.end();
	}

	void Phy2dWorld::updateEditor(f32 dt)
	{
		//update camera
		updateCamera(dt);

		//update actors
		for(int i=0, is=(int)m_actors.size(); i<is; i++)
		{
			m_actors[i]->updateEditor(dt);			
		}
	}


}//phy2d
}//flt