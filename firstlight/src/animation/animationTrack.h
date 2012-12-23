#ifndef FLT_ANIMATION_TRACK_H
#define FLT_ANIMATION_TRACK_H

#include "common.h"
#include "animation/keyframe.h"
#include "externs/tinyxml/tinyxml.h"

namespace flt
{
namespace anim
{

	enum EAnimTrackDataType
	{
		ATDT_NULL,
		ATDT_FLOAT=1,
		ATDT_VECTOR2F,
		ATDT_VECTOR3F,
	};

	class AnimationTrack
	{

	public:
		AnimationTrack();

		~AnimationTrack();

		bool loadFromXML(TiXmlElement *xmlNodeTrack);

		void saveToXML(TiXmlElement *xmlNodeTrack);

		const stringc& getName() const { return m_name; }

		void setName(const stringc& name) { m_name = name; }

		//int getKeyFrameCount() const;

		//KeyFrame* getKeyFrame(u32 index);

		//const KeyFrame* getKeyFrame(u32 index) const;

		void apply(f32 timeMs, void* &target);

		bool getFrame(f32 timeMs, KeyFrame& frame) const;

		f32 getTotalTimeMs();		

		array_t<KeyFrame *>::const_iterator getUpperBound(f32 timeMs) const;	

		void addKeyFrame(KeyFrame* kf) { m_keyFrames.push_back(kf); }

	private:
		EAnimTrackDataType stringToDataType(const stringc& str);
		void dataTypeToString(EAnimTrackDataType dataType, stringc& str);
		KeyFrame* createKeyFrame();

		void clearAll();

	private:
		stringc m_name;
		EAnimTrackDataType m_dataType;
	
		array_t<KeyFrame*> m_keyFrames;
		
		KeyFrame* m_computedFrame;//frame computed from time
	};


} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMATION_TRACK_H