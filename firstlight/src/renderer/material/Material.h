#ifndef FLT_MATERIAL_H
#define FLT_MATERIAL_H

#include "renderer/material/RenderTechnique.h"

namespace flt
{	
namespace renderer
{	

FLT_FORWARD_PTR(Material)

class Material: public BaseObject
{
public:		
	//! create a simple empty material with one active technique with one render pass and no texture layer
	static MaterialPtr createSimpleEmptyMaterial(bool hasTexture);

	Material();
	~Material();		

	/// texture is shared
	Material(const Material& rhs);

	//! destroy self , name reserved, texture shared
	Material& operator=( const Material& rhs );

	RenderTechnique* addTechnique();

	void removeTechnique(u32 index);

	RenderTechnique* getTechnique(u32 index);

	RenderTechnique* getTechnique(u32 index) const;

	void setActiveTechnique(u32 index);

	RenderTechnique* getActiveTechnique() { return m_activeTechnique; }

	u32 getTechniqueCount() const { return (u32)m_techniques.size(); }

	void removeAllTechniques();

	RenderPass* getRenderPass(int index);

	u32 getRenderPassCount();

	void setTexture(u32 passIndex, u32 textureLayerIndex, TexturePtr texture);

	/// Is the material can batch
	bool canBatch();

	/// Is the material match for batch
	bool isMatchForBatch(const MaterialPtr& rhs);

private:	
	typedef array_t<RenderTechnique*> RenderTechniqueArray;

	RenderTechniqueArray m_techniques;

	RenderTechnique* m_activeTechnique;

	s32 m_activeTechniqueIndex;
};


} //end namespace renderer
} //end namespace flt

#endif //FLT_MATERIAL_H