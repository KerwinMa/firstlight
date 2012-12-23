#ifndef FLT_RENDERER_DEFS_H
#define FLT_RENDERER_DEFS_H

#include "common.h"

namespace flt
{	
namespace renderer
{

enum RENDERER_TYPE
{						
	RENDERER_OPENGLES1,						
};

enum FRAME_BUFFER_TYPE
{
	FB_COLOR = 1,

	FB_DEPTH = FB_COLOR<<1,

	FB_STENCIL = FB_DEPTH<<1
};

enum RENDER_SCREEN_MODE
{
	RSM_NOT_SET,
	RSM_3D,
	RSM_2D_SCREEN,
	//RSM_2D_CARTESIAN,
};

enum TRANSFORM_MODE
{
	TM_PROJECTION = 0,
	TM_VIEW,
	TM_WORLD,
	TM_TEX0,
	TM_TEX1,
	TM_TEX2,
	TM_TEX3,
	TM_COUNT
};

//for rendering work
enum RENDER_STEP
{
	RS_SOLID_STEP = 0,
	RS_ALPHA_TEST_STEP,
	RS_TRANSPARENT_STEP,
	RS_COUNT
};

//for render states

//Maximum number of texture units engine can use (usually larger than renderer support max tex units, used of define some array)
#ifdef FLT_COMPILE_WITH_OPENGLES1
	const u32 FLT_MAX_TEXTURE_UNITS = 2;
#else
	const u32 FLT_MAX_TEXTURE_UNITS = 4;
#endif

enum COMPARISON_FUNC
{
	CF_NEVER = 0,
	CF_ALWAYS,
	CF_EQUAL,
	CF_NOTEQUAL,
	CF_LESS,
	CF_LESS_EQUAL,
	CF_GREATER,
	CF_CREATER_EQUAL,	
};

enum COLOR_BUFFER_MASK
{
	CBM_NONE = 0,
	CBM_ALPHA = 1,
	CBM_RED = 2,
	CBM_GREEN = 4,
	CBM_BLUE = 8,
	CBM_RGB = 14,
	CBM_ALL = 15
};

enum FRONT_FACE_ORDER
{
	FT_CCW = 0,
	FT_CW
};

enum CULLING_MODE
{	
	CULL_BACK = 0,
	CULL_FRONT,
	CULL_FRONT_BACK,
	CULL_NONE,
};

enum SHADE_MODEL
{
	SHADE_FLAT = 0,
	SHADE_SMOOTH,
};

enum NORMAL_MODIFY
{
	NORMAL_NO_MODIFY,
	NORMAL_SCALE,
	NORMAL_NORMALIZE
};

enum COLOR_MATERIAL_FLAG
{
	CMF_NONE = 0,	
	
	//for OpenGLES1.1 if enabled, both diffuse and ambient trace

	CMF_AMBIENT = 1,
	CMF_DIFFUSE = 2,
	CMF_SPECULAR = 4,
	CMF_EMISSIVE = 8,
	CMF_DIFFUSE_AND_AMBIENT = 3 
};

enum FOG_MODE
{
	FOG_LINEAR = 0,
	FOG_EXP,
	FOG_EXPSQR
};

// Blend

enum BLEND_MODE_MASK
{
	BLEND_DISABLE = 0,			//disable blend
	BLEND_ENABLE = 1,

	//below not supported by OpenGLES1.1
	BLEND_FACTOR_RGBA = 1,		//RGB & Alpha use the same factor
	BLEND_FACTOR_RGB_ALPHA = 2,	//RGB & Alpha use separate factors
	BLEND_OP_RGBA = 4,			//RGB & Alpha use the same operation
	BLEND_OP_RGB_ALPHA = 8,		//RGB & Alpha use separate operations
};

//blend op not supported by OpenGLES1.1 (only use ADD, and can't be changed)
enum BLEND_OP
{
	BLDOP_ADD,
	BLDOP_SUBTRACT,
	BLDOP_REVERSE_SUBTRACT,
	BLDOP_MIN,
	BLDOP_MAX
};

enum BLEND_FACTOR
{
	BLD_ZERO = 0,
	BLD_ONE,	
	BLD_SRC_COLOR,
	BLD_ONE_MINUS_SRC_COLOR,
	BLD_DST_COLOR,
	BLD_ONE_MINUS_DST_COLOR,
	BLD_SRC_ALPHA,
	BLD_ONE_MINUS_SRC_ALPHA,
	BLD_DST_ALPHA,
	BLD_ONE_MINUS_DST_ALPHA,
	BLD_SRC_ALPHA_SATURATE,

	//below not supported by OpenGLES1.1
	BLD_CONSTANT_COLOR,
	BLD_ONE_MINUS_CONSTANT_COLOR,
	BLD_CONSTANT_ALPHA,
	BLD_ONE_MINUS_CONSTANT_ALPHA,
};

// for texture unit states

enum TEX_WRAP_MODE
{
	TEXW_REPEAT = 0,
	TEXW_CLAMP_TO_EDGE,

	//below not supported by OpenGLES1.1
	TEXW_CLAMP,
	TEXW_CLAMP_TO_BORDER,
	TEXW_MIRROR,
	TEXW_MIRROR_CLAMP,
	TEXW_MIRROR_CLAMP_TO_EDGE,
	TEXW_MIRROR_CLAMP_TO_BORDER
};

enum TEX_FILTER_MODE
{
	TEXF_NEAREST = 0,
	TEXF_BILINEAR,
	TEXF_TRILINEAR,	//for mipmap only,need mipmaps
};

enum TEX_ENV_MODE
{
	TEXENV_REPLACE = 0,
	TEXENV_DECAL,
	TEXENV_MODULATE,
	TEXENV_BLEND,
	TEXENV_ADD,
	TEXENV_COMBINE,
};

enum TEX_COMBINE_FUNC
{
	TEXCBF_REPLACE = 0,
	TEXCBF_MODULATE,
	TEXCBF_ADD,
	TEXCBF_ADD_SIGNED,
	TEXCBF_INTERPOLATE,
	TEXCBF_SUBTRACT,
	TEXCBF_DOT3_RGB,
	TEXCBF_DOT3_RGBA,
};

enum TEX_COMBINE_SOURCE
{
	TEXCBS_TEXTURE = 0,
	TEXCBS_CONSTANT,
	TEXCBS_PRIMARY_COLOR,
	TEXCBS_PREVIOUS,
};

enum TEX_COMBINE_OP
{
	TEXCBOP_SRC_COLOR = 0,			//RGB only
	TEXCBOP_ONE_MINUS_SRC_COLOR,	//RGB only
	TEXCBOP_SRC_ALPHA,
	TEXCBOP_ONE_MINUS_SRC_ALPHA,
};
		

//built in material types, set by MaterialFactory
enum BUILT_IN_MATERIAL_TYPE
{
	BIMT_2D_SOLID = 0,
	BIMT_2D_ALPHA_BLEND,
	BIMT_2D_ADDITIVE,	
	BIMT_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR,	
	BIMT_2D_TEXTURE_ADDITIVE_MODULATE_COLOR,
	BIMT_2D_TEXTURE_TINT, //tint color with texture
	
	BIMT_COUNT
};

enum BATCH_BUFFER_TYPE
{
	BBT_2D = 1,
	BBT_3D,
	BBT_3D_NORMAL,
};

} //end namespace renderer
} //end namespace flt

#endif //FLT_RENDERER_DEFS_H


