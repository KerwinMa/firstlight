#include "RenderStates.h"

namespace flt
{
namespace renderer
{

RenderStates::RenderStates()
	:DepthTest(true),
	 DepthWrite(true),
	 DepthFunc(CF_LESS_EQUAL),
	 ColorBufferMask(CBM_ALL),
	 FrontFaceOrder(FT_CCW),
	 CullMode(CULL_BACK),
	 WireFrameMode(false),
	 ShadeModel(SHADE_SMOOTH),
	 Emissive(0.0f,0.0f,0.0f),
	 Ambient(1.0f,1.0f,1.0f),
	 Diffuse(1.0f,1.0f,1.0f),
	 Specular(1.0f,1.0f,1.0f),
	 Shininess(0.0f),
	 NormalModify(NORMAL_NO_MODIFY),
	 ColorMaterialFlag(CMF_NONE),
	 FogEnable(false),
	 FogMode(FOG_LINEAR),
	 FogStart(0.0f),
	 FogEnd(1.0f),
	 FogDensity(1.0f),
	 FogColor(0.0f,0.0f,0.0f),
	 AlphaTest(false),
	 AlphaFunc(CF_ALWAYS),
	 AlphaTestRef(0.5f),
	 BlendMode(BLEND_DISABLE),
	 //BlendOperation(BLDOP_ADD),
	 //BlendOperationAlpha(BLDOP_ADD),
	 BlendSrcFactor(BLD_ONE),
	 BlendDstFactor(BLD_ZERO),
	 //BlendSrcFactorAlpha(BLD_SRC_ALPHA),
	 //BlendDstFactorAlpha(BLD_SRC_ALPHA),
	 LightingEnable(false)	 
{
}

RenderStates& RenderStates::operator=(const RenderStates& rhs)
{
	if(this==&rhs)
		return *this;

	DepthTest = rhs.DepthTest;
	DepthWrite = rhs.DepthWrite;
	DepthFunc = rhs.DepthFunc;
	ColorBufferMask = rhs.ColorBufferMask;
	FrontFaceOrder = rhs.FrontFaceOrder;
	CullMode = rhs.CullMode;
	WireFrameMode = rhs.WireFrameMode;
	ShadeModel = rhs.ShadeModel;
	Emissive = rhs.Emissive;
	Ambient = rhs.Ambient;
	Diffuse = rhs.Diffuse;
	Specular = rhs.Specular;
	Shininess = rhs.Shininess;
	NormalModify = rhs.NormalModify;
	ColorMaterialFlag = rhs.ColorMaterialFlag;
	FogEnable = rhs.FogEnable;
	FogMode = rhs.FogMode;
	FogStart = rhs.FogStart;
	FogEnd = rhs.FogEnd;
	FogDensity = rhs.FogDensity;
	FogColor = rhs.FogColor;
	AlphaTest = rhs.AlphaTest;
	AlphaFunc = rhs.AlphaFunc;
	AlphaTestRef = rhs.AlphaTestRef;
	BlendMode = rhs.BlendMode;
	//BlendOperation = rhs.BlendOperation;
	//BlendOperationAlpha = rhs.BlendOperationAlpha;
	BlendSrcFactor = rhs.BlendSrcFactor;
	BlendDstFactor = rhs.BlendDstFactor;
	//BlendSrcFactorAlpha = rhs.BlendSrcFactorAlpha;
	//BlendDstFactorAlpha = rhs.BlendDstFactorAlpha;
	LightingEnable = rhs.LightingEnable;	

	return *this;
}

bool RenderStates::operator!=(const RenderStates& rhs) const
{
	if(this==&rhs)
		return false;

	return DepthTest != rhs.DepthTest ||
		DepthWrite != rhs.DepthWrite ||
		DepthFunc != rhs.DepthFunc ||
		ColorBufferMask != rhs.ColorBufferMask ||
		FrontFaceOrder != rhs.FrontFaceOrder ||
		CullMode != rhs.CullMode ||
		WireFrameMode != rhs.WireFrameMode ||
		ShadeModel != rhs.ShadeModel ||
		Emissive != rhs.Emissive ||
		Ambient != rhs.Ambient ||
		Diffuse != rhs.Diffuse ||
		Specular != rhs.Specular ||
		Shininess != rhs.Shininess ||
		NormalModify != rhs.NormalModify ||
		ColorMaterialFlag != rhs.ColorMaterialFlag ||
		FogEnable != rhs.FogEnable ||
		FogMode != rhs.FogMode ||
		FogStart != rhs.FogStart ||
		FogEnd != rhs.FogEnd ||
		FogDensity != rhs.FogDensity ||
		FogColor != rhs.FogColor ||
		AlphaTest != rhs.AlphaTest ||
		AlphaFunc != rhs.AlphaFunc ||
		AlphaTestRef != rhs.AlphaTestRef ||
		BlendMode != rhs.BlendMode ||	
		BlendSrcFactor != rhs.BlendSrcFactor ||
		BlendDstFactor != rhs.BlendDstFactor ||
		LightingEnable != rhs.LightingEnable;
}

///////////////////////////////////////////////////////

GlobalStates::GlobalStates()
	:LightEnable(false),
	 SceneAmbientLight(0.2f,0.2f,0.2f)
{
}

///////////////////////////////////////////////////////


}// end namespace renderer
}// end namespace flt