#ifndef FLT_XSPRITE_MGR_H
#define FLT_XSPRITE_MGR_H

#include "xlib2d/XSprite.h"
#include "ResourceManager.h"

namespace flt
{	

class XSpriteManager: public ResourceManager<CXSpritePtr>, public utils::ISingleton<XSpriteManager>
{
public:
	XSpriteManager();
	virtual ~XSpriteManager(){};

	virtual CXSpritePtr createRes(const stringc& name, int flag);

	virtual CXSpritePtr cloneRes(const CXSpritePtr& rhs, const stringc& newResName);
};


} //end namespace flt

#endif //FLT_XSPRITE_MGR_H

