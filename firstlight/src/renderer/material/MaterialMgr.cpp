#include "MaterialMgr.h"
#include "stream/FileStream.h"
#include "archive/ArchiveManager.h"
#include "utils/Logger.h"
#include "externs/jsoncpp/json.h"
#include "renderer/material/TextureMgr.h"

namespace flt
{
namespace renderer
{

MaterialPtr MaterialManager::createRes(const stringc& name, int flag)
{
	FileStream file;
	ArchiveManager::getInstance().openFile(file, name);
	
	MaterialPtr newMaterial = new Material();
	newMaterial->setName(name);

	if(!file.IsValid())
		return newMaterial;

	Json::Reader reader;
	Json::Value root;

	bool ok = reader.parse(&file, root, false);
	FLT_ASSERT(ok);

	if(!ok)
		return newMaterial;

	//renderer::TextureManagerPtr texMgr = renderer->getTextureManager();

	Json::Value& techniquesData = root["techniques"];
	for(u32 t=0; t<techniquesData.size(); t++)
	{
		RenderTechnique* tech = newMaterial->addTechnique();
		Json::Value& techData = techniquesData[t];
		Json::Value& passesData = techData["passes"];
		for(u32 p=0; p<passesData.size(); p++)
		{
			RenderPass* pass = tech->addPass();

			Json::Value::iterator it = passesData[p].begin();
			for( ; it !=passesData[p].end(); ++it) 
			{ 
				Json::Value v = *it; 
				flt::stringc vname(it.memberName());
				if(vname=="textures")
				{
					//add texture layers					
					for(u32 tex=0; tex<v.size(); tex++)
					{
						TextureLayer* layer = pass->addTextureLayer();

						Json::Value::iterator texIt = v[tex].begin();
						for( ; texIt !=v[tex].end(); ++texIt) 
						{ 
							Json::Value texv = *texIt; 
							flt::stringc texvname(texIt.memberName());
							flt::stringc texValue = texv.asString();

							#define SET_IF(ts, strValue) if( texValue == #strValue ) layer->m_texState.ts = strValue;						
							
							if(texvname=="textureResName")
							{																
								if(texValue.length()>0)
								{
									TextureManager& texMgr = TextureManager::getInstance();
									layer->m_texture = texMgr.getRes(texValue);
								}
							}
							else if(texvname=="WrapModeU")
							{
								SET_IF(WrapModeU,TEXW_REPEAT)
								else SET_IF(WrapModeU,TEXW_CLAMP_TO_EDGE)
							}
							else if(texvname=="WrapModeV")
							{
							}
							else if(texvname=="EnvMode")
							{
								SET_IF(EnvMode,TEXENV_REPLACE)
								else SET_IF(EnvMode,TEXENV_DECAL)
								else SET_IF(EnvMode,TEXENV_MODULATE)
								else SET_IF(EnvMode,TEXENV_BLEND)
								else SET_IF(EnvMode,TEXENV_ADD)
								else SET_IF(EnvMode,TEXENV_COMBINE)
							}

							#undef SET_IF
						}//for tex params																		
					}//for tex layers
				}
				else
				{
					//set render state

					RenderStates& renderStates = pass->getRenderStates();

					flt::stringc value;
					if(!v.isArray())
						value = v.asString();

					#define SET_IF(rs, strValue) if( value == #strValue ) renderStates.rs = strValue;						

					if(vname=="ColorMaterialFlag")
					{											
						SET_IF(ColorMaterialFlag,CMF_NONE)
						else SET_IF(ColorMaterialFlag,CMF_DIFFUSE_AND_AMBIENT)
					}
					else if(vname=="BlendMode")
					{
						SET_IF(BlendMode,BLEND_ENABLE)
						else SET_IF(BlendMode,BLEND_DISABLE)
					}
					else if(vname=="BlendSrcFactor")
					{
						SET_IF(BlendSrcFactor,BLD_ZERO)
						else SET_IF(BlendSrcFactor,BLD_ONE)
						else SET_IF(BlendSrcFactor,BLD_SRC_COLOR)
						else SET_IF(BlendSrcFactor,BLD_ONE_MINUS_SRC_COLOR)
						else SET_IF(BlendSrcFactor,BLD_DST_COLOR)
						else SET_IF(BlendSrcFactor,BLD_ONE_MINUS_DST_COLOR)
						else SET_IF(BlendSrcFactor,BLD_SRC_ALPHA)
						else SET_IF(BlendSrcFactor,BLD_ONE_MINUS_SRC_ALPHA)
						else SET_IF(BlendSrcFactor,BLD_DST_ALPHA)
						else SET_IF(BlendSrcFactor,BLD_ONE_MINUS_DST_ALPHA)
						else SET_IF(BlendSrcFactor,BLD_SRC_ALPHA_SATURATE)
					}
					else if(vname=="BlendDstFactor")
					{
						SET_IF(BlendDstFactor,BLD_ZERO)
						else SET_IF(BlendDstFactor,BLD_ONE)
						else SET_IF(BlendDstFactor,BLD_SRC_COLOR)
						else SET_IF(BlendDstFactor,BLD_ONE_MINUS_SRC_COLOR)
						else SET_IF(BlendDstFactor,BLD_DST_COLOR)
						else SET_IF(BlendDstFactor,BLD_ONE_MINUS_DST_COLOR)
						else SET_IF(BlendDstFactor,BLD_SRC_ALPHA)
						else SET_IF(BlendDstFactor,BLD_ONE_MINUS_SRC_ALPHA)
						else SET_IF(BlendDstFactor,BLD_DST_ALPHA)
						else SET_IF(BlendDstFactor,BLD_ONE_MINUS_DST_ALPHA)
						else SET_IF(BlendDstFactor,BLD_SRC_ALPHA_SATURATE)
					}
					else if(vname=="Ambient")
					{
						FLT_ASSERT(v.isArray() && v.size()==3);
						f32 r = (f32)v[(u32)0].asDouble();
						f32 g = (f32)v[(u32)1].asDouble();
						f32 b = (f32)v[(u32)2].asDouble();
						renderStates.Ambient.set(r,g,b);
					}

					#undef SET_IF
				}
				
				//DBG("test","it %s", it.memberName());
			}//for renderstates and textures						
		}// for passes
	}//for techniques


	Json::Value& activeTech = root["active_technique"];
	u32 activeTechnqueIndex = activeTech.asUInt();
	newMaterial->setActiveTechnique(activeTechnqueIndex);

	return newMaterial;
}

MaterialPtr MaterialManager::cloneRes(const MaterialPtr& rhs, const stringc& newResName)
{
	MaterialPtr newMaterial = rhs;
	return newMaterial;
}


}// end namespace renderer
}// end namespace flt

