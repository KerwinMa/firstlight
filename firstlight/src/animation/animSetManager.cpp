#include "animation/animSetManager.h"
#include "stream/FileStream.h"
#include "archive/ArchiveManager.h"

namespace flt
{
namespace anim
{
	AnimationSetPtr AnimSetManager::createRes(const stringc& name, int flag)
	{
		//FileStream file;
		//ArchiveManager::getInstance().openFile(file, name);
		//Note: xml has already use FileStream for load, but NOT for save
		
		AnimationSetPtr animSet = new AnimationSet();

		bool loadSuccess = false;

		if(flag==ANIM_SET_XML)
		{
			loadSuccess = animSet->loadFromXMLFile(name.c_str());			
		}			


		if(loadSuccess)
		{			
			animSet->setName(name);
		}
		else
		{
			animSet.reset();
		}
		
		return animSet;
	}

	AnimationSetPtr AnimSetManager::cloneRes(const AnimationSetPtr& rhs, const stringc& newResName)
	{
		AnimationSetPtr newAnim = rhs;
		return newAnim;
	}

} //end namespace anim
} //end namespace flt