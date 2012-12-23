#ifndef FLT_OPENGL_CONSTS_MAP_H
#define FLT_OPENGL_CONSTS_MAP_H

#include "config.h"

#ifdef FLT_COMPILE_WITH_OPENGLES1

#if defined(FLT_DEVICE_WIN32)
//#include "libs/win32/gles1/egl.h"
//#include "libs/win32/gles1/gl.h"
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/egl.h>
#endif

namespace flt
{	
namespace renderer
{	
	const int k_glTypes[] = 
	{
		GL_BYTE,			//VCT_BYTE=0,
		GL_UNSIGNED_BYTE,	//VCT_UNSIGNED_BYTE,
		GL_SHORT,			//VCT_SHORT,
		GL_UNSIGNED_SHORT,	//VCT_UNSIGNED_SHORT,
		GL_SHORT,			//VCT_INT,
		GL_UNSIGNED_SHORT,	//VCT_UNSIGNED_INT,
		GL_FLOAT,			//VCT_FLOAT	
	};

	const int k_glPrimitiveTypes[] =
	{
		GL_TRIANGLES,		//PT_TRIANGLES = 0,
		GL_TRIANGLE_STRIP,	//PT_TRIANGLE_STRIP,
		GL_LINES,			//PT_LINES,
		GL_LINE_STRIP,		//PT_LINE_STRIP,
        GL_LINE_LOOP,       //PT_LINE_LOOP,
		GL_POINTS,			//PT_POINTS,
	};

	const int k_glCompareFuncs[] =
	{
		GL_NEVER,			//CF_NEVER = 0,
		GL_ALWAYS,			//CF_ALWAYS,
		GL_EQUAL,			//CF_EQUAL,
		GL_NOTEQUAL,		//CF_NOTEQUAL,
		GL_LESS,			//CF_LESS,
		GL_LEQUAL,			//CF_LESS_EQUAL,
		GL_GREATER,			//CF_GREATER,
		GL_GEQUAL,			//CF_CREATER_EQUAL,	
	};

	const int k_glFrontFaceOrder[] =
	{
		GL_CCW,				//FT_CCW = 0,
		GL_CW,				//FT_CW
	};

	const int k_glCullFaceMode[] =
	{
		GL_BACK,			//CULL_BACK = 0,
		GL_FRONT,			//CULL_FRONT,
		GL_FRONT_AND_BACK  ,//CULL_FRONT_BACK,
	};

	const int k_glShadeModel[] =
	{
		GL_FLAT,			//SHADE_FLAT = 0,
		GL_SMOOTH,			//SHADE_SMOOTH,
	};

	const int k_glFogMode[] =
	{
		GL_LINEAR,			//FOG_LINEAR = 0,
		GL_EXP,				//FOG_EXP,
		GL_EXP2,			//FOG_EXPSQR
	};

	const int k_glBlendFactor[] =
	{
		GL_ZERO,				//BLD_ZERO = 0,
		GL_ONE,					//BLD_ONE,	
		GL_SRC_COLOR,			//BLD_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,	//BLD_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR,			//BLD_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,	//BLD_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,			//BLD_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,	//BLD_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,			//BLD_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,	//BLD_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE,	//BLD_SRC_ALPHA_SATURATE,

		////below not supported by OpenGLES1.1
		//BLD_CONSTANT_COLOR,
		//BLD_ONE_MINUS_CONSTANT_COLOR,
		//BLD_CONSTANT_ALPHA,
		//BLD_ONE_MINUS_CONSTANT_ALPHA,
	};

	const int k_glWrapMode[] =
	{
		GL_REPEAT,				//TEXW_REPEAT = 0,
		GL_CLAMP_TO_EDGE,		//TEXW_CLAMP_TO_EDGE,

		///below not supported by OpenGLES1.1, set to GL_CLAMP_TO_EDGE
		GL_CLAMP_TO_EDGE,		//TEXW_CLAMP,		
		GL_CLAMP_TO_EDGE,		//TEXW_CLAMP_TO_BORDER,
		GL_CLAMP_TO_EDGE,		//TEXW_MIRROR,
		GL_CLAMP_TO_EDGE,		//TEXW_MIRROR_CLAMP,
		GL_CLAMP_TO_EDGE,		//TEXW_MIRROR_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE,		//TEXW_MIRROR_CLAMP_TO_BORDER
	};

	const int k_glFilterMode[] =
	{
		GL_NEAREST,             //TEXF_NEAREST,
		GL_LINEAR,              //TEXF_BILINEAR,		
		GL_LINEAR_MIPMAP_LINEAR //TEXF_TRILINEAR,	//for mipmap only,need mipmaps
	};

	const int k_glTexEnvMode[] =
	{	
		GL_REPLACE,				//TEXENV_REPLACE = 0,
		GL_DECAL,				//TEXENV_DECAL,
		GL_MODULATE,			//TEXENV_MODULATE,
		GL_BLEND,				//TEXENV_BLEND,
		GL_ADD,					//TEXENV_ADD,
		GL_COMBINE,				//TEXENV_COMBINE,
	};

	const int k_glTexCombineFunc[] =
	{
		GL_REPLACE,				//TEXCBF_REPLACE = 0,
		GL_MODULATE,			//TEXCBF_MODULATE,
		GL_ADD,					//TEXCBF_ADD,
		GL_ADD_SIGNED,			//TEXCBF_ADD_SIGNED,
		GL_INTERPOLATE,			//TEXCBF_INTERPOLATE,
		GL_SUBTRACT,			//TEXCBF_SUBTRACT,
		GL_DOT3_RGB,			//TEXCBF_DOT3_RGB,
		GL_DOT3_RGBA,			//TEXCBF_DOT3_RGBA,
	};

	const int k_glTexCombineSrc[] =
	{
		GL_TEXTURE,				//TEXCBS_TEXTURE = 0,
		GL_CONSTANT,			//TEXCBS_CONSTANT,
		GL_PRIMARY_COLOR,		//TEXCBS_PRIMARY_COLOR,
		GL_PREVIOUS,			//TEXCBS_PREVIOUS,
	};

	const int k_glTexCombineOp[] =
	{
		GL_SRC_COLOR,			//TEXCBOP_SRC_COLOR = 0,
		GL_ONE_MINUS_SRC_COLOR,	//TEXCBOP_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,			//TEXCBOP_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,	//TEXCBOP_ONE_MINUS_SRC_ALPHA,
	};

} //end namespace renderer
} //end namespace flt

#endif //FLT_COMPILE_WITH_OPENGLES1

#endif //FLT_OPENGL_CONSTS_MAP_H