#ifndef FLT_RENDER_PASS_H
#define FLT_RENDER_PASS_H

#include "renderer/material/RenderStates.h"
#include "renderer/material/TextureLayer.h"

namespace flt
{	
namespace renderer
{	


class RenderPass
{
public:		
	RenderPass();
	~RenderPass();

	RenderPass& operator=( const RenderPass& rhs );

	TextureLayer* addTextureLayer();

	RenderStates& getRenderStates() { return m_renderStates; }
	const RenderStates& getRenderStates() const { return m_renderStates; }

	u32 getTextureLayerNum() const { return (u32)m_textureLayers.size(); }
	TextureLayer& getTextureLayer(u32 index);
	const TextureLayer& getTextureLayer(u32 index) const;

	void removeAllTextureLayers();

	void setDynamic(bool isDynamic) { m_isDynamic = isDynamic; }

	void notifyChange();
	s32 getPassID(); //ID used for identify a RenderPass

private:
	RenderStates m_renderStates;
	typedef array_t<TextureLayer*> TextureLayerArray;
	TextureLayerArray m_textureLayers;

	s32 m_passID;
	bool m_isDynamic;

	RenderPass(const RenderPass& rhs);

};


} //end namespace renderer
} //end namespace flt

#endif //FLT_RENDER_PASS_H