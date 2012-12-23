#include "kfAnimManager.h"

namespace kfAnim
{

AnimPtr AnimManager::createRes(const stringc& name, int flag)
{
	FileStream file;
	ArchiveManager::getInstance().openFile(file, name);
	AnimPtr anim = new Anim();
	anim->load(&file);
	anim->setName(name);	

	return anim;
}

AnimPtr AnimManager::cloneRes(const AnimPtr& rhs, const stringc& newResName)
{
	AnimPtr newAnim = rhs;
	return newAnim;
}

}//namespace kfAnim