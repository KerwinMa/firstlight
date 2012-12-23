#include "animObject.h"
#include "IAnimable.h"
#include "animationSet.h"

namespace flt
{
namespace anim
{		
	AnimObject::AnimObject()
		:m_pAnimableProvider(0),
		 m_pCurrentAnimInst(0)
	{

	}

	AnimObject::~AnimObject()
	{
		clearAll();
	}

	void AnimObject::clearAll()
	{
		m_animSet = 0;
		m_pAnimableProvider = 0;

		AnimInstMap::iterator iter = m_animInstances.begin();

		for(; iter!=m_animInstances.end(); ++iter)
		{
			AnimInstance* animInst = (*iter).second;
			delete animInst;
		}		
		m_animInstances.clear();		
	}

	bool AnimObject::bindTargetToChannelInst(ChannelInstance *channelInst)
	{
		FLT_ASSERT(m_pAnimableProvider!=0);

		AnimationChannel *channel = channelInst->m_channel;
		const stringc& animableName = channel->getAnimableName();
		const stringc& targetName = channel->getTargetName();

		int animableNum = m_pAnimableProvider->getAnimableNum();
		for(int i=0; i<animableNum; ++i)
		{
			IAnimable* animable = m_pAnimableProvider->getAnimable(i);
			if(animable->getName() == animableName)
			{
				void* target = animable->getTarget(targetName);
				channelInst->m_animable = animable;
				channelInst->m_target = target;
				return true;
			}
		}

		return false;
	}

	void AnimObject::init(IAnimableProvider *pAnimableProvider, const AnimationSetPtr& animSet)
	{		
		FLT_ASSERT(pAnimableProvider!=0);	
		FLT_ASSERT(animSet.isValid());
		FLT_ASSERT(animSet->isReady());//到这儿的AnimationSet里面的所有动画必须都是init好的，包括里面channel和track的bind是搞好的了

		m_animSet = animSet;

		if(pAnimableProvider->getAnimableNum()<=0)
		{
			FLT_ASSERT(false);
			return;
		}

		m_pAnimableProvider = pAnimableProvider;

		//create animInstances for each anim in anim set
		int animNum = m_animSet->getAnimationNum();

		for(int i=0; i<animNum; ++i)
		{			
			Animation* anim = m_animSet->getAnimation(i);
			AnimInstance *animInst = new AnimInstance();
			animInst->m_anim = anim;			
			m_animInstances[anim->getName()] = animInst;

			int channelNum = anim->getChannelNum();
			for(int j=0; j<channelNum; ++j)
			{
				ChannelInstance *channelInst = new ChannelInstance();
				channelInst->m_channel = anim->getChannel(j);
				animInst->m_channels.push_back(channelInst);

				//bind animable targets to anim channels
				//for each channel, find the animable and target, set the target pointer
				bindTargetToChannelInst(channelInst);
			}	

			//init anim inst, important!
			animInst->init();
		}
	

		//注意：由于Animation是共享的，所以不能把target指针存放在channel里面，或者说channel应该不属于anim,
		//channel数据属于anim,但channel实例属于animObject,类的名字应该也有所改变

	}

	void AnimObject::playAnimation(const stringc& animName, ELoopSetting loopSetting)
	{
		if(m_pCurrentAnimInst!=0 && !m_pCurrentAnimInst->isOver())
		{
			m_pCurrentAnimInst->skip();
		}

		if(m_animInstances.find(animName)!=m_animInstances.end())
		{
			m_pCurrentAnimInst = m_animInstances[animName];
			m_pCurrentAnimInst->start(loopSetting);
		}
	}

	void AnimObject::pauseAnimation()
	{
		if(m_pCurrentAnimInst!=0)
		{
			m_pCurrentAnimInst->pause();
		}
	}

	void AnimObject::resumeAnimation()
	{
		if(m_pCurrentAnimInst!=0)
		{
			m_pCurrentAnimInst->resume();
		}
	}

	void AnimObject::skipAnimation()
	{
		if(m_pCurrentAnimInst!=0)
		{
			m_pCurrentAnimInst->skip();
		}
	}

	void AnimObject::update(f32 dt)
	{
		if(m_pCurrentAnimInst!=0)
		{
			m_pCurrentAnimInst->update(dt);
		}
	}

	bool AnimObject::isAnimationOver()
	{
		if(m_pCurrentAnimInst!=0)
		{
			return m_pCurrentAnimInst->isOver();
		}
		else
		{
			return true;
		}
	}	

} //end namespace anim
} //end namespace flt