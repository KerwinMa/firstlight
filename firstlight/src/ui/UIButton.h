#ifndef FLT_UI_BUTTON_H
#define FLT_UI_BUTTON_H

#include "ui/UIPane.h"
#include "xlib2d/XFont.h"

namespace flt
{
namespace ui
{
	class UIButton: public UIPane
	{
	public:
		UIButton();
		virtual ~UIButton();
		
		virtual bool loadFromXML(TiXmlElement *xmlUIObjNode);
		virtual void saveToXML(TiXmlElement *xmlUIObjNode);
		virtual void init();
		virtual bool processEvent(const IEvent& event);
		virtual void update(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);		

		virtual bool isHolding() { return m_isHolding; }		

	protected:
		void drawButtonText(const renderer::IRendererPtr& renderer, const stringc& text);

	protected:
		stringc m_fontName;
		CXFontPtr m_font;
		
		s32 m_pressedAnimID;
		s32 m_doneAnimID;
		
		stringc m_text;
		
		bool m_isHolding;		
	};

}//ui
}//flt

#endif //FLT_UI_BUTTON_H
