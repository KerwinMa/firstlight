#ifndef FLT_ANIM_INSTANCE_H
#define FLT_ANIM_INSTANCE_H

#include "common.h"

namespace flt
{
namespace anim
{
	class AnimationChannel;
	class IAnimable;
	class Animation;

	struct ChannelInstance
	{	
		AnimationChannel *m_channel;
		IAnimable *m_animable;
		void *m_target;
	};

	enum ELoopSetting
	{
		LS_USE_ANIM_LOOP,
		LS_FORCE_LOOP,
		LS_FORCE_UNLOOP		
	};

	class AnimInstance
	{	
		friend class AnimObject;

	public:
		AnimInstance();			
		~AnimInstance();

		void init();
		void start(ELoopSetting loopSetting=LS_USE_ANIM_LOOP);
		void pause();
		void resume();
		void skip();
		void update(f32 dt);
		bool isOver();
	
	private:
		Animation* m_anim;
		array_t<ChannelInstance*> m_channels;

		bool m_bLoop;
		bool m_bPaused;
		f32 m_maxTime;
		f32 m_playTime;	
	};

	typedef map_t<stringc, AnimInstance*> AnimInstMap;

} //end namespace anim
} //end namespace flt

#endif //FLT_ANIM_INSTANCE_H
