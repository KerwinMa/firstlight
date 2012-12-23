#ifndef FLT_TEXTURE_MGR_H
#define FLT_TEXTURE_MGR_H

#include "TextureLayer.h"
#include "ResourceManager.h"
#include "BaseObject.h"
#include "sharePtr.h"

namespace flt
{	
namespace renderer
{	

class IRenderer;

class Image;

enum ETexCreateFlag
{
	ETCF_Normal,
	ETCF_Thread,
};


class TextureManager: public ResourceManager<TexturePtr>, public utils::ISingleton<TextureManager>
{
public:
	TextureManager();
	virtual ~TextureManager();

	virtual TexturePtr createRes(const stringc& name, int flag);

	virtual TexturePtr cloneRes(const TexturePtr& rhs, const stringc& newResName);

	void setRenderer(IRenderer *renderer);

	IRenderer* getRenderer() { return m_renderer; }

	//this is a patch for thread load texture
	void setThreadLoadedImage(Image*& image)
	{
		m_threadLoadImagePointer = &image;
	}

private:	
	IRenderer* m_renderer;

	Image** m_threadLoadImagePointer;

};


} //end namespace renderer
} //end namespace flt

#endif //FLT_TEXTURE_MGR_H
