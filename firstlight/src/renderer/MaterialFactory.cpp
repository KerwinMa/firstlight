#include "renderer/MaterialFactory.h"

namespace flt
{
namespace renderer
{

MaterialFactory::MaterialFactory()
{
	addModifiers();
}

MaterialFactory::~MaterialFactory()	
{
	deleteModifiers();
}

void MaterialFactory::addModifiers()
{
	IMaterialModifierPtr modifier;

	modifier = new MaterialModifier_2D_SOLID();
	addModifier(modifier);

	modifier = new MaterialModifier_2D_ALPHA_BLEND();
	addModifier(modifier);

	modifier = new MaterialModifier_2D_ADDITIVE();
	addModifier(modifier);	

	modifier = new MaterialModifier_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR();
	addModifier(modifier);

	modifier = new MaterialModifier_2D_TEXTURE_ADDITIVE_MODULATE_COLOR();
	addModifier(modifier);
}

void MaterialFactory::addModifier(const IMaterialModifierPtr& modifier)
{
	m_modifiers.push_back(modifier);
}

void MaterialFactory::deleteModifiers()
{
	m_modifiers.clear();
}

void MaterialFactory::modifyMaterialByType(MaterialPtr& material, BUILT_IN_MATERIAL_TYPE materialType, bool set)
{
	FLT_ASSERT(materialType<m_modifiers.size());

	if(set)
	{
		m_modifiers[materialType]->set(material);
	}
	else
	{
		m_modifiers[materialType]->unset(material);
	}

}

//////////////////////////////////////////////////////////////////////////////

void MaterialModifier_2D_SOLID::set(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = false;
	rs.DepthWrite = false;	
}

void MaterialModifier_2D_SOLID::unset(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = true;
	rs.DepthWrite = true;	
}

//////////////////////////////////////////////////////////////////////////////

void MaterialModifier_2D_ALPHA_BLEND::set(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = false;
	rs.DepthWrite = false;
	rs.BlendMode = BLEND_ENABLE;
	rs.BlendSrcFactor = BLD_SRC_ALPHA;
	rs.BlendDstFactor = BLD_ONE_MINUS_SRC_ALPHA;
}

void MaterialModifier_2D_ALPHA_BLEND::unset(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = true;
	rs.DepthWrite = true;
	rs.BlendMode = BLEND_DISABLE;
	rs.BlendSrcFactor = BLD_ONE;
	rs.BlendDstFactor = BLD_ZERO;
}

//////////////////////////////////////////////////////////////////////////////

void MaterialModifier_2D_ADDITIVE::set(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = false;
	rs.DepthWrite = false;
	rs.BlendMode = BLEND_ENABLE;
	rs.BlendSrcFactor = BLD_SRC_ALPHA;
	rs.BlendDstFactor = BLD_ONE;
}

void MaterialModifier_2D_ADDITIVE::unset(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	

	rs.DepthTest = true;
	rs.DepthWrite = true;
	rs.BlendMode = BLEND_DISABLE;
	rs.BlendSrcFactor = BLD_ONE;
	rs.BlendDstFactor = BLD_ZERO;
}

//////////////////////////////////////////////////////////////////////////////

void MaterialModifier_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR::set(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();
	TextureLayer& layer = pass->getTextureLayer(0);

	rs.DepthTest = false;
	rs.DepthWrite = false;
	rs.BlendMode = BLEND_ENABLE;
	rs.BlendSrcFactor = BLD_SRC_ALPHA;
	rs.BlendDstFactor = BLD_ONE_MINUS_SRC_ALPHA;
	//layer.m_texState.EnvMode = TEXENV_MODULATE;
	//for test tint
	layer.m_texState.EnvMode = TEXENV_COMBINE;
	layer.m_texState.EnvBlendColor.set(0.5,0,0,0);
	
	//alpha is modulate by vertex color
	layer.m_texState.CombineFuncAlpha = TEXCBF_MODULATE;
	layer.m_texState.CombineSrc0Alpha = TEXCBS_PRIMARY_COLOR;
	layer.m_texState.CombineSrc1Alpha = TEXCBS_TEXTURE;
	
	//rgb is tint between vertex color & texture by the const color
	layer.m_texState.CombineFuncRGB = TEXCBF_INTERPOLATE;
	layer.m_texState.CombineSrc0RGB = TEXCBS_PRIMARY_COLOR;
	layer.m_texState.CombineSrc1RGB = TEXCBS_TEXTURE;
	layer.m_texState.CombineSrc2RGB = TEXCBS_CONSTANT;
}

void MaterialModifier_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR::unset(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();
	TextureLayer& layer = pass->getTextureLayer(0);

	rs.DepthTest = true;
	rs.DepthWrite = true;
	rs.BlendMode = BLEND_DISABLE;
	rs.BlendSrcFactor = BLD_ONE;
	rs.BlendDstFactor = BLD_ZERO;
	layer.m_texState.EnvMode = TEXENV_REPLACE;
}

//////////////////////////////////////////////////////////////////////////////

void MaterialModifier_2D_TEXTURE_ADDITIVE_MODULATE_COLOR::set(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();	
	TextureLayer& layer = pass->getTextureLayer(0);

	rs.DepthTest = false;
	rs.DepthWrite = false;
	rs.BlendMode = BLEND_ENABLE;
	rs.BlendSrcFactor = BLD_SRC_ALPHA;
	rs.BlendDstFactor = BLD_ONE;
	layer.m_texState.EnvMode = TEXENV_MODULATE;
}

void MaterialModifier_2D_TEXTURE_ADDITIVE_MODULATE_COLOR::unset(MaterialPtr& material)
{
	RenderPass* pass = material->getRenderPass(0);
	RenderStates& rs = pass->getRenderStates();
	TextureLayer& layer = pass->getTextureLayer(0);

	rs.DepthTest = true;
	rs.DepthWrite = true;
	rs.BlendMode = BLEND_DISABLE;
	rs.BlendSrcFactor = BLD_ONE;
	rs.BlendDstFactor = BLD_ZERO;
	layer.m_texState.EnvMode = TEXENV_REPLACE;
}


} //end namespace renderer
} //end namespace flt