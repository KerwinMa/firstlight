#include "animationInstance.h"
#include "animationTrack.h"
#include "animationChannel.h"
#include "animation.h"
#include "IAnimable.h"
#include "utils/Logger.h"

namespace flt
{
namespace anim
{		
	AnimInstance::AnimInstance()
		:m_anim(0),m_bLoop(false),m_bPaused(false),m_maxTime(0.0f),m_playTime(0.0f)
	{

	}

	AnimInstance::~AnimInstance()
	{
		for(int i=0, channelInstNum=(int)m_channels.size(); i<channelInstNum; ++i)
		{
			delete m_channels[i];
		}

		m_channels.clear();
	}

	void AnimInstance::init()
	{
		m_maxTime = 0.0f;

		for(int i=0, channelInstNum=(int)m_channels.size(); i<channelInstNum; ++i)
		{
			AnimationTrack* pTrack = m_channels[i]->m_channel->getTrack();
			if(pTrack!=0 && m_maxTime < pTrack->getTotalTimeMs())
			{
				m_maxTime = pTrack->getTotalTimeMs();
			}
		}		
	}

	void AnimInstance::start(ELoopSetting loopSetting)
	{
		m_bPaused=false;
		m_playTime = 0;

		switch(loopSetting)
		{
		case LS_FORCE_LOOP:
			m_bLoop = true;
			break;
		case LS_FORCE_UNLOOP:
			m_bLoop = false;
			break;
		default:
			m_bLoop = m_anim->isLoop();
			break;
		}
	}
	
	void AnimInstance::pause()
	{
		m_bPaused=true;
	}
	
	void AnimInstance::resume()
	{
		m_bPaused=false;
	}
	
	void AnimInstance::skip()
	{
		m_playTime = m_maxTime;
		update(1.0f);
	}
	
	void AnimInstance::update(f32 dt)
	{
		if(m_bPaused)
			return;

		for(int i=0, channelInstNum=(int)m_channels.size(); i<channelInstNum; ++i)
		{
			AnimationTrack* pTrack = m_channels[i]->m_channel->getTrack();
			void* target = m_channels[i]->m_target;
			IAnimable* animable = m_channels[i]->m_animable;

			if(pTrack!=0 && target!=0)
			{
				pTrack->apply(m_playTime, target);
				animable->notifyTargetModified();
			}
		}		
		
		m_playTime+=dt;
		if(m_playTime>m_maxTime)
		{
			if(m_bLoop)
				m_playTime=0;
			else		
				m_playTime = m_maxTime;					
		}
	}

	bool AnimInstance::isOver() 
	{ 
		return !m_bLoop && (m_playTime>=m_maxTime); 
	}

} //end namespace anim
} //end namespace flt
