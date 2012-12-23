#include "renderer/opengl/OpenGLES1Renderer.h"
#include "renderer/Camera.h"

#ifdef FLT_COMPILE_WITH_OPENGLES1

#include "renderer/opengl/OpenGLConstsMap.h"
#include "utils/Logger.h"
#include "core/matrix4.h"
#include "renderer/material/Material.h"
#include "stream/FileStream.h"
#include "renderer/image/Image.h"
#include "renderer/opengl/OpenGLESTexture.h"
#include "renderer/MaterialFactory.h"
#include "archive/ArchiveManager.h"

namespace flt
{
namespace renderer
{

void OpenGLES1Renderer::initRenderer()
{	
	const GLubyte* glVersion = glGetString(GL_VERSION);
	const GLubyte* glRenderer = glGetString(GL_RENDERER);
	const GLubyte* glVendor = glGetString(GL_VENDOR);
	if (glVersion && glRenderer && glVendor)
	{
		LOG("engine","%s-%s-%s",glVersion, glRenderer, glVendor);		
	}

	GLint glMaxTexUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &glMaxTexUnits);
	m_maxTextureUnits=core::min_((u32)glMaxTexUnits, FLT_MAX_TEXTURE_UNITS);	
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Reset The Current Viewport
	glViewport(0, 0, m_screenSize.Width, m_screenSize.Height);	
	
	setRenderScreenMode(RSM_3D);				

	m_transformDirtyFlag = TDF_ALL;
	m_viewMatrix.makeIdentity();
	m_worldMatrix.makeIdentity();
	applyTransforms(true);

	setClearDepth(1.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	
	
	glColor4ub(255,255,255,255);
	
	m_useBatch = true;

	//reset to default states

	m_currentMaterial.reset();
	
	RenderStates defaultRenderStates;
	applyRenderStates(defaultRenderStates, true);

	TextureState defaultTextureState;
	for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		setActiveTextureUnit(i,true);
		enableTexture(false,true);
		applyTextureStates(defaultTextureState, true);		
	}
	
	setActiveTextureUnit(0);

	setClientActiveTextureUnit(0,true);

	m_curArrayStates = ArrayStates();
	applyArrayStates(true);
		
	m_curGlobalStates = GlobalStates();
	applyGlobalStates(true);

	//create default material
	{
		m_defaultMaterial = Material::createSimpleEmptyMaterial(false);
		m_defaultMaterial->setName("DefaultMaterial");
	}

	//create default 2d material
	{
		m_sharedMaterial2D = Material::createSimpleEmptyMaterial(true);
		m_sharedMaterial2D->setName("SharedMaterial2D");
		m_sharedMaterial2D->getRenderPass(0)->getRenderStates().CullMode = CULL_NONE;	//disable backface culling for 2d drawing
		
		set2DMaterialTextureWrapModeU(TEXW_CLAMP_TO_EDGE);
		set2DMaterialTextureWrapModeV(TEXW_CLAMP_TO_EDGE);
		set2DMaterialTextureMinFilter(TEXF_BILINEAR);
		set2DMaterialTextureMagFilter(TEXF_BILINEAR);

		m_sharedMaterial2DType = BIMT_2D_ALPHA_BLEND;

		MaterialFactory::getInstance().modifyMaterialByType(m_sharedMaterial2D, m_sharedMaterial2DType, true);	
	}

	//create 2d quard vertex array
	{
		m_vertexArray2DQuard.initForQuard2D(VAF_COLOR);
		m_vertexArray2DImage.initForQuard2D(VAF_COLOR|VAF_UV);

		m_pBatchBuffer2DImage = new BatchBuffer(BBT_2D, k_maxBatch2DImgQuardNum*4, this);
	}
	
}

void OpenGLES1Renderer::uninitRenderer()
{
	SAFE_DEL(m_pBatchBuffer2DImage);
}

void OpenGLES1Renderer::onSizeChange(u32 width, u32 height)
{
	StubRenderer::onSizeChange(width, height);

	glViewport(0, 0, m_screenSize.Width, m_screenSize.Height);	

	RENDER_SCREEN_MODE renderScreenMode = m_curRenderScreenMode;
	m_curRenderScreenMode = RSM_NOT_SET;
	setRenderScreenMode(renderScreenMode);
}

void OpenGLES1Renderer::beginRender()
{
	StubRenderer::beginRender();

	//update camera
	//TODO: refactor code to only update when dirty
	if(m_curRenderScreenMode==RSM_3D)
	{
		glMatrixMode(GL_PROJECTION);
		const core::matrix4 &mat = m_camera3D->getProjectionMatrix();
		glLoadMatrixf(mat.pointer());

		//handleOrientation(m_screenSize, m_orientation);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();				

		m_transformDirtyFlag = TDF_ALL;
		m_viewMatrix = m_camera3D->getViewMatrix();
		m_worldMatrix.makeIdentity();
	}
	
}


void OpenGLES1Renderer::endRender()
{
	flushBatch();
	glFlush();
	StubRenderer::endRender();

}

void OpenGLES1Renderer::begin2DMode()
{
	flushBatch();
	setRenderScreenMode(RSM_2D_SCREEN);
}

void OpenGLES1Renderer::end2DMode()
{
	//restore to 3d mode, and flush batch
	flushBatch();
	setRenderScreenMode(RSM_3D);
}

void OpenGLES1Renderer::clearBuffer(int bufferMask)
{
	GLbitfield mask = 0;
	if (bufferMask & FB_COLOR)
	{
		const f32 inv = 1.0f / 255.0f;
		glClearColor(m_clearColor.R * inv,
			m_clearColor.G * inv,
			m_clearColor.B * inv,
			m_clearColor.A * inv);
		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (bufferMask & FB_DEPTH)
	{
		glDepthMask(GL_TRUE);
		glClearDepthf(m_clearDepth);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	if (mask != 0)
	{
		glClear(mask);
	}
}	

void OpenGLES1Renderer::setViewTransform(const core::matrix4& viewTransform)
{
	m_viewMatrix = viewTransform;
	m_transformDirtyFlag |= TDF_VIEW;
}

void OpenGLES1Renderer::setWorldTransform(const core::matrix4& worldTransform)
{
	m_worldMatrix = worldTransform;
	m_transformDirtyFlag |= TDF_WORLD;
}

void OpenGLES1Renderer::applyTransforms(bool forceApply)
{
	if(forceApply || m_transformDirtyFlag & TDF_VIEW || m_transformDirtyFlag & TDF_WORLD)
	{
		glLoadMatrixf((m_viewMatrix * m_worldMatrix).pointer());

		m_transformDirtyFlag &= ~(TDF_VIEW | TDF_WORLD);
	}
}

void handleOrientation(const core::dimension2di& screenSize, EDeviceOrientation ori)
{
#ifdef FLT_DEVICE_IOS
	switch(ori)
	{
		case EDO_LandscapeLeft:
			{
				glTranslatef(screenSize.Width, 0, 0);
				glRotatef(90.0f, 0, 0, 1);
			}
			break;

		case EDO_LandscapeRight:
			{
				glTranslatef(0, screenSize.Height, 0);
				glRotatef(-90.0f, 0, 0, 1);
			}
			break;

		case EDO_PortraitUpsideDown:
			{
				glTranslatef(screenSize.Width, screenSize.Height, 0);
				glRotatef(180.0f, 0, 0, 1);
			}
			break;
	}
#endif
}

void OpenGLES1Renderer::setRenderScreenMode(RENDER_SCREEN_MODE mode)
{
	if(m_curRenderScreenMode!=mode)
	{		
		switch(mode)
		{
		case RSM_3D:
			{
				glMatrixMode(GL_PROJECTION);
				//core::matrix4 mat;
				//mat.buildProjectionMatrixPerspectiveFovOpenGL(60*core::DEGTORAD, (f32)m_screenSize.Width/m_screenSize.Height, 0, 10000);				

				const core::matrix4 &mat = m_camera3D->getProjectionMatrix();
				glLoadMatrixf(mat.pointer());

				handleOrientation(m_screenSize, m_orientation);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();				

				m_transformDirtyFlag = TDF_ALL;
				m_viewMatrix = m_camera3D->getViewMatrix();
				m_worldMatrix.makeIdentity();
			}
			break;

		case RSM_2D_SCREEN:
			{
				glMatrixMode(GL_PROJECTION);

				const core::dimension2di& renderTargetSize = m_screenSize;
				//core::matrix4 m;
				//m.buildProjectionMatrixOrthoOpenGL(f32(renderTargetSize.Width), f32(-renderTargetSize.Height), -1.0, 1.0);				
				//glLoadMatrixf(m.pointer());
				glLoadIdentity();
				glOrthof(0,(GLfloat)renderTargetSize.Width,(GLfloat)renderTargetSize.Height,0,-1.0f,1.0f);

				handleOrientation(m_screenSize, m_orientation);				

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.375, 0.375, 0.0);

				// Make sure we set first texture matrix
				setActiveTextureUnit(0);
				
				m_transformDirtyFlag = TDF_ALL;
				m_viewMatrix.makeIdentity();
				m_worldMatrix.makeIdentity();
			}
			break;

		default:
			break;
		}		

		m_curRenderScreenMode = mode;
	}
}

u32 OpenGLES1Renderer::getMaxTextureUnitNum() const
{
	return m_maxTextureUnits;
}

TexturePtr OpenGLES1Renderer::createTexture(const char* filePath)
{
	FileStream file;
	ArchiveManager::getInstance().openFile(file, filePath);
	//FileStream file(filePath);
	return createTexture(&file);
}

TexturePtr OpenGLES1Renderer::createTextureFromImage(Image* pImage)
{
	TexturePtr texture = new OpenGLESTexture(pImage);
	setActiveTextureUnit(0);
	setTexture(texture);

	bool holdImageByTexture = false;

	texture->create(holdImageByTexture);

	TextureState defaultTS;
	applyTextureStates(defaultTS,true);
	
	if(!holdImageByTexture)
		delete pImage;
	
	enableTexture(false);

	return texture;
}

TexturePtr OpenGLES1Renderer::createTexture(FileStream* file)
{	
	Image* img = createImageFromStream(file);	
	Image* pImage = 0;

	if(img != NULL && !img->isSizePowerOf2())
	{
		pImage = new Image(img,true);
		delete img;
	}
	else
	{
		pImage = img;
	}

	return createTextureFromImage(pImage);	
}

void OpenGLES1Renderer::enableLighting(bool enable)
{
	m_curGlobalStates.LightEnable = enable;
}

void OpenGLES1Renderer::setSceneAmbientLightColor(const Colorf& color)
{
	m_curGlobalStates.SceneAmbientLight = color;
}

void OpenGLES1Renderer::applyRenderPass(const RenderPass& pass)
{	
	applyRenderStates(pass.getRenderStates());
	
	u32 textureLayerNum = pass.getTextureLayerNum();
	for(u32 i=textureLayerNum; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		if(m_lastStates.m_textureEnable[i])
		{
			setActiveTextureUnit(i);
			enableTexture(false);
		}
	}
	for(u32 i=0; i<textureLayerNum; ++i)
	{
		const TextureLayer& textureLayer = pass.getTextureLayer(i);
		setActiveTextureUnit(i);
		setTexture(textureLayer.m_texture);
		applyTextureStates(textureLayer.m_texState);		
	}
}

//======= TextureState ===========================================================
void OpenGLES1Renderer::setActiveTextureUnit(u32 unit, bool forceApply)
{
	FLT_ASSERT(unit>=0 && unit<FLT_MAX_TEXTURE_UNITS);

	
	if(forceApply || m_lastStates.m_activeTexture!=unit)
	{
		m_lastStates.m_activeTexture = unit;

		GLenum texUnit = (GLenum)(GL_TEXTURE0 + unit);
		glActiveTexture(texUnit);
		
		DBG("engineDbg_RS","set active texture unit %d.",unit);		
	}
}

void OpenGLES1Renderer::setClientActiveTextureUnit(u32 unit, bool forceApply)
{
	FLT_ASSERT(unit>=0 && unit<FLT_MAX_TEXTURE_UNITS);
	
	if(forceApply || m_lastStates.m_clientActiveTexture!=unit)
	{
		m_lastStates.m_clientActiveTexture = unit;

		GLenum texUnit = (GLenum)(GL_TEXTURE0 + unit);
		glClientActiveTexture(texUnit);
		
		DBG("engineDbg_RS","set client active texture unit %d.",unit);
	}
}

void OpenGLES1Renderer::enableTexture(bool enable, bool forceApply)
{
	if(forceApply || m_lastStates.m_textureEnable[m_lastStates.m_activeTexture]!=enable)
	{
		m_lastStates.m_textureEnable[m_lastStates.m_activeTexture] = enable;						

		if(enable)
		{
			glEnable(GL_TEXTURE_2D);
			DBG("engineDbg_RS","enable texture for unit %d.",m_lastStates.m_activeTexture);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			DBG("engineDbg_RS","disable texture for unit %d.",m_lastStates.m_activeTexture);
		}
	}
}

bool OpenGLES1Renderer::setTexture(TexturePtr texture)
{
	if(!texture.isValid())
	{
		enableTexture(false);
	}
	else
	{
		//TODO: save in shadow state, avoid bind the same texture each frame
		enableTexture(true);
		OpenGLESTexture* pTexture = static_cast<OpenGLESTexture*>(texture.get());
		glBindTexture(GL_TEXTURE_2D, pTexture->getGLID());
		DBG("engineDbg_RS","bind texture for unit %d, glID=%d",m_lastStates.m_activeTexture,pTexture->getGLID());
	}

	return true;
}

void OpenGLES1Renderer::applyTextureStates(const TextureState& textureState, bool forceApply)
{		
	u32 unit = m_lastStates.m_activeTexture;
	TextureState& lastState = m_lastStates.m_textureStates[unit];

	#define STATE_CHANGE(state) if(forceApply || textureState.state!=lastState.state)
	#define SYNC_STATE(state) lastState.state = textureState.state

	STATE_CHANGE(WrapModeU)
	{
		SYNC_STATE(WrapModeU);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, k_glWrapMode[textureState.WrapModeU]);
		DBG("engineDbg_RS","set texture wrap mode U = %d, for unit %d",textureState.WrapModeU,unit);
	}

	STATE_CHANGE(WrapModeV)
	{
		SYNC_STATE(WrapModeV);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, k_glWrapMode[textureState.WrapModeV]);
		DBG("engineDbg_RS","set texture wrap mode V = %d, for unit %d",textureState.WrapModeV,unit);
	}

	STATE_CHANGE(MinFilterMode)
	{
		SYNC_STATE(MinFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, k_glFilterMode[textureState.MinFilterMode]);
		DBG("engineDbg_RS","set texture min filter mode %d, for unit %d",textureState.MinFilterMode,unit);
	}

	STATE_CHANGE(MagFilterMode)
	{
		SYNC_STATE(MagFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, k_glFilterMode[textureState.MagFilterMode]);
		DBG("engineDbg_RS","set texture mag filter mode %d, for unit %d",textureState.MagFilterMode,unit);
	}

	STATE_CHANGE(EnvMode)
	{
		SYNC_STATE(EnvMode);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, k_glTexEnvMode[textureState.EnvMode]);
		DBG("engineDbg_RS","set texture ENV mode %d, for unit %d",textureState.EnvMode,unit);
	}	

	if(forceApply || lastState.EnvMode==TEXENV_COMBINE)
	{
		STATE_CHANGE(CombineFuncRGB)
		{
			SYNC_STATE(CombineFuncRGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, k_glTexCombineFunc[textureState.CombineFuncRGB]);
			DBG("engineDbg_RS","set texture combine RGB func %d, for unit %d",textureState.CombineFuncRGB,unit);
		}

		STATE_CHANGE(CombineFuncAlpha)
		{
			SYNC_STATE(CombineFuncAlpha);
			FLT_DEBUG_BREAK_IF(textureState.CombineFuncAlpha==TEXCBF_DOT3_RGB || textureState.CombineFuncAlpha==TEXCBF_DOT3_RGBA);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, k_glTexCombineFunc[textureState.CombineFuncAlpha]);
			DBG("engineDbg_RS","set texture combine Alpha func %d, for unit %d",textureState.CombineFuncAlpha,unit);
		}

		STATE_CHANGE(CombineSrc0RGB)
		{
			SYNC_STATE(CombineSrc0RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, k_glTexCombineSrc[textureState.CombineSrc0RGB]);
			DBG("engineDbg_RS","set texture combine src0 rgb %d, for unit %d",textureState.CombineSrc0RGB,unit);
		}

		STATE_CHANGE(CombineSrc1RGB)
		{
			SYNC_STATE(CombineSrc1RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, k_glTexCombineSrc[textureState.CombineSrc1RGB]);
			DBG("engineDbg_RS","set texture combine src1 rgb %d, for unit %d",textureState.CombineSrc1RGB,unit);
		}

		STATE_CHANGE(CombineSrc2RGB)
		{
			SYNC_STATE(CombineSrc2RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, k_glTexCombineSrc[textureState.CombineSrc2RGB]);
			DBG("engineDbg_RS","set texture combine src2 rgb %d, for unit %d",textureState.CombineSrc2RGB,unit);
		}

		STATE_CHANGE(CombineSrc0Alpha)
		{
			SYNC_STATE(CombineSrc0Alpha);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, k_glTexCombineSrc[textureState.CombineSrc0Alpha]);
			DBG("engineDbg_RS","set texture combine src0 alpha %d, for unit %d",textureState.CombineSrc0Alpha,unit);
		}

		STATE_CHANGE(CombineSrc1Alpha)
		{
			SYNC_STATE(CombineSrc1Alpha);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, k_glTexCombineSrc[textureState.CombineSrc1Alpha]);
			DBG("engineDbg_RS","set texture combine src1 alpha %d, for unit %d",textureState.CombineSrc1Alpha,unit);
		}

		STATE_CHANGE(CombineSrc2Alpha)
		{
			SYNC_STATE(CombineSrc2Alpha);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_ALPHA, k_glTexCombineSrc[textureState.CombineSrc2Alpha]);
			DBG("engineDbg_RS","set texture combine src2 alpha %d, for unit %d",textureState.CombineSrc2Alpha,unit);
		}

		STATE_CHANGE(CombineOp0RGB)
		{
			SYNC_STATE(CombineOp0RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, k_glTexCombineOp[textureState.CombineOp0RGB]);
			DBG("engineDbg_RS","set texture combine op0 rgb %d, for unit %d",textureState.CombineOp0RGB,unit);
		}

		STATE_CHANGE(CombineOp1RGB)
		{
			SYNC_STATE(CombineOp1RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, k_glTexCombineOp[textureState.CombineOp1RGB]);
			DBG("engineDbg_RS","set texture combine op1 rgb %d, for unit %d",textureState.CombineOp1RGB,unit);
		}

		STATE_CHANGE(CombineOp2RGB)
		{
			SYNC_STATE(CombineOp2RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, k_glTexCombineOp[textureState.CombineOp2RGB]);
			DBG("engineDbg_RS","set texture combine op2 rgb %d, for unit %d",textureState.CombineOp2RGB,unit);
		}

		STATE_CHANGE(CombineOp0Alpha)
		{
			SYNC_STATE(CombineOp0Alpha);
			FLT_DEBUG_BREAK_IF(textureState.CombineOp0Alpha==TEXCBOP_SRC_COLOR || textureState.CombineOp0Alpha==TEXCBOP_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, k_glTexCombineOp[textureState.CombineOp0Alpha]);
			DBG("engineDbg_RS","set texture combine op0 alpha %d, for unit %d",textureState.CombineOp0Alpha,unit);
		}

		STATE_CHANGE(CombineOp1Alpha)
		{
			SYNC_STATE(CombineOp1Alpha);
			FLT_DEBUG_BREAK_IF(textureState.CombineOp1Alpha==TEXCBOP_SRC_COLOR || textureState.CombineOp1Alpha==TEXCBOP_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, k_glTexCombineOp[textureState.CombineOp1Alpha]);
			DBG("engineDbg_RS","set texture combine op1 alpha %d, for unit %d",textureState.CombineOp1Alpha,unit);
		}

		STATE_CHANGE(CombineOp2Alpha)
		{
			SYNC_STATE(CombineOp2Alpha);
			FLT_DEBUG_BREAK_IF(textureState.CombineOp2Alpha==TEXCBOP_SRC_COLOR || textureState.CombineOp2Alpha==TEXCBOP_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, k_glTexCombineOp[textureState.CombineOp2Alpha]);
			DBG("engineDbg_RS","set texture combine op2 alpha %d, for unit %d",textureState.CombineOp2Alpha,unit);
		}

		STATE_CHANGE(EnvBlendColor)
		{
			SYNC_STATE(EnvBlendColor);
			GLfloat constColor[4] = {textureState.EnvBlendColor.r, textureState.EnvBlendColor.g, textureState.EnvBlendColor.b, textureState.EnvBlendColor.a};
			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
			DBG("engineDbg_RS","set texture combine const color RGBA(%f,%f,%f,%f), for unit %d",constColor[0],constColor[1],constColor[2],constColor[3],unit);
		}

		STATE_CHANGE(CombineRGBScale)
		{
			SYNC_STATE(CombineRGBScale);
			FLT_DEBUG_BREAK_IF(textureState.CombineRGBScale!=1.0f && textureState.CombineRGBScale!=2.0f && textureState.CombineRGBScale!=4.0f);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, textureState.CombineRGBScale);
			DBG("engineDbg_RS","set texture rgb scale %f, for unit %d",textureState.CombineRGBScale,unit);
		}

		STATE_CHANGE(CombineAlphaScale)
		{
			SYNC_STATE(CombineAlphaScale);
			FLT_DEBUG_BREAK_IF(textureState.CombineAlphaScale!=1.0f && textureState.CombineAlphaScale!=2.0f && textureState.CombineAlphaScale!=4.0f);
			glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE, textureState.CombineAlphaScale);
			DBG("engineDbg_RS","set texture alpha scale %f, for unit %d",textureState.CombineAlphaScale,unit);
		}

	}//combine


	#undef STATE_CHANGE
	#undef SYNC_STATE
}


//========= apply RenderStates ==================================================

void OpenGLES1Renderer::applyRenderStates(const RenderStates& renderState, bool forceApply)
{

	#define STATE_CHANGE(state) if(forceApply || renderState.state!=m_lastStates.m_renderStates.state)
	#define STATE_CHANGE_OR( state1, state2 ) if(forceApply || renderState.state1!=m_lastStates.m_renderStates.state1 || renderState.state2!=m_lastStates.m_renderStates.state2)
	#define SYNC_STATE(state) m_lastStates.m_renderStates.state = renderState.state

	STATE_CHANGE(DepthTest)
	{
		SYNC_STATE(DepthTest);
		if(renderState.DepthTest)
		{			
			glEnable(GL_DEPTH_TEST);
			DBG("engineDbg_RS","enable depth test.");
		}
		else
		{			
			glDisable(GL_DEPTH_TEST);
			DBG("engineDbg_RS","disable depth test.");
		}
	}

	STATE_CHANGE(DepthWrite)
	{
		SYNC_STATE(DepthWrite);
		if(renderState.DepthWrite)
		{			
			glDepthMask(GL_TRUE);
			DBG("engineDbg_RS","enable depth write.");
		}
		else
		{			
			glDepthMask(GL_FALSE);
			DBG("engineDbg_RS","disable depth write.");
		}
	}

	STATE_CHANGE(DepthFunc)
	{
		SYNC_STATE(DepthFunc);		
		glDepthFunc(k_glCompareFuncs[renderState.DepthFunc]);
		DBG("engineDbg_RS", "set depth func %d", renderState.DepthFunc);
	}

	STATE_CHANGE(ColorBufferMask)
	{
		SYNC_STATE(ColorBufferMask);
		GLboolean writeR = (renderState.ColorBufferMask & CBM_RED) ? GL_TRUE : GL_FALSE;
		GLboolean writeG = (renderState.ColorBufferMask & CBM_GREEN) ? GL_TRUE : GL_FALSE;
		GLboolean writeB = (renderState.ColorBufferMask & CBM_BLUE) ? GL_TRUE : GL_FALSE;
		GLboolean writeA = (renderState.ColorBufferMask & CBM_ALPHA) ? GL_TRUE : GL_FALSE;
		glColorMask(writeR,writeG,writeB,writeA);
		DBG("engineDbg_RS", "set color buffer write r:%d,g:%d,b:%d,a:%d", writeR,writeG,writeB,writeA);
	}

	STATE_CHANGE(FrontFaceOrder)
	{
		SYNC_STATE(FrontFaceOrder);
		glFrontFace(k_glFrontFaceOrder[renderState.FrontFaceOrder]);
		DBG("engineDbg_RS", "set front face order %s", (renderState.FrontFaceOrder==FT_CCW)?"counter clock wise":"clock wise");
	}

	STATE_CHANGE(CullMode)
	{	
		SYNC_STATE(CullMode);
		if(renderState.CullMode==CULL_NONE)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{					
			glEnable(GL_CULL_FACE);
			glCullFace(k_glCullFaceMode[renderState.CullMode]);
		}		
		DBG("engineDbg_RS", "set cull face mode %d", renderState.CullMode);
	}

	STATE_CHANGE(WireFrameMode)
	{
		SYNC_STATE(WireFrameMode);
		//Note: glPolygonMode not supported in OpenGLES, should use draw lines instead when exec draw call
		DBG("engineDbg_RS", "set wire frame mode %d", renderState.WireFrameMode);
	}

	STATE_CHANGE(ShadeModel)
	{
		SYNC_STATE(ShadeModel);
		glShadeModel(k_glShadeModel[renderState.ShadeModel]);
		DBG("engineDbg_RS", "set shade model %d", renderState.ShadeModel);
	}

	//TODO: light material should optimize according to lighting and color material state
	STATE_CHANGE(Emissive)
	{
		SYNC_STATE(Emissive);
		GLfloat _emissive_color[] = {renderState.Emissive.r, renderState.Emissive.g, renderState.Emissive.b, renderState.Emissive.a};
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _emissive_color);
		DBG("engineDbg_RS", "set material Emissive (%f,%f,%f,%f)", renderState.Emissive.r, renderState.Emissive.g, renderState.Emissive.b, renderState.Emissive.a);
	}

	STATE_CHANGE(Ambient)
	{
		SYNC_STATE(Ambient);
		GLfloat _ambient_color[] = {renderState.Ambient.r, renderState.Ambient.g, renderState.Ambient.b, renderState.Ambient.a};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _ambient_color);
		DBG("engineDbg_RS", "set material Ambient (%f,%f,%f,%f)", renderState.Ambient.r, renderState.Ambient.g, renderState.Ambient.b, renderState.Ambient.a);
	}

	STATE_CHANGE(Diffuse)
	{
		SYNC_STATE(Diffuse);
		GLfloat _diffuse_color[] = {renderState.Diffuse.r, renderState.Diffuse.g, renderState.Diffuse.b, renderState.Diffuse.a};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _diffuse_color);
		DBG("engineDbg_RS", "set material Diffuse (%f,%f,%f,%f)", renderState.Diffuse.r, renderState.Diffuse.g, renderState.Diffuse.b, renderState.Diffuse.a);
	}

	STATE_CHANGE(Specular)
	{
		SYNC_STATE(Specular);
		GLfloat _specular_color[] = {renderState.Specular.r, renderState.Specular.g, renderState.Specular.b, renderState.Specular.a};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _specular_color);
		DBG("engineDbg_RS", "set material Specular (%f,%f,%f,%f)", renderState.Specular.r, renderState.Specular.g, renderState.Specular.b, renderState.Specular.a);
	}

	STATE_CHANGE(Shininess)
	{
		SYNC_STATE(Shininess);		
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, renderState.Shininess);
		DBG("engineDbg_RS", "set material Shininess %f", renderState.Shininess);
	}

	STATE_CHANGE(NormalModify)
	{
		SYNC_STATE(NormalModify);
		if(renderState.NormalModify==NORMAL_NO_MODIFY)
		{
			glDisable(GL_NORMALIZE);
			glDisable(GL_RESCALE_NORMAL);
		}
		else if(renderState.NormalModify==NORMAL_SCALE)
		{
			glEnable(GL_RESCALE_NORMAL);
		}
		else
		{
			glEnable(GL_NORMALIZE);
		}
		DBG("engineDbg_RS", "set Normal Modify %d", renderState.NormalModify);
	}

	STATE_CHANGE(ColorMaterialFlag)
	{
		SYNC_STATE(ColorMaterialFlag);
		//Note: for OpenGLES1.1,  color material if enabled, have ambient and diffuse material parameters track the current color.
		if(renderState.ColorMaterialFlag!=CMF_NONE)
		{
			glEnable(GL_COLOR_MATERIAL);
			DBG("engineDbg_RS", "enable color material, for OpenGLES1.1, ambient and diffuse trace color material.");
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
			DBG("engineDbg_RS", "disable color material");
		}
	}

	//Fog state

	STATE_CHANGE(FogEnable)
	{
		SYNC_STATE(FogEnable);
		if(renderState.FogEnable)
		{
			glEnable(GL_FOG);
			DBG("engineDbg_RS","enable fog.");
		}
		else
		{
			glDisable(GL_FOG);
			DBG("engineDbg_RS","disable fog.");
		}
	}

	if(forceApply || renderState.FogEnable)
	{
		STATE_CHANGE(FogMode)
		{
			SYNC_STATE(FogMode);
			glFogf(GL_FOG_MODE, k_glFogMode[renderState.FogMode]);
			DBG("engineDbg_RS","set fog mode %d", renderState.FogMode);
		}

		STATE_CHANGE(FogStart)
		{
			SYNC_STATE(FogStart);
			glFogf(GL_FOG_START, renderState.FogStart);
			DBG("engineDbg_RS","set fog start %f", renderState.FogStart);
		}

		STATE_CHANGE(FogEnd)
		{
			SYNC_STATE(FogEnd);
			glFogf(GL_FOG_END, renderState.FogEnd);
			DBG("engineDbg_RS","set fog end %f", renderState.FogEnd);
		}

		STATE_CHANGE(FogDensity)
		{
			SYNC_STATE(FogDensity);
			glFogf(GL_FOG_DENSITY, renderState.FogDensity);
			DBG("engineDbg_RS","set fog density %f", renderState.FogDensity);
		}

		STATE_CHANGE(FogColor)
		{
			SYNC_STATE(FogColor);
			GLfloat _color[] = { renderState.FogColor.r, renderState.FogColor.g, renderState.FogColor.b, renderState.FogColor.a };
			glFogfv(GL_FOG_DENSITY, _color);
			DBG("engineDbg_RS","set fog color (%f,%f,%f,%f)", renderState.FogColor.r, renderState.FogColor.g, renderState.FogColor.b, renderState.FogColor.a);
		}

	}//fog

	// alpha test
	STATE_CHANGE(AlphaTest)
	{
		SYNC_STATE(AlphaTest);
		if(renderState.AlphaTest)
		{
			glEnable(GL_ALPHA_TEST);
			DBG("engineDbg_RS","enable alpha test.");
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
			DBG("engineDbg_RS","disable alpha test.");
		}
	}

	if(forceApply || renderState.AlphaTest)
	{
		STATE_CHANGE_OR(AlphaFunc, AlphaTestRef)
		{
			SYNC_STATE(AlphaFunc);
			SYNC_STATE(AlphaTestRef);	
			glAlphaFunc(k_glCompareFuncs[renderState.AlphaFunc], renderState.AlphaTestRef);
			DBG("engineDbg_RS","set alpha test function %d, ref value %f.", renderState.AlphaFunc, renderState.AlphaTestRef);
		}
	}//alpha test

	// blend
	STATE_CHANGE(BlendMode)
	{
		SYNC_STATE(BlendMode);
		if(renderState.BlendMode==BLEND_DISABLE)
		{
			glDisable(GL_BLEND);
			DBG("engineDbg_RS","disable blend.");
		}
		else
		{
			glEnable(GL_BLEND);
			DBG("engineDbg_RS","enable blend.");
		}
	}

	if(forceApply || renderState.BlendMode!=BLEND_DISABLE)
	{
		STATE_CHANGE_OR(BlendSrcFactor, BlendDstFactor)
		{
			SYNC_STATE(BlendSrcFactor);
			SYNC_STATE(BlendDstFactor);
			glBlendFunc(k_glBlendFactor[renderState.BlendSrcFactor], k_glBlendFactor[renderState.BlendDstFactor]);
			DBG("engineDbg_RS","set blend srcFactor %d, dstFactor %d", renderState.BlendSrcFactor, renderState.BlendDstFactor);
		}
	}//blend

	STATE_CHANGE(LightingEnable)
	{
		SYNC_STATE(LightingEnable);
		if(renderState.LightingEnable)
		{
			glEnable(GL_LIGHTING);
			DBG("engineDbg_RS","enable lighting.");
		}
		else
		{
			glDisable(GL_LIGHTING);
			DBG("engineDbg_RS","disable lighting.");
		}
	}

	#undef STATE_CHANGE
	#undef STATE_CHANGE_OR
	#undef SYNC_STATE

}

//==================================================================================

void OpenGLES1Renderer::applyGlobalStates(bool forceApply)
{
	//------Global States---------------

	if(forceApply || m_curGlobalStates.LightEnable!=m_lastStates.m_globalStates.LightEnable)
	{
		m_lastStates.m_globalStates.LightEnable = m_curGlobalStates.LightEnable;
		if(m_curGlobalStates.LightEnable)
		{			
			glEnable(GL_LIGHTING);
			DBG("engineDbg_RS","enable light.");
		}
		else
		{			
			glDisable(GL_LIGHTING);
			DBG("engineDbg_RS","disable light.");
		}

		//TODO: flush batch
	}

	if(forceApply || m_curGlobalStates.SceneAmbientLight!=m_lastStates.m_globalStates.SceneAmbientLight)
	{
		m_lastStates.m_globalStates.SceneAmbientLight = m_curGlobalStates.SceneAmbientLight;
		GLfloat _light_color[] = { m_curGlobalStates.SceneAmbientLight.r,
									 m_curGlobalStates.SceneAmbientLight.g,
									 m_curGlobalStates.SceneAmbientLight.b, 1.0f};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, _light_color);
		
		//TODO: flush batch
	}		
}

void OpenGLES1Renderer::applyArrayStates(bool forceApply)
{	
	if(forceApply || m_curArrayStates.VertexArrayEnable!=m_lastStates.m_arrayStates.VertexArrayEnable)
	{
		m_lastStates.m_arrayStates.VertexArrayEnable = m_curArrayStates.VertexArrayEnable;
		if(m_curArrayStates.VertexArrayEnable)
		{			
			glEnableClientState(GL_VERTEX_ARRAY);
			DBG("engineDbg_RS","enable vertex array.");
		}
		else
		{			
			glDisableClientState(GL_VERTEX_ARRAY);
			DBG("engineDbg_RS","disable vertex array.");
		}
	}

	if(forceApply || m_curArrayStates.NormalArrayEnable!=m_lastStates.m_arrayStates.NormalArrayEnable)
	{
		m_lastStates.m_arrayStates.NormalArrayEnable = m_curArrayStates.NormalArrayEnable;
		if(m_curArrayStates.NormalArrayEnable)
			glEnableClientState(GL_NORMAL_ARRAY);
		else
			glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(forceApply || m_curArrayStates.ColorArrayEnable!=m_lastStates.m_arrayStates.ColorArrayEnable)
	{
		m_lastStates.m_arrayStates.ColorArrayEnable = m_curArrayStates.ColorArrayEnable;
		if(m_curArrayStates.ColorArrayEnable)
		{			
			glEnableClientState(GL_COLOR_ARRAY);
			DBG("engineDbg_RS","enable color array.");
		}
		else
		{			
			glDisableClientState(GL_COLOR_ARRAY);
			DBG("engineDbg_RS","disable color array.");
		}
	}

	for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		if(forceApply || m_curArrayStates.UVArrayEnable[i]!=m_lastStates.m_arrayStates.UVArrayEnable[i])
		{
			m_lastStates.m_arrayStates.UVArrayEnable[i] = m_curArrayStates.UVArrayEnable[i];

			setClientActiveTextureUnit(i);
			if(m_curArrayStates.UVArrayEnable[i])
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				DBG("engineDbg_RS","enable texture coordinate array of texture unit %d.",i);
			}
			else
			{			
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				DBG("engineDbg_RS","disable texture coordinate array of texture unit %d.",i);
			}
		}
	}
}

void OpenGLES1Renderer::setMaterial(const MaterialPtr& material)
{
	FLT_ASSERT(material.isValid());

	m_currentMaterial = material;
}

void OpenGLES1Renderer::flushBatch()
{
	if(m_curRenderScreenMode==RSM_2D_SCREEN)
	{
		m_pBatchBuffer2DImage->flush();
	}
}

void OpenGLES1Renderer::drawBatchBuffer(const BatchBuffer& batchBuffer)
{
	RenderPass* pass = batchBuffer.getRenderPass();

	applyRenderPass(*pass);
	drawPrimitivesInternal(batchBuffer.getVertexArray());		
}

void OpenGLES1Renderer::drawPrimitives(const VertexArray& vertexArray)
{
	if(!m_currentMaterial.isValid())
	{
		setMaterial(m_defaultMaterial);
	}
	
	bool batched = false;

	if(m_useBatch) 
	{
		//do 2d batching
		if(m_curRenderScreenMode==RSM_2D_SCREEN && m_pBatchBuffer2DImage->canBatch(vertexArray, m_currentMaterial))
		{
			m_pBatchBuffer2DImage->append(vertexArray, m_currentMaterial, core::IdentityMatrix);//Note: 2d drawing not use world transform
			batched = true;
		}
	}
	
	//Not batched, directly draw
	if(!batched)
	{		
		flushBatch();

		applyTransforms();

		u32 passCount = m_currentMaterial->getRenderPassCount();

		for(u32 i=0; i<passCount; ++i)
		{
			RenderPass* pass = m_currentMaterial->getRenderPass(i);
			applyRenderPass(*pass);

			drawPrimitivesInternal(vertexArray);
		}
	}
}

void OpenGLES1Renderer::drawPrimitivesInternal(const VertexArray& vertexArray)
{
	if(vertexArray.m_positions==0)
	{
		FLT_ASSERT(false);
		return;
	}	

	applyGlobalStates();

	//last render states has synced with current render states in applyRenderStates
	const RenderStates& renderState = m_lastStates.m_renderStates; 


	bool useNormal = (m_curGlobalStates.LightEnable && renderState.LightingEnable && vertexArray.m_normals!=0);
	bool useColor = (vertexArray.m_colors!=0);	

	/////////// setup array enables //////////////

	//position
	{
		FLT_ASSERT(vertexArray.m_posType==renderer::VCT_BYTE 
					|| vertexArray.m_posType==renderer::VCT_SHORT					
					|| vertexArray.m_posType==renderer::VCT_FLOAT);

		enableVertexArray(true);		
	}

	//normal
	if(useNormal)
	{
		FLT_ASSERT(vertexArray.m_normalType==renderer::VCT_BYTE 
					|| vertexArray.m_normalType==renderer::VCT_SHORT					
					|| vertexArray.m_normalType==renderer::VCT_FLOAT);

		enableNormalArray(true);		
	}

	//color
	if(useColor)
	{
		FLT_ASSERT(vertexArray.m_colorType==renderer::VCT_UNSIGNED_BYTE 					
					|| vertexArray.m_colorType==renderer::VCT_FLOAT);

		enableColorArray(true);		
	}

	//uv
	for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		if(vertexArray.m_texCoords[i])
		{
			enableUVArray(i,true);			
		}		
	}

	applyArrayStates();


	//////////////// set up arrays ////////////
	//position
	{		
		glVertexPointer(vertexArray.m_posCompCount, 
			k_glTypes[vertexArray.m_posType],
			vertexArray.m_posStride,
			vertexArray.m_positions);
	}

	//normal
	if(useNormal)
	{		
		glNormalPointer(k_glTypes[vertexArray.m_normalType],
			vertexArray.m_normalStride,
			vertexArray.m_normals);
	}

	//color
	if(useColor)
	{		
		glColorPointer(4, 
			k_glTypes[vertexArray.m_colorType], 
			vertexArray.m_colorStride,
			vertexArray.m_colors);
	}

	//uv
	for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		if(vertexArray.m_texCoords[i])
		{	
			setClientActiveTextureUnit(i);
			glTexCoordPointer(2, 
				GL_FLOAT,
				vertexArray.m_texCoordStride[i],
				vertexArray.m_texCoords[i]);
		}		
	}

	///////////////////////////////////

	int primitiveType = k_glPrimitiveTypes[vertexArray.m_primitiveType];
	
	if(vertexArray.m_indices)
	{		
		DBG("engineDbg1","drawPrimitives. vertex count=%d, index count=%d",vertexArray.m_vertexCount,vertexArray.m_indexCount);

		glDrawElements( primitiveType, 
						vertexArray.getRenderIndexCount(), 
						GL_UNSIGNED_SHORT, 
						vertexArray.m_indices);
	}
	else
	{
		glDrawArrays(primitiveType, 0, vertexArray.m_vertexCount);
	}
	

	enableColorArray(false);
	enableNormalArray(false);
	enableVertexArray(false);
	for(u32 i=0; i<FLT_MAX_TEXTURE_UNITS; ++i)
	{
		enableUVArray(i,false);
	}

	//Vertex attributes that are modified by glDrawElements have an unspecified value after glDrawElements returns
	glColor4ub(255,255,255,255);
}

void OpenGLES1Renderer::set2DMaterial(BUILT_IN_MATERIAL_TYPE materialType)
{
	MaterialFactory::getInstance().modifyMaterialByType(m_sharedMaterial2D, m_sharedMaterial2DType, false);
	m_sharedMaterial2DType = materialType;
	MaterialFactory::getInstance().modifyMaterialByType(m_sharedMaterial2D, m_sharedMaterial2DType, true);
}

void OpenGLES1Renderer::set2DMaterialTextureWrapModeU(TEX_WRAP_MODE wrapModeU)
{
	TextureState& texState0 = m_sharedMaterial2D->getRenderPass(0)->getTextureLayer(0).m_texState;
	texState0.WrapModeU = wrapModeU;	
}

void OpenGLES1Renderer::set2DMaterialTextureWrapModeV(TEX_WRAP_MODE wrapModeV)
{
	TextureState& texState0 = m_sharedMaterial2D->getRenderPass(0)->getTextureLayer(0).m_texState;	
	texState0.WrapModeV = wrapModeV;		
}

void OpenGLES1Renderer::set2DMaterialTextureMinFilter(TEX_FILTER_MODE minFilter)
{
	TextureState& texState0 = m_sharedMaterial2D->getRenderPass(0)->getTextureLayer(0).m_texState;		
	texState0.MinFilterMode = minFilter;		
}

void OpenGLES1Renderer::set2DMaterialTextureMagFilter(TEX_FILTER_MODE magFilter)
{
	TextureState& texState0 = m_sharedMaterial2D->getRenderPass(0)->getTextureLayer(0).m_texState;			
	texState0.MagFilterMode = magFilter;
}

void OpenGLES1Renderer::drawImage2D(TexturePtr texture, s32 x, s32 y, const core::rectf* const srcRect, const Color* const colorArray, core::matrix4* pMatrix)
{
	FLT_ASSERT(m_curRenderScreenMode==RSM_2D_SCREEN);

	FLT_ASSERT(texture.isValid());

	//compute rect

	f32 quardWidth, quardHeight;

	quardWidth = (f32)texture->getOriginalWidth();
	quardHeight = (f32)texture->getOriginalHeight();

	if(srcRect!=0)
	{
		quardWidth = (quardWidth > srcRect->getWidth()) ? srcRect->getWidth():quardWidth;
		quardHeight =(quardHeight > srcRect->getHeight()) ? srcRect->getHeight():quardHeight;
	}	

	core::rectf rect((f32)x, (f32)y, quardWidth, quardHeight);

	//compute uv

	f32 u0, v0, u1, v1;	

	if(srcRect!=0)
	{
		const f32 invW = 1.f / texture->getWidth();
		const f32 invH = 1.f / texture->getHeight();

		u0 = srcRect->UpperLeftCorner.X * invW;
		v0 = srcRect->UpperLeftCorner.Y * invH;
		u1 = srcRect->LowerRightCorner.X * invW;
		v1 = srcRect->LowerRightCorner.Y * invH;
		
		u0 = core::clamp(u0, 0.0f, 1.0f);
		v0 = core::clamp(v0, 0.0f, 1.0f);
		u1 = core::clamp(u1, 0.0f, 1.0f);
		v1 = core::clamp(v1, 0.0f, 1.0f);
	}
	else
	{
		u0 = v0 = 0.0f;

		if(texture->hasAligned())
		{			
			texture->getMaxUVOfAligned(u1,v1);		
		}
		else
		{
			u1 = v1 = 1.0f;		
		}	
	}

	//clip

	if(m_hasClip2D)
	{	
		f32 _uvs[] = { u0, v0, u1, v1 };

		if(colorArray==0)
		{
			if(clipQuard2D(m_clipRect2D, rect, _uvs, 0))
				return;
			
			m_vertexArray2DImage.setQuard2DColor(&ColorWhite, true);
		}
		else
		{
			Color _colors[] = { colorArray[0], colorArray[1], colorArray[2], colorArray[3] };
			if(clipQuard2D(m_clipRect2D, rect, _uvs, _colors))
				return;			

			m_vertexArray2DImage.setQuard2DColor(_colors, false);
		}		

		u0 = _uvs[0];
		v0 = _uvs[1];
		u1 = _uvs[2];
		v1 = _uvs[3];
	}
	else
	{		
		if(colorArray!=0)
		{
			m_vertexArray2DImage.setQuard2DColor(colorArray, false);
		}
		else
		{
			m_vertexArray2DImage.setQuard2DColor(&ColorWhite, true);
		}		
	}

	//Note: transform 2d image quard is not compatible with clip 2d now	

	m_vertexArray2DImage.setQuard2DPos(rect, pMatrix);

	m_vertexArray2DImage.setQuard2DUV(u0, v0, u1, v1);

	m_sharedMaterial2D->setTexture(0, 0, texture);

	setMaterial(m_sharedMaterial2D);

	drawPrimitives(m_vertexArray2DImage);

	m_sharedMaterial2D->setTexture(0, 0, 0);
}

void OpenGLES1Renderer::drawImage2D(TexturePtr texture, s32 x, s32 y, const Color* const colorArray, core::matrix4* pMatrix)
{
	drawImage2D(texture, x, y, 0, colorArray, pMatrix);
}

void OpenGLES1Renderer::drawRect2D(const core::rectf& rect, const Color& color)
{
	drawRect2D(rect, &color, true);	
}

void OpenGLES1Renderer::drawRect2D(const core::rectf& rect, const Color* const colorArray, bool singleColor)
{
	FLT_ASSERT(m_curRenderScreenMode==RSM_2D_SCREEN);	

	if(m_hasClip2D)
	{
		core::rectf clippedRect(rect);

		if(colorArray==0 || singleColor)
		{
			clippedRect.clipAgainst(m_clipRect2D);		
			if(!clippedRect.isValid() || clippedRect.isZero())
				return;
			
			if(colorArray!=0)
				m_vertexArray2DQuard.setQuard2DColor(colorArray, true);
		}
		else
		{
			Color _colors[] = { colorArray[0], colorArray[1], colorArray[2], colorArray[3] };
			if(clipQuard2D(m_clipRect2D, clippedRect, 0, _colors))
				return;			

			m_vertexArray2DQuard.setQuard2DColor(_colors, false);
		}

		m_vertexArray2DQuard.setQuard2DPos(clippedRect);
	}
	else
	{
		m_vertexArray2DQuard.setQuard2DPos(rect);
		
		if(colorArray!=0)
		{
			m_vertexArray2DQuard.setQuard2DColor(colorArray, singleColor);
		}//Note: If colorArray is 0, will use colors set last call, because of m_vertexArray2DQuard is shared.
	}			
	
	setMaterial(m_sharedMaterial2D);

	drawPrimitives(m_vertexArray2DQuard);
}

void OpenGLES1Renderer::drawBox2D(const core::rectf& rect, const Color& color)
{
	//TODO: Renderer should implement draw line

	core::rectf rect1;

	rect1.UpperLeftCorner = rect.UpperLeftCorner;
	rect1.LowerRightCorner.X = rect.UpperLeftCorner.X+1 ;
	rect1.LowerRightCorner.Y = rect.LowerRightCorner.Y;
	drawRect2D(rect1, color);

	rect1.UpperLeftCorner = rect.UpperLeftCorner;
	rect1.LowerRightCorner.X = rect.LowerRightCorner.X ;
	rect1.LowerRightCorner.Y = rect.UpperLeftCorner.Y+1;
	drawRect2D(rect1, color);

	rect1.LowerRightCorner = rect.LowerRightCorner;
	rect1.UpperLeftCorner.X = rect.LowerRightCorner.X-1 ;
	rect1.UpperLeftCorner.Y = rect.UpperLeftCorner.Y;
	drawRect2D(rect1, color);

	rect1.LowerRightCorner = rect.LowerRightCorner;
	rect1.UpperLeftCorner.X = rect.UpperLeftCorner.X;
	rect1.UpperLeftCorner.Y = rect.LowerRightCorner.Y-1;
	drawRect2D(rect1, color);
}

}// end namespace renderer
}// end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1
