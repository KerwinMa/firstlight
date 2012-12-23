#ifndef FLT_UI_PANE_H
#define FLT_UI_PANE_H

#include "ui/UIObject.h"
#include "renderer/Color.h"

namespace flt
{

FLT_FORWARD_PTR(CXSprite)
class CXAnimObject;

namespace ui
{
	class UIPane: public UIObject
	{
	public:
		UIPane();
		virtual ~UIPane();

		virtual bool loadFromXML(TiXmlElement *xmlUIObjNode);
		virtual void saveToXML(TiXmlElement *xmlUIObjNode);		
		virtual void init();
		virtual void update(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);		
		virtual void notifyChangePrameter();

	protected:
		stringc m_spriteName;
		CXSpritePtr m_sprite;
		CXAnimObject *m_cxAnimObj;
		s32 m_animID;

		renderer::Color m_color;
	};

}//ui
}//flt

#endif //FLT_UI_PANE_H
