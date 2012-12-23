#include "Material.h"

namespace flt
{
namespace renderer
{

Material::Material():m_activeTechnique(0),m_activeTechniqueIndex(-1)
{

}

Material::~Material()
{
	removeAllTechniques();
}

Material::Material(const Material& rhs)
{		
	u32 tech_num = rhs.getTechniqueCount();
	for(u32 i=0; i<tech_num; ++i)
	{
		RenderTechnique* tech = addTechnique();		
		*tech = *rhs.getTechnique(i);
	}

	setActiveTechnique(rhs.m_activeTechniqueIndex);	
}

//! destroy self , name reserved, texture shared
Material& Material::operator=( const Material& rhs )
{
	if(this==&rhs)
		return *this;

	//destroy self
	removeAllTechniques();

	u32 tech_num = rhs.getTechniqueCount();
	for(u32 i=0; i<tech_num; ++i)
	{
		RenderTechnique* tech = addTechnique();
		*tech = *rhs.getTechnique(i);
	}

	setActiveTechnique(rhs.m_activeTechniqueIndex);

	return *this;
}

MaterialPtr Material::createSimpleEmptyMaterial(bool hasTexture)
{
	MaterialPtr mat = new Material();

	RenderTechnique* tech = mat->addTechnique();
	mat->setActiveTechnique(0);
	RenderPass* pass = tech->addPass();	

	if(hasTexture)
	{
		pass->addTextureLayer();
	}

	return mat;	
}

bool Material::canBatch()
{
	FLT_ASSERT(m_activeTechnique!=0);

	return (m_activeTechnique->getPassCount()==1); //only 1 pass can batch
}

bool Material::isMatchForBatch(const MaterialPtr& rhs)
{
	do
	{
		RenderPass* pass = m_activeTechnique->getPass(0);
		RenderPass* passR = rhs->m_activeTechnique->getPass(0);

		if(pass->getRenderStates()!= passR->getRenderStates())
			break;

		if(pass->getTextureLayerNum()!=passR->getTextureLayerNum())
			break;

		u32 layerNum = pass->getTextureLayerNum();
		for(u32 i=0; i<layerNum; ++i)
		{
			TextureLayer& layer = pass->getTextureLayer(i);
			TextureLayer& layerR = passR->getTextureLayer(i);
			if(layer!=layerR)
			{
				return false;
			}
		}	

		return true;
	}
	while(false);

	return false;
}

RenderTechnique* Material::addTechnique()
{
	RenderTechnique* technique = new RenderTechnique();
	m_techniques.push_back(technique);
	return technique;
}

void Material::removeTechnique(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_techniques.size());

	if(m_techniques[index]==m_activeTechnique)
	{
		m_activeTechnique = 0;
		m_activeTechniqueIndex = -1;
	}

	delete m_techniques[index];

	m_techniques.erase(m_techniques.begin()+index);
}

RenderTechnique* Material::getTechnique(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_techniques.size());

	return m_techniques[index];
}

RenderTechnique* Material::getTechnique(u32 index) const
{
	FLT_ASSERT(index>=0 && index<m_techniques.size());

	return m_techniques[index];
}

void Material::setActiveTechnique(u32 index)
{
	FLT_ASSERT(index>=0 && index<m_techniques.size());

	m_activeTechnique = m_techniques[index];

	m_activeTechniqueIndex = (s32)index;
}

void Material::removeAllTechniques()
{
	ForEach(RenderTechniqueArray, m_techniques, iter)
	{
		delete *iter;
	}

	m_techniques.clear();
	m_activeTechnique = 0;
	m_activeTechniqueIndex = -1;
}

RenderPass* Material::getRenderPass(int index)
{
	FLT_ASSERT(m_activeTechnique!=0);

	return m_activeTechnique->getPass(index);
}

u32 Material::getRenderPassCount()
{
	FLT_ASSERT(m_activeTechnique!=0);

	return m_activeTechnique->getPassCount();
}

void Material::setTexture(u32 passIndex, u32 textureLayerIndex, TexturePtr texture)
{
	m_activeTechnique->getPass(passIndex)->getTextureLayer(textureLayerIndex).m_texture = texture;
}

}// end namespace renderer
}// end namespace flt