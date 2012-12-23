#include "animationChannel.h"

namespace flt
{
namespace anim
{

	AnimationChannel::AnimationChannel()
		:m_track(0)
	{
	}

	AnimationChannel::~AnimationChannel()
	{
	}

	bool AnimationChannel::loadFromXML(TiXmlElement *xmlNodeChannel)
	{
		xmlNodeChannel->QueryStringAttribute("track",&m_trackName);	
		xmlNodeChannel->QueryStringAttribute("animable",&m_animableName);	
		xmlNodeChannel->QueryStringAttribute("target",&m_targetName);	
		return true;
	}

	void AnimationChannel::saveToXML(TiXmlElement *xmlNodeChannel)
	{
		xmlNodeChannel->SetAttribute("track",m_trackName.c_str());
		xmlNodeChannel->SetAttribute("animable",m_animableName.c_str());
		xmlNodeChannel->SetAttribute("target",m_targetName.c_str());
	}

} //end namespace anim
} //end namespace flt