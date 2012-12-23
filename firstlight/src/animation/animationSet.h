#ifndef FLT_ANIMATION_SET_H
#define FLT_ANIMATION_SET_H

#include "common.h"
#include "animation/animation.h"
#include "externs/tinyxml/tinyxml.h"

namespace flt
{
namespace anim
{

	typedef map_t<stringc, Animation*> AnimMap;

	FLT_FORWARD_PTR(AnimationSet)

	class AnimationSet: public BaseObject
	{
	public:
		AnimationSet();
		~AnimationSet();

		/// load Animation Set from xml file
		bool loadFromXMLFile(const char* xmlFile);
		void saveToXMLFile(const char* xmlFile);

		bool loadFromXML(TiXmlElement* xmlNodeAnimSet);
		void saveToXML(TiXmlElement* xmlNodeAnimSet);

		/// init the anim set, build the channel-track link
		void init();

		bool isReady() { return m_isReady; }

		//public for manully control add/remove anim, e.g. in editor
		void addAnimation(Animation *anim);
		
		void removeAnimation(const stringc& name);

		int getAnimationNum() { return (int)m_animations.size(); }

		Animation* getAnimation(const stringc& name);

		Animation* getAnimation(int index);

	private:

		void clearAll();

		AnimMap m_animations;

		bool m_isReady; //if it is ready for use, loaded and inited

	};


} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMATION_SET_H