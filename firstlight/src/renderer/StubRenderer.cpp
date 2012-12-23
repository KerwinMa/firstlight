#include "renderer/StubRenderer.h"
#include "renderer/Camera.h"
#include "image/image.h"
#include "image/IImageLoader.h"
#include "image/TGAImageLoader.h"
//#include "image/JPGImageLoader.h"
#include "image/PNGImageLoader.h"
#include "stream/FileStream.h"
#include "utils/Logger.h"
#include "renderer/material/TextureMgr.h"
#include "renderer/MaterialFactory.h"

namespace flt
{
namespace renderer
{

StubRenderer::StubRenderer(const core::dimension2di& screenSize)
	:m_clearColor(0,0,0,0),
	 m_clearDepth(0.0f),
	 m_screenSize(screenSize),
	 m_viewPort(0,0,screenSize.Width,screenSize.Height),
	 m_curRenderScreenMode(RSM_NOT_SET),
	 m_clipRect2D(0,0,0,0),
	 m_hasClip2D(false),
	 m_orientation(EDO_Portrait)
{

	m_camera3D = new Camera();
	m_camera3D->setAspectRatio((float)screenSize.Width/screenSize.Height);

	m_imageLoaders.clear();
	m_imageLoaders.push_back(new TGAImageLoader());
	//m_imageLoaders.push_back(new JPGImageLoader());
	m_imageLoaders.push_back(new PNGImageLoader());

	TextureManager::getInstance().setRenderer(this);
}

StubRenderer::~StubRenderer()
{
	m_imageLoaders.clear();
}

void StubRenderer::onSizeChange(u32 width, u32 height)
{
	m_screenSize.Width = width;
	m_screenSize.Height = height;
}

bool StubRenderer::makeCurrent()
{
	return true;
}

void StubRenderer::beginRender()
{
	
}


void StubRenderer::endRender()
{
	
}

void StubRenderer::setCamera(const CameraPtr& camera)
{
	m_camera3D = camera;
}

CameraPtr StubRenderer::getCamera()
{
	return m_camera3D;
}

void StubRenderer::clearBuffer(int bufferMask)
{
	
}

bool StubRenderer::swapBuffer()
{
	return true;
}

void StubRenderer::setClearColor(const Color& color)
{
	m_clearColor = color;
}

Color StubRenderer::getClearColor() const
{
	return m_clearColor;
}

void StubRenderer::setClearDepth(f32 depth)
{
	m_clearDepth = depth;
}

float StubRenderer::getClearDepth() const
{
	return m_clearDepth;
}

u32 StubRenderer::getMaxTextureUnitNum() const
{
	return 0;
}

Image* StubRenderer::createImageFromFile(const char* filePath)
{
	FileStream file;
	if(file.Open(filePath))
	{
		Image* img = createImageFromStream(&file);
		if(img==0)		
		{
			DBG("engineDbg_Res","can't load image file(file is bad): %s",filePath);
		}

		return img;
	}
	else
	{
		DBG("engineDbg_Res","can't open image file(file not found): %s",filePath);
		return 0;
	}	
}

Image* StubRenderer::createImageFromStream(FileStream* file)
{
	if (!file)
		return 0;

	Image* image = 0;

	u32 i;

	// try to load file based on file extension
	for (i=0; i<m_imageLoaders.size(); ++i)
	{
		if (m_imageLoaders[i]->isFileExtensionSupport(file->GetFileName().c_str()))
		{
			// reset file position which might have changed due to previous loadImage calls
			file->Seek(0, io::beg);
			image = m_imageLoaders[i]->loadImage(file);
			if (image)
			{
				DBG("engineDbg_Res","load image file: %s", file->GetFileName().c_str());
				return image;
			}
		}
	}

	// try to load file based on what is in it
	for (i=0; i<m_imageLoaders.size(); ++i)
	{
		// dito
		file->Seek(0, io::beg);
		if (m_imageLoaders[i]->isFileDataSupport(file))
		{
			file->Seek(0, io::beg);
			image = m_imageLoaders[i]->loadImage(file);
			if (image)
			{
				DBG("engineDbg_Res","load image file: %s", file->GetFileName().c_str());		
				return image;
			}
		}
	}

	return 0; // failed to load
}

void StubRenderer::setClip2D(const core::rectf& clipRect)
{
	m_clipRect2D = clipRect;
	m_hasClip2D = true;
}

void StubRenderer::resetClip2D()
{
	m_hasClip2D = false;
}

bool StubRenderer::clipQuard2D(const core::rectf& clipRect, core::rectf& quardRect, f32* uvArray, Color* colorArray)
{
	if(quardRect.UpperLeftCorner.X>clipRect.LowerRightCorner.X ||
		quardRect.UpperLeftCorner.Y>clipRect.LowerRightCorner.Y ||
		quardRect.LowerRightCorner.X<clipRect.UpperLeftCorner.X ||
		quardRect.LowerRightCorner.Y<clipRect.UpperLeftCorner.Y)
	{
		return true;
	}

	//clip left
	if(clipRect.UpperLeftCorner.X > quardRect.UpperLeftCorner.X)
	{
		f32 percent = (clipRect.UpperLeftCorner.X - quardRect.UpperLeftCorner.X)/quardRect.getWidth();

		if(uvArray)
		{
			f32& u0 = uvArray[0];
			f32& u1 = uvArray[2];

			u0 = u0 + percent*(u1-u0);
		}

		if(colorArray)
		{
			colorArray[0] = colorArray[0].getInterpolated(colorArray[2], percent);
			colorArray[1] = colorArray[1].getInterpolated(colorArray[3], percent);
		}

		quardRect.UpperLeftCorner.X = clipRect.UpperLeftCorner.X;
	}

	//clip top
	if(clipRect.UpperLeftCorner.Y > quardRect.UpperLeftCorner.Y)
	{
		f32 percent = (clipRect.UpperLeftCorner.Y - quardRect.UpperLeftCorner.Y)/quardRect.getHeight();

		if(uvArray)
		{
			f32& v0 = uvArray[1];
			f32& v1 = uvArray[3];

			v0 = v0 + percent*(v1-v0);
		}

		if(colorArray)
		{
			colorArray[0] = colorArray[0].getInterpolated(colorArray[1], percent);
			colorArray[2] = colorArray[2].getInterpolated(colorArray[3], percent);
		}

		quardRect.UpperLeftCorner.Y = clipRect.UpperLeftCorner.Y;
	}

	//clip right
	if(clipRect.LowerRightCorner.X < quardRect.LowerRightCorner.X)
	{
		f32 percent = 1.0f - (quardRect.LowerRightCorner.X - clipRect.LowerRightCorner.X)/quardRect.getWidth();
		
		if(uvArray)
		{
			f32& u0 = uvArray[0];
			f32& u1 = uvArray[2];

			u1 = u0 + percent*(u1-u0);
		}

		if(colorArray)
		{
			colorArray[2] = colorArray[0].getInterpolated(colorArray[2], percent);
			colorArray[3] = colorArray[1].getInterpolated(colorArray[3], percent);
		}

		quardRect.LowerRightCorner.X = clipRect.LowerRightCorner.X;
	}

	//clip bottom
	if(clipRect.LowerRightCorner.Y < quardRect.LowerRightCorner.Y)
	{
		f32 percent = 1.0f - (quardRect.LowerRightCorner.Y - clipRect.LowerRightCorner.Y)/quardRect.getHeight();
		
		if(uvArray)
		{
			f32& v0 = uvArray[1];
			f32& v1 = uvArray[3];

			v1 = v0 + percent*(v1-v0);
		}

		if(colorArray)
		{
			colorArray[1] = colorArray[0].getInterpolated(colorArray[1], percent);
			colorArray[3] = colorArray[2].getInterpolated(colorArray[3], percent);
		}

		quardRect.LowerRightCorner.Y = clipRect.LowerRightCorner.Y;
	}
	
	return false;
}

void StubRenderer::setOrientation(EDeviceOrientation newOrientation)
{	
	m_orientation = newOrientation;
}

EDeviceOrientation StubRenderer::getOrientation() const
{
	return m_orientation;
}

}// end namespace renderer
}// end namespace flt