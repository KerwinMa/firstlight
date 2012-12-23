#include "TextureMgr.h"
#include "renderer/IRenderer.h"
#include "utils/Logger.h"
#include "stream/FileStream.h"
#include "archive/ArchiveManager.h"
#include "renderer/image/Image.h"

#if FLT_TEXTURE_ENABLE_THREAD_LOAD

#include <assert.h>
#include "pthread.h"

pthread_mutex_t s_mutex;

#endif

using namespace flt;
using namespace renderer;

class SImageHold
{
public:
	stringc Name;
	Image** pImage;
};

void* backgroundLoadImage(void* data)
{
	SImageHold* imgHold = (SImageHold*)(data);	

#if FLT_TEXTURE_ENABLE_THREAD_LOAD
	DBG("thread","sub thread start loading image %s to pointer %d, mutex lock", imgHold->Name.c_str(), (int)(imgHold->pImage));
	pthread_mutex_lock(&s_mutex);
#endif
				
	Image* pImage = 0;
	
	FileStream file;
	ArchiveManager::getInstance().openFile(file, imgHold->Name);
	
	TextureManager& texMgr = renderer::TextureManager::getInstance();
	Image* img = texMgr.getRenderer()->createImageFromStream(&file);		
	
	if(img != NULL && !img->isSizePowerOf2())
	{
		pImage = new Image(img,true);
		delete img;
	}
	else
	{
		pImage = img;
	}

	*(imgHold->pImage) = pImage;	
	
#if FLT_TEXTURE_ENABLE_THREAD_LOAD
	pthread_mutex_unlock(&s_mutex);
	DBG("thread","sub thread end loading image %s to pointer %d, mutex unlock", imgHold->Name.c_str(), (int)(*imgHold->pImage));
#endif	

	delete imgHold;
	
	return 0;
}

namespace flt
{
namespace renderer
{

TextureManager::TextureManager()
	:m_renderer(0),m_threadLoadImagePointer(0)
{
#if FLT_TEXTURE_ENABLE_THREAD_LOAD
	pthread_mutex_init(&s_mutex, NULL);
#endif
}

TextureManager::~TextureManager()
{
#if FLT_TEXTURE_ENABLE_THREAD_LOAD
	pthread_mutex_destroy(&s_mutex);
#endif
}

TexturePtr TextureManager::createRes(const stringc& name, int flag)
{
	FLT_ASSERT(m_renderer);

	TexturePtr texture;

	if(flag==0)
	{
		FileStream file;
		ArchiveManager::getInstance().openFile(file, name);
		texture = m_renderer->createTexture(file.IsValid() ? &file : NULL);		
		texture->setName(name);
	}
	else
	{
		SImageHold* imgHold = new SImageHold();
		imgHold->Name = name;
		imgHold->pImage = m_threadLoadImagePointer;

		//work in a single thread-----------------------
#if FLT_TEXTURE_ENABLE_THREAD_LOAD
		// Create the thread using POSIX routines.
		pthread_attr_t  attr;
		pthread_t       posixThreadID;
		int             returnVal;
		
		returnVal = pthread_attr_init(&attr);
		assert(!returnVal);
		returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		assert(!returnVal);				

		int threadError = pthread_create(&posixThreadID, &attr, &backgroundLoadImage, (void*)(imgHold));

#if WIN32
		DBG("thread","main thread create thread %d to load image: %s", (int)posixThreadID.p, name.c_str());
#endif
		
		returnVal = pthread_attr_destroy(&attr);
		assert(!returnVal);
		if (threadError != 0)
		{
			DBG("thread","Error! create thread error: %d", threadError);
		}				

#else
		backgroundLoadImage(&imgHold);				
#endif
		//----------------------------------------------		
	}	

	DBG("engineDbg_Res","create texture name: %s valid: %d", name.c_str(), texture.isValid());

	return texture;
}

TexturePtr TextureManager::cloneRes(const TexturePtr& rhs, const stringc& newResName)
{
	FLT_ASSERT(m_renderer);

	TexturePtr newTexture = rhs;
	return newTexture;
}

void TextureManager::setRenderer(IRenderer *renderer)
{	
	m_renderer = renderer;
}

}// end namespace renderer
}// end namespace flt

