#ifndef FLT_PHY2D_WORLD_H
#define FLT_PHY2D_WORLD_H

#include "common.h"
#include "externs/Box2D/Box2D.h"
#include "sharePtr.h"
#include "core/rect.h"
#include "externs/tinyxml/tinyxml.h"

namespace flt
{

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)
}

namespace phy2d
{
	class Phy2dActor;

	//! Phy2dWorld is a wrapper of 2d physic world, using box2d
	//! About coordinates:
	//!   screen coordinates -> world coordinates -> box2d coordinates
	//!   world coordinates use pixel unit, but the axis is x right & y up.
	//!   User should use world coordinates to place actors and camera, 
	//!   and the phy2d convert them to screen coordinates to draw, and to box2d coordinates to move.


	class Phy2dWorld
	{
		friend class Phy2dActor;

	public:
		Phy2dWorld();
		virtual ~Phy2dWorld();	

		void createPhyWorld();
		void releaseActors();
		void reloadWorld();

		void setRatio(f32 ratio){ m_ratio = ratio; }

		f32 getRatio() const { return m_ratio; }

		void setGravity(f32 x, f32 y);

		void setCameraRectOnScreen(const core::rectf &camRect);		

		void setCameraWorldPos(f32 x, f32 y, bool smooth);

		void setCameraFocusOn(Phy2dActor* actor, bool smooth);

		void getCameraWorldPos(f32& x, f32 &y);

		void setCameraZoomLimit(f32 minZoom, f32 maxZoom);

		void setCameraZoom(f32 zoom);

		f32 getCameraZoom() const { return m_cameraZoom; }
		
		Phy2dActor* createActor();		

		void addActor(Phy2dActor* actor)
		{
			m_actors.push_back(actor);
		}

		bool removeActor(Phy2dActor* actor);
				
		int getActorNum() const
		{
			return (int)m_actors.size();
		}

		Phy2dActor* getActor(int index) const
		{
			return m_actors[index];
		}

		bool loadFromXMLFile(const char* xmlFile);
		bool loadFromXML(TiXmlElement *xmlNodePhy2DWorld);
		void saveXMLFile();
		void saveToXMLFile(const char* otherXMLFile, bool setAsDocFile=false);		
		void saveToXML(TiXmlElement *xmlNodePhy2DWorld);

		void sortActors(array_t<Phy2dActor*> &actors);
		void resetToInitial();

		virtual void update(f32 dt, bool phyUpdate=true);
		virtual void updateCamera(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);

		Phy2dActor* getDynamicActorAtPoint(f32 screenX, f32 screenY);

		//remove from phy world, hide , but not remove from world's actor list
		void removeActorFromPhy(Phy2dActor* actor);

		b2Fixture* getStaticFixtureAtPoint(f32 screenX, f32 screenY);
		void removeStaticFixtureFromPhy(b2Fixture* fixture);


		inline void worldToScreenPos(f32 worldX, f32 worldY, f32& screenX, f32& screenY);
		inline void screenToWorldPos(f32 screenX, f32 screenY, f32& worldX, f32& worldY);

		inline void worldToPhysicsPos(f32 worldX, f32 worldY, b2Vec2& phyPos);
		inline void physicsToWorldPos(const b2Vec2& phyPos, f32& worldX, f32& worldY);

		inline void physicsToScreenPos(const b2Vec2& phyPos, f32& screenX, f32& screenY);
		inline void screenToPhysicsPos(f32 screenX, f32 screenY, b2Vec2& phyPos);		

		//////////////for editor//////////////
		void clearEditorSelection();
		//mark the actor as selected, and push it to the selection array
		void selectedActor(Phy2dActor* actor);
		//mark the actor as unselected, and erase it from the selection array
		void deselectedActor(Phy2dActor* actor);
		//find top most actor at pos
		Phy2dActor* findTopMostEditorActor(f32 screenX, f32 screenY);
		//find and set multi actors selected in a rect
		void selectEditorActors(const core::rectf& rect);
		int getEditorSelectedActorsNum();
		array_t<Phy2dActor*>& getEditorSelectedActors();
		void deleteSelectedActors();
		bool isActorSelected(Phy2dActor* actor);

		void updateEditor(f32 dt);

	private:
		//! camera pos & size in world space
		f32 m_cameraX;
		f32 m_cameraY;
		f32 m_cameraOldX, m_cameraOldY, m_cameraDestX, m_cameraDestY;
		f32 m_cameraHalfWidth;
		f32 m_cameraHalfHeight;		

		//! Define camera rect on screen in screen coordinates, 
		//! by default camera occupies the full screen,
		//! you can specify a portion of screen as camera, but can't larger than screen
		//! camera rect will be clipped to screen size when set.
		core::rectf m_cameraRectOnScreen;

		//! use this to scale whole world size
		f32 m_cameraZoom;
		f32 m_cameraZoomMin, m_cameraZoomMax;

		//! ratio map phy position to world position
		//! world position = phy position * m_ratio
		f32 m_ratio;

		//! Box2D world
		b2World* m_world;

		b2Body* m_staticBody; //use only one static actor body to attach all static fixtures

		array_t<Phy2dActor*> m_actors;

		array_t<Phy2dActor*> m_renderList;

		stringc m_xmlFileName;

		// for editor
		array_t<Phy2dActor*> m_editorSelectedActors;
	};

	inline void Phy2dWorld::worldToScreenPos(f32 worldX, f32 worldY, f32& screenX, f32& screenY)
	{
		//world to camera position
		f32 x_onCam = (worldX- m_cameraX)*m_cameraZoom;
		f32 y_onCam = (worldY - m_cameraY)*m_cameraZoom;
		//camera to screen position
		screenX = m_cameraRectOnScreen.UpperLeftCorner.X + m_cameraHalfWidth + x_onCam;
		screenY = m_cameraRectOnScreen.UpperLeftCorner.Y + m_cameraHalfHeight - y_onCam;		
	}

	inline void Phy2dWorld::screenToWorldPos(f32 screenX, f32 screenY, f32& worldX, f32& worldY)
	{		
		//screen to camera position
		f32 x_onCam = screenX - ( m_cameraRectOnScreen.UpperLeftCorner.X + m_cameraHalfWidth );
		f32 y_onCam = ( m_cameraRectOnScreen.UpperLeftCorner.Y + m_cameraHalfHeight) - screenY;
		//camera to world position
		worldX = x_onCam/m_cameraZoom + m_cameraX;
		worldY = y_onCam/m_cameraZoom + m_cameraY;
	}

	inline void Phy2dWorld::worldToPhysicsPos(f32 worldX, f32 worldY, b2Vec2& phyPos)
	{
		phyPos.x = worldX/m_ratio;
		phyPos.y = worldY/m_ratio;
	}

	inline void Phy2dWorld::physicsToWorldPos(const b2Vec2& phyPos, f32& worldX, f32& worldY)
	{
		worldX = phyPos.x*m_ratio;
		worldY = phyPos.y*m_ratio;
	}

	inline void Phy2dWorld::physicsToScreenPos(const b2Vec2& phyPos, f32& screenX, f32& screenY)
	{
		f32 worldX, worldY;
		physicsToWorldPos(phyPos, worldX, worldY);
		worldToScreenPos(worldX, worldY, screenX, screenY);		
	}

	inline void Phy2dWorld::screenToPhysicsPos(f32 screenX, f32 screenY, b2Vec2& phyPos)
	{
		f32 worldX, worldY;
		screenToWorldPos(screenX, screenY, worldX, worldY);
		worldToPhysicsPos(worldX, worldY, phyPos);
	}

}//phy2d
}//flt

#endif //FLT_PHY2D_WORLD_H
