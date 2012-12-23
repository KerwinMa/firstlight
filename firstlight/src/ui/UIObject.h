#ifndef FLT_UI_OBJECT_H
#define FLT_UI_OBJECT_H

#include "common.h"
#include "core/vector2d.h"
#include "core/rect.h"
#include "core/matrix4.h"
#include "externs/tinyxml/tinyxml.h"
#include "sharePtr.h"
#include "animation/IAnimable.h"

namespace flt
{

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)
}

class IEvent;

namespace ui
{
	enum ERotationCenter
	{
		ER_LEFT_TOP,
		ER_RECT_CENTER,
	};
	
	class UIObject: public anim::IAnimable
	{
	public:
		UIObject();
		virtual ~UIObject(){};
		
		virtual bool loadFromXML(TiXmlElement *xmlUIObjNode);
		virtual void saveToXML(TiXmlElement *xmlUUIObjNode);
		virtual void init(){};

		//! return true to absorb the event
		virtual bool processEvent(const IEvent& event) { return false; }

		virtual void update(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);

		virtual void attachToParent(UIObject *parent) { m_parentObj = parent; updateFinalRect(); }

		//TODO: 判断动画到底修改了什么参数，然后再更新，而不是全部更新
		virtual void notifyChangePrameter() { m_isLocalXformDirty = true; updateFinalRect(); }

		u16 getOwnerLayerID() { return m_ownerLayerID; }
		void setOwnerLayerID(u16 id) { m_ownerLayerID = id; }

		u16 getObjID() { return m_objID; }
		void setObjID(u16 id) { m_objID = id; }		

		f32 getAlpha() { return m_alpha; }
		f32 getRed() { return m_r; }
		f32 getGreen() { return m_g; }
		f32 getBlue() { return m_b; }

		const core::matrix4& getLocalXForm() 
		{ 
			updateLocalXForm();
			return m_localXform; 
		}

		void setX(f32 x) { m_pos.X = x; m_isLocalXformDirty = true; }
		void setY(f32 y) { m_pos.Y = y; m_isLocalXformDirty = true; }
		void setRotation(f32 rot) { m_rotation = rot; m_isLocalXformDirty = true; }
		void setScaleX(f32 scaleX) { m_scaleX = scaleX; m_isLocalXformDirty = true; }
		void setScaleY(f32 scaleY) { m_scaleY = scaleY; m_isLocalXformDirty = true; }

		void setRotationCenterType(ERotationCenter erc) { m_rotationCenter = erc; }

		/// IAnimable interface
		virtual const stringc& getName() const { return m_name; }

		virtual void* getTarget(const stringc& name);

		virtual void notifyTargetModified();

#ifdef FLT_DEBUG
		void setShowDebug(bool show) { m_bShowDebug = show; }
#else
		void setShowDebug(bool show) {}
#endif

	protected:
		stringc m_name;

		core::rectf m_localRect;
		core::rectf m_finalRect;

		core::vector2df m_pos;
		f32 m_scaleX, m_scaleY;
		f32 m_rotation;
		ERotationCenter m_rotationCenter;
		
		f32 m_alpha;
		f32 m_r, m_g, m_b;
		
		//! ID of owner UILayer
		u16 m_ownerLayerID;

		//! ID of this UIObject on the UILayer
		u16 m_objID;

		UIObject *m_parentObj;

		core::matrix4 m_localXform;

#ifdef FLT_DEBUG
		bool m_bShowDebug;
#endif

	protected:
		void updateFinalRect();

	private:
		void updateLocalXForm();
		bool m_isLocalXformDirty;	
	};

}//ui
}//flt

#endif //FLT_UI_OBJECT_H
