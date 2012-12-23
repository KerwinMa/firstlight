#include "XSpriteMgr.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "archive/ArchiveManager.h"
#include "renderer/material/TextureMgr.h"
#include "utils/StringUtil.h"

namespace flt
{

XSpriteManager::XSpriteManager()
{
}

CXSpritePtr XSpriteManager::createRes(const stringc& name, int flag)
{		
	CXSpritePtr sprite = new CXSprite();
	if(sprite->LoadFromFile(name.c_str()))
	{
		sprite->setName(name);
		DBG("engineDbg_Res","create sprite name: %s", name.c_str());
	}
	else
	{
		sprite = 0;
		DBG("engineDbg_Res","can not create sprite, name: %s", name.c_str());
	}	

	if(sprite.isValid())
	{
		stringc baseName, exten, path;

		StringUtil::splitFullFilename(name, baseName, exten, path);
		baseName = path+baseName+".tga";

		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();				
		sprite->SetTexture(texMgr.getRes(baseName));
	}

	return sprite;
}

CXSpritePtr XSpriteManager::cloneRes(const CXSpritePtr& rhs, const stringc& newResName)
{	
	CXSpritePtr newSprite = rhs;
	return newSprite;
}


}// end namespace flt

