#ifndef FLT_XFONT_MGR_H
#define FLT_XFONT_MGR_H

#include "xlib2d/XFont.h"
#include "ResourceManager.h"

namespace flt
{	

class XFontManager: public ResourceManager<CXFontPtr>, public utils::ISingleton<XFontManager>
{
public:
	XFontManager();
	virtual ~XFontManager(){};

	virtual CXFontPtr createRes(const stringc& name, int flag);

	virtual CXFontPtr cloneRes(const CXFontPtr& rhs, const stringc& newResName);
};


} //end namespace flt

#endif //FLT_XFONT_MGR_H

