#ifndef FLT_ANIMSET_MANAGER_H
#define FLT_ANIMSET_MANAGER_H

#include "animation/animationSet.h"
#include "ResourceManager.h"

namespace flt
{
namespace anim
{

	enum
	{
		ANIM_SET_XML,
		ANIM_SET_BIN
	};

	class AnimSetManager: public ResourceManager<AnimationSetPtr>, public utils::ISingleton<AnimSetManager>
	{
	public:
		AnimSetManager(){};
		virtual ~AnimSetManager(){};

		virtual AnimationSetPtr createRes(const stringc& name, int flag);

		virtual AnimationSetPtr cloneRes(const AnimationSetPtr& rhs, const stringc& newResName);

	};

} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMSET_MANAGER_H

