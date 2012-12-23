#include "XFontMgr.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "archive/ArchiveManager.h"

namespace flt
{

XFontManager::XFontManager()
{
}

CXFontPtr XFontManager::createRes(const stringc& name, int flag)
{		
	CXFontPtr font = new CXFont();
	font->LoadFromFile(name.c_str());
	font->setName(name);

	DBG("engineDbg_Res","create font name: %s", name.c_str());

	return font;
}

CXFontPtr XFontManager::cloneRes(const CXFontPtr& rhs, const stringc& newResName)
{	
	CXFontPtr newFont = rhs;
	return newFont;
}


}// end namespace flt

