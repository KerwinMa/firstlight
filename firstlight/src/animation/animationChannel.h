#ifndef FLT_ANIMATION_CHANNEL_H
#define FLT_ANIMATION_CHANNEL_H

#include "common.h"
#include "animation/animationTrack.h"

namespace flt
{
namespace anim
{

	/// channel is a binding from animable's target to animation track.

	class AnimationChannel
	{
		public:
			AnimationChannel();

			~AnimationChannel();

			bool loadFromXML(TiXmlElement *xmlNodeChannel);

			void saveToXML(TiXmlElement *xmlNodeChannel);

			void setAnimableName(const stringc& animableName) { m_animableName = animableName; }

			const stringc& getAnimableName() const { return m_animableName; }

			void setTargetName(const stringc& targetName) { m_targetName = targetName; }

			const stringc& getTargetName() const { return m_targetName; }

			void setTrackName(const stringc& trackName) { m_trackName = trackName; }

			const stringc& getTrackName() const { return m_trackName; }

			void setTrack(AnimationTrack* track) { m_track = track; }

			AnimationTrack* getTrack() { return m_track; }
			
		private:
			stringc m_trackName;
			stringc m_animableName;
			stringc m_targetName;				

			AnimationTrack* m_track;			
	};


} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMATION_CHANNEL_H