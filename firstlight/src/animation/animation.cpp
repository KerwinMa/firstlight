#include "animation.h"

namespace flt
{
namespace anim
{
	Animation::Animation()
		:m_isLoop(false)
	{
		///Temp code: to make some anim data//////////////////////////////////////
		//AnimationTrack* track = new AnimationTrack();
		//track->setName("btn move in");
		//addTrack(track);
		//track = new AnimationTrack();
		//track->setName("btn move out");
		//addTrack(track);

		//AnimationChannel *channel = new AnimationChannel();	
		//channel->setAnimableName("play button");
		//channel->setTargetName("pos2d");
		//channel->setTrackName("btn move in");
		//addChannel(channel);
		//channel = new AnimationChannel();	
		//channel->setAnimableName("play button");
		//channel->setTargetName("pos2d");
		//channel->setTrackName("btn move out");
		//addChannel(channel);
		//////////////////////////////////////////////////////////////////////////
	}

	Animation::~Animation()
	{
		clearAll();
	}

	void Animation::clearAll()
	{
		for(int i=0, trackNum=(int)m_tracks.size(); i<trackNum; ++i)
		{
			delete m_tracks[i];			
		}
		m_tracks.clear();
			
		for(int i=0, channelNum=(int)m_channels.size(); i<channelNum; ++i)
		{
			delete m_channels[i];			
		}
		m_channels.clear();
	}

	bool Animation::loadFromXML(TiXmlElement *xmlNodeAnim)
	{
		TiXmlHandle hXmlNode(xmlNodeAnim);

		xmlNodeAnim->QueryStringAttribute("name",&m_name);
		int loop;
		xmlNodeAnim->QueryIntAttribute("loop",&loop);
		m_isLoop = loop;

		//load tracks
		TiXmlElement* pTrackNode = hXmlNode.FirstChild( "Tracks" ).FirstChild("Track").Element();
		for( ; pTrackNode; pTrackNode=pTrackNode->NextSiblingElement())
		{
			AnimationTrack* track = new AnimationTrack();
			
			if(track->loadFromXML(pTrackNode))
			{
				addTrack(track);
			}
		}

		//load channels
		TiXmlElement* pChannelNode = hXmlNode.FirstChild( "Channels" ).FirstChild("Channel").Element();
		for( ; pChannelNode; pChannelNode=pChannelNode->NextSiblingElement())
		{
			AnimationChannel *channel = new AnimationChannel();	

			if(channel->loadFromXML(pChannelNode))
			{
				addChannel(channel);
			}
		}

		return true;
	}

	void Animation::saveToXML(TiXmlElement *xmlNodeAnim)
	{
		xmlNodeAnim->SetAttribute("name",m_name.c_str());
		xmlNodeAnim->SetAttribute("loop", m_isLoop);

		//save tracks
		TiXmlElement * tracksNode = new TiXmlElement( "Tracks" );  
		xmlNodeAnim->LinkEndChild( tracksNode );  		
		for(int i=0, trackNum=(int)m_tracks.size(); i<trackNum; ++i)
		{
			AnimationTrack* track = m_tracks[i];
			TiXmlElement *trackNode = new TiXmlElement("Track");
			track->saveToXML(trackNode);
			tracksNode->LinkEndChild(trackNode);
		}

		//save channels
		TiXmlElement * channelsNode = new TiXmlElement( "Channels" );  
		xmlNodeAnim->LinkEndChild( channelsNode );  		
		for(int i=0, channelNum=(int)m_channels.size(); i<channelNum; ++i)
		{
			AnimationChannel* channel = m_channels[i];
			TiXmlElement *channelNode = new TiXmlElement("Channel");
			channel->saveToXML(channelNode);
			channelsNode->LinkEndChild(channelNode);
		}		
	}

	//call after animatin load or created
	void Animation::bindChannels()
	{
		for(int i=0, channelNum=(int)m_channels.size(); i<channelNum; ++i)
		{
			if(m_channels[i]!=0)
			{
				bindTrackToChannel(m_channels[i]);
			}
		}
	}

	void Animation::bindTrackToChannel(AnimationChannel* channel)
	{		
		const stringc& trackName = channel->getTrackName();

		for(int i=0, trackNum=(int)m_tracks.size(); i<trackNum; ++i)
		{
			AnimationTrack* track = m_tracks[i];
			if(track!=0)
			{
				if(track->getName() == trackName)
				{
					channel->setTrack(track);
					break;
				}
			}
		}
	}

} //end namespace anim
} //end namespace flt