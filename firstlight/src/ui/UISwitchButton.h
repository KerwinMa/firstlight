#ifndef FLT_UI_SWITCH_BUTTON_H
#define FLT_UI_SWITCH_BUTTON_H

#include "ui/UIButton.h"

namespace flt
{
namespace ui
{
	class UISwitchButton: public UIButton
	{
	public:
		UISwitchButton();
		virtual ~UISwitchButton();
		
		virtual bool loadFromXML(TiXmlElement *xmlUIObjNode);
		virtual void saveToXML(TiXmlElement *xmlUIObjNode);
		virtual void init();
		virtual bool processEvent(const IEvent& event);
		virtual void update(f32 dt);
		virtual void draw(const renderer::IRendererPtr& renderer);	

		void switchOnOff(bool on) { m_isOn = on; setAnimState(); }
		bool isOn() { return m_isOn; }

	private:
		void setAnimState();

	protected:
		s32 m_offAnimID;
		s32 m_offPressedAnimID;
		stringc m_offText;

		bool m_isOn;
	};

}//ui
}//flt

#endif //FLT_UI_SWITCH_BUTTON_H
