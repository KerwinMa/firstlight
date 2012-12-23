#ifndef FLT_PHY2D_ACTOR_H
#define FLT_PHY2D_ACTOR_H

#include "common.h"
#include "sharePtr.h"
#include "externs/Box2D/Box2D.h"
#include "xlib2d/XAnimObject.h"
#include "externs/tinyxml/tinyxml.h"

namespace flt
{

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)
}

namespace phy2d
{
	class Phy2dWorld;
	
	struct ActorBasicAttr
	{		
		f32 worldX, worldY;	//initial position in world coordinates
		f32 sizeScaleX, sizeScaleY;
		f32 angle;
		bool isVisible;

		ActorBasicAttr()
			:worldX(0.0f),worldY(0.0f),sizeScaleX(1.0f),sizeScaleY(1.0f),angle(0.0f),isVisible(true)
		{
		}
	};	

	struct ActorPhyAttr
	{
		bool isEnable:1;
		bool isStatic:1;
		bool isBall:1;//if it's ball, use sprite box's half width as radius,note: only uniform scale is support (ellipse is not supported now!)
		f32 density;
		f32 friction;
		f32 restitution;
		bool isSensor;
		f32 linearDamping;
		f32 angularDamping;

		ActorPhyAttr()
			:isEnable(true),isStatic(true),isBall(false),density(1.0f),friction(0.3f),restitution(0.0f),isSensor(false),
			 linearDamping(0.0f),angularDamping(0.0f)
		{
		}
	};

	struct ActorGfxAttr
	{
		stringc sprName;
		s32 animID;
		f32 a,r,g,b;
		s32 layer;

		ActorGfxAttr()
			:animID(0),a(1.0f),r(1.0f),g(1.0f),b(1.0f),layer(0)
		{
		}
	};	

	//! Phy2dActor is a wrapper of 2d dynamic or static actor, using box2d

	class Phy2dActor
	{
	public:
		Phy2dActor(Phy2dWorld* phyWorld);
		virtual ~Phy2dActor();
		
		bool loadFromXML(TiXmlElement *xmlActorNode);
		void saveToXML(TiXmlElement *xmlActorNode);

		virtual bool init();
		virtual bool init(const Phy2dActor& rhs);
		virtual bool init(const ActorBasicAttr& basicAttr, const ActorPhyAttr& phyAttr, const ActorGfxAttr& gfxAttr, bool isRefreshInEditor=false);		
		virtual void update(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);
		
		int getLayer() const { return m_gfxAttr.layer; }

		void resetToInitial();

		inline f32 getWorldX() const { return m_worldX; }
		inline f32 getWorldY() const { return m_worldY; }

		b2Body* getBody() { return m_body; }

		void destroyBody(b2World* world);

		void applyForce(const core::vector2df& vec);

		/////////////////for editor//////////////////////
		core::rectf calcCurrentBound();
		void calcEditorPointInActorSpace(f32 &sx, f32 &sy);
		bool isEditorPointIn(f32 sx, f32 sy);
		bool isEditorInRect(const core::rectf& rect);
		void setSelected(bool sel) { m_bSelected = sel; }
		bool isSelected() { return m_bSelected; }
		void setEditorWorldPos(f32 x, f32 y) { m_basicAttr.worldX=x; m_basicAttr.worldY=y; }
		void addEditorWorldPos(f32 dx, f32 dy) { m_basicAttr.worldX+=dx; m_basicAttr.worldY+=dy; }
		void alignEditorWorldPos() { m_basicAttr.worldX = f32((s32)m_basicAttr.worldX); m_basicAttr.worldY = f32((s32)m_basicAttr.worldY); }
		void addEditorAngle(f32 angleDelta) { m_basicAttr.angle += angleDelta; }
		void addEditorSizeScale(f32 dsx, f32 dsy) { m_basicAttr.sizeScaleX+=dsx; m_basicAttr.sizeScaleY+=dsy; }
		void updateEditor(f32 dt);
		ActorBasicAttr& getBasicAttr() { return m_basicAttr; }
		ActorPhyAttr& getPhyAttr() { return m_phyAttr; }
		ActorGfxAttr& getGfxAttr() { return m_gfxAttr; }
		void refreshByEditor();
		const core::rectf& getBoundRect() const { return m_boundRect; }


	private:		
		Phy2dWorld* m_phyWorld;
		
		b2Body * m_body;

		CXAnimObject* m_pAnimObj;	

		f32 m_worldX, m_worldY;

		//initial attr in world
		ActorBasicAttr m_basicAttr;
		ActorPhyAttr m_phyAttr;
		ActorGfxAttr m_gfxAttr;

		///////////////for editor////////////////////
		core::rectf m_boundRect;
		bool m_bSelected;
	};

}//phy2d
}//flt

#endif //FLT_PHY2D_ACTOR_H
