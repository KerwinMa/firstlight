#include "RenderPass.h"
#include "RenderStates.h"

namespace flt
{
namespace renderer
{

RenderPass::RenderPass()	
	:m_passID(-1),
	 m_isDynamic(false)
{
}

RenderPass::~RenderPass()
{
	removeAllTextureLayers();
}

RenderPass& RenderPass::operator=( const RenderPass& rhs )
{
	if(this==&rhs)
		return *this;

	removeAllTextureLayers();

	m_renderStates = rhs.m_renderStates;

	u32 layer_num = rhs.getTextureLayerNum();

	for(u32 i=0; i<layer_num; ++i)
	{
		TextureLayer* layer = addTextureLayer();
		*layer = rhs.getTextureLayer(i);
	}

	return *this;
}

TextureLayer* RenderPass::addTextureLayer()
{
	TextureLayer* layer = new TextureLayer();
	m_textureLayers.push_back(layer);
	return layer;
}

TextureLayer& RenderPass::getTextureLayer(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_textureLayers.size());

	return *m_textureLayers[index];
}

const TextureLayer& RenderPass::getTextureLayer(u32 index) const
{
	FLT_ASSERT(index>=0 && index<m_textureLayers.size());

	return *m_textureLayers[index];
}

void RenderPass::removeAllTextureLayers()
{
	ForEach(TextureLayerArray, m_textureLayers, iter)
	{
		delete *iter;
	}

	m_textureLayers.clear();
}

}// end namespace renderer
}// end namespace flt