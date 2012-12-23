#ifndef KF_ANIM_MANAGER_H
#define KF_ANIM_MANAGER_H

#include "firstlight.h"
#include "kfAnim.h"

using namespace flt;

namespace kfAnim
{

class AnimManager: public ResourceManager<AnimPtr>, public utils::ISingleton<AnimManager>
{
public:
	AnimManager(){};
	virtual ~AnimManager(){};

	virtual AnimPtr createRes(const stringc& name, int flag);

	virtual AnimPtr cloneRes(const AnimPtr& rhs, const stringc& newResName);

};

}//namespace kfAnim

#endif //KF_ANIM_MANAGER_H

