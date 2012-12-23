#ifndef FLT_RENDER_STATES_H
#define FLT_RENDER_STATES_H

#include "renderer/RendererDefs.h"
#include "renderer/Color.h"

namespace flt
{	
namespace renderer
{	
	// Render states set in material

	class RenderStates
	{
	public:		
		RenderStates();
		~RenderStates(){};

		RenderStates& operator=(const RenderStates& rhs);

		bool operator!=(const RenderStates& rhs) const;

	public:

		// Depth buffer states
		bool DepthTest;
		bool DepthWrite;
		COMPARISON_FUNC DepthFunc;

		// Color buffer states
		COLOR_BUFFER_MASK ColorBufferMask;

		// Face culling
		FRONT_FACE_ORDER FrontFaceOrder;
		CULLING_MODE CullMode;

		// Wire frame
		bool WireFrameMode;

		// Shade mode
		SHADE_MODEL ShadeModel;

		// Light material
		Colorf Emissive;
		Colorf Ambient;
		Colorf Diffuse;
		Colorf Specular;
		f32 Shininess;

		// Normal
		NORMAL_MODIFY NormalModify;

		// Vertex color as material flag
		COLOR_MATERIAL_FLAG ColorMaterialFlag;

		// Fog
		bool FogEnable;
		FOG_MODE FogMode;
		f32 FogStart;
		f32 FogEnd;
		f32 FogDensity;
		Colorf FogColor;

		// Alpha Test
		bool AlphaTest;
		COMPARISON_FUNC AlphaFunc;
		f32 AlphaTestRef;

		// Blend
		BLEND_MODE_MASK BlendMode;
		//BLEND_OP BlendOperation;		//not support by gles1.1
		//BLEND_OP BlendOperationAlpha;	//not support by gles1.1
		BLEND_FACTOR BlendSrcFactor;	
		BLEND_FACTOR BlendDstFactor;
		//BLEND_FACTOR BlendSrcFactorAlpha;	//not support by gles1.1
		//BLEND_FACTOR BlendDstFactorAlpha;	//not support by gles1.1

		// Lighting
		bool LightingEnable;
	};

	// Render states only can set by IRenderer
	// change it will break batching
	// accessed by IRenderer

	class GlobalStates
	{
	public:		
		GlobalStates();
		~GlobalStates(){};
	public:
		bool LightEnable;
		Colorf SceneAmbientLight;

		

		//line smooth
		//point smooth
		//point sprite
		//polygon offset
		//scissor test
	};	

	
} //end namespace renderer
} //end namespace flt

#endif //FLT_RENDER_STATES_H