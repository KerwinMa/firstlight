#include "phy2dActor.h"
#include "renderer/IRenderer.h"
#include "phy2dWorld.h"
#include "utils/Logger.h"
#include "xlib2d/XSpriteMgr.h"

namespace flt
{
namespace phy2d
{

	Phy2dActor::Phy2dActor(Phy2dWorld* phyWorld)
		:m_phyWorld(phyWorld),
		 m_body(0),
		 m_pAnimObj(0),
		 m_worldX(0),
		 m_worldY(0),
		 //for editor
		 m_bSelected(false)
	{

	}

	Phy2dActor::~Phy2dActor()
	{
		SAFE_DEL(m_pAnimObj);
	}

	bool Phy2dActor::loadFromXML(TiXmlElement *xmlActorNode)
	{		
		int tmp;

		TiXmlHandle hXmlNode(xmlActorNode);		

		TiXmlElement * basicAttrNode = hXmlNode.FirstChild("basic").Element();
		if(basicAttrNode==0)
		{
			DBG("engineDbg_Phy2d","Fail to load Actor, missing basic node!");
			return false;
		}

		basicAttrNode->QueryFloatAttribute("x", &m_basicAttr.worldX);
		basicAttrNode->QueryFloatAttribute("y", &m_basicAttr.worldY);
		basicAttrNode->QueryFloatAttribute("angle", &m_basicAttr.angle);
		basicAttrNode->QueryFloatAttribute("sizeScaleX", &m_basicAttr.sizeScaleX);
		basicAttrNode->QueryFloatAttribute("sizeScaleY", &m_basicAttr.sizeScaleY);		
		basicAttrNode->QueryIntAttribute("visible", &tmp);
		m_basicAttr.isVisible = (tmp==1);

		TiXmlElement * gfxAttrNode = hXmlNode.FirstChild("gfx").Element();
		if(gfxAttrNode==0)
		{
			DBG("engineDbg_Phy2d","Fail to load Actor, missing gfx node!");
			return false;
		}

		gfxAttrNode->QueryStringAttribute("sprite", &m_gfxAttr.sprName);		
		gfxAttrNode->QueryIntAttribute("animID", &m_gfxAttr.animID);
		gfxAttrNode->QueryFloatAttribute("a", &m_gfxAttr.a);
		gfxAttrNode->QueryFloatAttribute("r", &m_gfxAttr.r);
		gfxAttrNode->QueryFloatAttribute("g", &m_gfxAttr.g);
		gfxAttrNode->QueryFloatAttribute("b", &m_gfxAttr.b);
		gfxAttrNode->QueryIntAttribute("layer", &m_gfxAttr.layer);

		TiXmlElement * phyAttrNode = hXmlNode.FirstChild("phy").Element();
		if(phyAttrNode==0)
		{
			DBG("engineDbg_Phy2d","Fail to load Actor, missing phy node!");
			return false;
		}

		phyAttrNode->QueryIntAttribute("isEnable", &tmp);
		m_phyAttr.isEnable = (tmp==1);
		phyAttrNode->QueryIntAttribute("static", &tmp);
		m_phyAttr.isStatic = (tmp==1);
		phyAttrNode->QueryIntAttribute("isBall", &tmp);
		m_phyAttr.isBall = (tmp==1);
		phyAttrNode->QueryFloatAttribute("density", &m_phyAttr.density);	
		phyAttrNode->QueryFloatAttribute("friction", &m_phyAttr.friction);	
		phyAttrNode->QueryFloatAttribute("restitution", &m_phyAttr.restitution);	
		tmp = 0;
		phyAttrNode->QueryIntAttribute("isSensor", &tmp);			
		m_phyAttr.isSensor = (tmp==1);		
		phyAttrNode->QueryFloatAttribute("linearDamping", &m_phyAttr.linearDamping);
		phyAttrNode->QueryFloatAttribute("angularDamping", &m_phyAttr.angularDamping);
			
		return true;
	}

	void Phy2dActor::saveToXML(TiXmlElement *xmlActorNode)
	{		
		TiXmlElement * basicAttrNode = new TiXmlElement( "basic" );  
		xmlActorNode->LinkEndChild( basicAttrNode );

		basicAttrNode->SetDoubleAttribute("x", m_basicAttr.worldX);
		basicAttrNode->SetDoubleAttribute("y", m_basicAttr.worldY);
		basicAttrNode->SetDoubleAttribute("angle", m_basicAttr.angle);		
		basicAttrNode->SetDoubleAttribute("sizeScaleX", m_basicAttr.sizeScaleX);
		basicAttrNode->SetDoubleAttribute("sizeScaleY", m_basicAttr.sizeScaleY);
		basicAttrNode->SetAttribute("visible", m_basicAttr.isVisible?1:0);

		TiXmlElement * gfxAttrNode = new TiXmlElement( "gfx" );  
		xmlActorNode->LinkEndChild( gfxAttrNode );

		gfxAttrNode->SetAttribute("sprite", m_gfxAttr.sprName);
		gfxAttrNode->SetAttribute("animID", m_gfxAttr.animID);
		gfxAttrNode->SetDoubleAttribute("a", m_gfxAttr.a);		
		gfxAttrNode->SetDoubleAttribute("r", m_gfxAttr.r);
		gfxAttrNode->SetDoubleAttribute("g", m_gfxAttr.g);
		gfxAttrNode->SetDoubleAttribute("b", m_gfxAttr.b);
		gfxAttrNode->SetAttribute("layer", m_gfxAttr.layer);

		TiXmlElement * phyAttrNode = new TiXmlElement( "phy" );  
		xmlActorNode->LinkEndChild( phyAttrNode );

		phyAttrNode->SetAttribute("isEnable", m_phyAttr.isEnable?1:0);
		phyAttrNode->SetAttribute("static", m_phyAttr.isStatic?1:0);
		phyAttrNode->SetAttribute("isBall", m_phyAttr.isBall?1:0);	
		phyAttrNode->SetDoubleAttribute("density", m_phyAttr.density);	
		phyAttrNode->SetDoubleAttribute("friction", m_phyAttr.friction);	
		phyAttrNode->SetDoubleAttribute("restitution", m_phyAttr.restitution);	
		phyAttrNode->SetAttribute("isSensor", m_phyAttr.isSensor?1:0);	
		phyAttrNode->SetDoubleAttribute("linearDamping", m_phyAttr.linearDamping);	
		phyAttrNode->SetDoubleAttribute("angularDamping", m_phyAttr.angularDamping);	
	}

	bool Phy2dActor::init()
	{
		return init(m_basicAttr, m_phyAttr, m_gfxAttr);
	}

	bool Phy2dActor::init(const Phy2dActor& rhs)
	{
		return init(rhs.m_basicAttr, rhs.m_phyAttr, rhs.m_gfxAttr);
	}

	bool Phy2dActor::init(const ActorBasicAttr& basicAttr, const ActorPhyAttr& phyAttr, const ActorGfxAttr& gfxAttr, bool isRefreshInEditor)
	{
		if(&m_basicAttr != &basicAttr)
			m_basicAttr = basicAttr;
		if(&m_phyAttr != &phyAttr)
			m_phyAttr = phyAttr;
		if(&m_gfxAttr != &gfxAttr)
			m_gfxAttr = gfxAttr;

		bool hasPhy = m_phyAttr.isEnable && !isRefreshInEditor;

		m_worldX = basicAttr.worldX;
		m_worldY = basicAttr.worldY;

		XSpriteManager &xspMgr = XSpriteManager::getInstance();
		
		if(m_pAnimObj==0 && !isRefreshInEditor)
		{
			m_pAnimObj = new CXAnimObject(); 
		}
		else
		{
			m_pAnimObj->SetScale(1.0f);
			m_pAnimObj->SetRotation(0.0f);
		}

		m_pAnimObj->SetSprite(xspMgr.getRes(gfxAttr.sprName));
		m_pAnimObj->SetAnim(gfxAttr.animID,true);
		m_pAnimObj->SetSizeScale(basicAttr.sizeScaleX, basicAttr.sizeScaleY);

		///TODO: Phy2dActor can has circle or polygon shape,
		///		if it is circle using the half-width of bound rect,
		///		if it is polygon, using modules transformed rect as shapes.
		
		//boundRect is in frame coordinates, the axis is y down
		core::rectf boundRect;
		m_pAnimObj->CalcAnimFrameRect(gfxAttr.animID, 0, boundRect);

		m_boundRect = boundRect;

		b2Shape* pShape=0;
		f32 phyX, phyY;

		if(hasPhy)
		{
			f32 ratio = m_phyWorld->m_ratio;

			b2Vec2 pos0, pos1;
			pos0.x = boundRect.UpperLeftCorner.X/ratio;
			pos0.y = -boundRect.UpperLeftCorner.Y/ratio;
			pos1.x = boundRect.LowerRightCorner.X/ratio;
			pos1.y = -boundRect.LowerRightCorner.Y/ratio;						

			//convert world position to box2d's position
			phyX = m_worldX/ratio;
			phyY = m_worldY/ratio;

			if(phyAttr.isStatic)
			{
				m_body = m_phyWorld->m_staticBody;
				m_body->SetUserData(0);
			}
			else
			{
				b2BodyDef bodyDef;
				bodyDef.position.Set(phyX, phyY);		
				bodyDef.type = b2_dynamicBody;
				
				bodyDef.linearDamping = phyAttr.linearDamping;
				bodyDef.angularDamping = phyAttr.angularDamping;

				m_body = m_phyWorld->m_world->CreateBody(&bodyDef);
				m_body->SetUserData(this);
			}
						
			//make shape
			if(phyAttr.isBall)
			{
				b2CircleShape *shape = new b2CircleShape();
				pShape = shape;

				shape->m_radius = boundRect.getWidth()/(2.0f*ratio);

				b2Vec2 center;
				center.x = (pos0.x+pos1.x)/2.0f;
				center.y = (pos0.y+pos1.y)/2.0f;
				
				if(phyAttr.isStatic)
				{
					center.x += phyX;
					center.y += phyY;					
				}
				
				shape->m_p.Set(center.x, center.y);			
			}
			else
			{			
				b2PolygonShape *shape = new b2PolygonShape();
				pShape = shape;

				b2Vec2 vertices[4];					
				vertices[0] = pos0;
				vertices[1].Set(pos0.x,pos1.y);
				vertices[2] = pos1;
				vertices[3].Set(pos1.x,pos0.y);

				if(phyAttr.isStatic)
				{
					//transform static object when init, and set anim obj angle arrcording to object angle
					b2Transform xf;
					xf.position.Set(phyX,phyY);
					xf.R.Set(basicAttr.angle*flt::core::DEGTORAD);
										
					for (int32 i = 0; i < 4; ++i)
					{
						vertices[i] = b2Mul(xf, vertices[i]);			
					}				
				}

				shape->Set(vertices,4);			
			}

		}//!hasPhy
									
		if(hasPhy)
		{
			b2FixtureDef fixtureDef;
			fixtureDef.shape = pShape;				
			fixtureDef.density = phyAttr.density;
			fixtureDef.friction = phyAttr.friction;
			fixtureDef.restitution = phyAttr.restitution;
			fixtureDef.isSensor = phyAttr.isSensor;
			b2Fixture* fixture = m_body->CreateFixture(&fixtureDef);

			if(!phyAttr.isStatic)
			{					
				m_body->SetTransform(b2Vec2(phyX,phyY),basicAttr.angle*flt::core::DEGTORAD);					
			}
			else
			{
				fixture->SetUserData(this);
			}
		}

		if(phyAttr.isStatic)
		{
			m_pAnimObj->SetRotation(basicAttr.angle);	
		}
		

		SAFE_DEL(pShape);
		
		return true;
	}

	void Phy2dActor::update(f32 dt)
	{
		FLT_ASSERT(m_pAnimObj!=0);
		
		if(m_phyAttr.isStatic || !m_phyAttr.isEnable)
		{
			f32 x,y;			
			m_phyWorld->worldToScreenPos(m_worldX, m_worldY, x, y);
			
			m_pAnimObj->SetPos(x,y);			
		}
		else
		{
			b2Vec2 pos = m_body->GetPosition();
			float32 angle = m_body->GetAngle();

			f32 x,y;
			m_phyWorld->physicsToWorldPos(pos, m_worldX, m_worldY);
			m_phyWorld->worldToScreenPos(m_worldX, m_worldY, x, y);
			
			m_pAnimObj->SetPos(x,y);
			m_pAnimObj->SetRotation(angle*flt::core::RADTODEG);

			//DBG("test","%4.2f %4.2f %4.2f\n", pos.x, pos.y, angle);
		}
		
		m_pAnimObj->UpdateAnim(dt);
	}

	void Phy2dActor::draw(const renderer::IRendererPtr& renderer)
	{
		FLT_ASSERT(m_pAnimObj!=0);

		if(m_basicAttr.isVisible)
		{	
			if(m_bSelected)
			{
				m_pAnimObj->GetSprite()->SetMaterial2D(renderer::BIMT_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR);
				m_pAnimObj->GetSprite()->SetColor(renderer::ColorBlue);
			}

			m_pAnimObj->SetScale(m_phyWorld->m_cameraZoom, m_phyWorld->m_cameraZoom);
			m_pAnimObj->Draw(renderer);

			if(m_bSelected)
			{
				m_pAnimObj->GetSprite()->SetMaterial2D(renderer::BIMT_2D_ALPHA_BLEND);
				m_pAnimObj->GetSprite()->UnsetColor();				
			}										
		}
	}

	void Phy2dActor::resetToInitial()
	{
		SAFE_DEL(m_pAnimObj);

		init();		
	}

	void Phy2dActor::destroyBody(b2World* world)
	{
		if(m_body!=0)
		{
			world->DestroyBody(m_body);
			m_body=0;
		}
	}

	void Phy2dActor::applyForce(const core::vector2df& vec)
	{
		b2Vec2 vec2 ;
		vec2.Set(vec.X, vec.Y);

		m_body->ApplyForce(vec2, m_body->GetPosition());
	}

	/////////////////for editor //////////////////////////////////////////

	core::rectf Phy2dActor::calcCurrentBound()
	{
		//f32 asx, asy;
		//m_phyWorld->worldToScreenPos(m_worldX,m_worldY,asx,asy);

		f32 zoom = m_phyWorld->getCameraZoom();
		core::rectf bound(m_boundRect);
		bound.scale(zoom,zoom);
		//bound+=core::position2df(asx,asy);		

		return bound;
	}

	void Phy2dActor::calcEditorPointInActorSpace(f32 &sx, f32 &sy)
	{			
		f32 asx, asy;
		m_phyWorld->worldToScreenPos(m_basicAttr.worldX,m_basicAttr.worldY,asx,asy);

		b2Vec2 vert;					
		vert.Set(sx-asx, sy-asy);			

		b2Transform xf;
		xf.position.Set(0,0);

		xf.R.Set(m_basicAttr.angle*flt::core::DEGTORAD);
		
		vert = b2Mul(xf, vert);	

		sx = vert.x;
		sy = vert.y;
	}

	bool Phy2dActor::isEditorPointIn(f32 sx, f32 sy)
	{
		if(m_pAnimObj!=0)
		{			
			core::rectf bound = calcCurrentBound();
			
			calcEditorPointInActorSpace(sx,sy);								
			
			if(bound.isPointInside(core::vector2df(sx,sy)))
				return true;
		}

		return false;
	}

	bool Phy2dActor::isEditorInRect(const core::rectf& rect)
	{		
		if(m_phyWorld!=0)
		{			
			f32 wx = m_basicAttr.worldX;
			f32 wy = m_basicAttr.worldY;
			f32 sx, sy;

			m_phyWorld->worldToScreenPos(wx,wy,sx,sy);

			if(rect.isPointInside(core::vector2df(sx,sy)))
			{
				return true;
			}
		}

		return false;
	}

	void Phy2dActor::updateEditor(f32 dt)
	{
		FLT_ASSERT(m_pAnimObj!=0);

		f32 x,y;			
		m_phyWorld->worldToScreenPos(m_basicAttr.worldX, m_basicAttr.worldY, x, y);
			
		m_pAnimObj->SetPos(x,y);

		m_pAnimObj->SetRotation(m_basicAttr.angle);

		m_pAnimObj->SetSizeScale(m_basicAttr.sizeScaleX, m_basicAttr.sizeScaleY);
						
		m_pAnimObj->UpdateAnim(dt);
	}

	void Phy2dActor::refreshByEditor()
	{
		init(m_basicAttr, m_phyAttr, m_gfxAttr, true);
	}

}//phy2d
}//flt