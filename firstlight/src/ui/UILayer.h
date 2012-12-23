#ifndef FLT_UI_LAYER_H
#define FLT_UI_LAYER_H

#include "ui/UIObject.h"
#include "event/IEventReceiver.h"
#include "animation/animationSet.h"
#include "animation/animObject.h"

namespace flt
{
namespace ui
{
	
	class UILayer: public IEventReceiver, public anim::IAnimableProvider
	{
	public:
		UILayer();
		~UILayer();

		bool loadFromXMLFile(const char* xmlFile);		
		void saveXMLFile();
		void saveToXMLFile(const char* otherXMLFile);		

		bool loadFromXML(TiXmlElement *xmlNodeUILayer);
		void saveToXML(TiXmlElement *xmlNodeUILayer);

		void init();

		UIObject* getUIObjectByID(int id) { return m_uiObjs[id]; }

		virtual bool onEvent(const IEvent& event);

		void update(f32 dt);
		void draw(const renderer::IRendererPtr& renderer);

		void releaseAll();

		u16 getLayerID() const { return m_layerID; }
		void setLayerID(u16 id) { m_layerID = id; }

		void setActive(bool active);
		bool isActive() const { return m_bActive; }

		/// anim::IAnimableProvider interface ////////////
		virtual int getAnimableNum();
		virtual anim::IAnimable* getAnimable(int index);

		void playAnimation(const stringc& animName);
		bool isCurrentAnimationOver();
		const stringc& getCurrentAnimationName() const { return m_currentAnimName; }

		//for UITool
		//output layer id & button id defines
		void outputLayerDefs(stringc& output);

	private:
		u16 m_layerID;		
		stringc m_xmlFileName;
		array_t<UIObject*> m_uiObjs;

		UIObject *m_layerObj;

		UIObject* createUIObjectByType(const stringc& type);

		anim::AnimationSetPtr m_animSet;

		anim::AnimObject* m_pAnimObject;

		stringc m_currentAnimName;

		bool m_bActive;
	};

}//ui
}//flt

#endif //FLT_UI_LAYER_H
