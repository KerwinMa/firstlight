#ifndef FLT_ANIM_OBJECT_H
#define FLT_ANIM_OBJECT_H

#include "common.h"
#include "sharePtr.h"
#include "animation/animationInstance.h"

namespace flt
{
namespace anim
{
	class IAnimableProvider;

	FLT_FORWARD_PTR(AnimationSet)

	class AnimObject
	{
	public:
		AnimObject();

		virtual ~AnimObject();

		//set IAnimableProvider and animation set, bind animable & anim channel
		void init(IAnimableProvider *pAnimableProvider, const AnimationSetPtr& animSet);

		void playAnimation(const stringc& animName, ELoopSetting loopSetting=LS_USE_ANIM_LOOP);

		void pauseAnimation();

		void resumeAnimation();

		void skipAnimation();

		void update(f32 dt);

		bool isAnimationOver();

	protected:

		bool bindTargetToChannelInst(ChannelInstance *channelInst);

		//one AnimObject can use only ONE AnimationSet at one time
		AnimationSetPtr m_animSet;

		// animables animated by animations		
		IAnimableProvider *m_pAnimableProvider;

		// animInstance will work at runtime
		AnimInstMap m_animInstances;

		// current playing AnimInst
		AnimInstance *m_pCurrentAnimInst;

		void clearAll();

	};

} //end namespace anim
} //end namespace flt

#endif //FLT_ANIM_OBJECT_H