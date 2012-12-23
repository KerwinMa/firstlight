#include "terrian.h"

Terrian::Terrian()
	:m_xyScale(1.0f)
	,m_heightScale(1.0f)
{
	utils::Logger::getInstance().enableGroup("terrian",true);
}

Terrian::~Terrian()
{
	unloadHeightMap();
}

bool Terrian::loadHeightMap(const char* fileName, int size)
{
	FileStream file;
	ArchiveManager::getInstance().openFile(file, fileName);
	if(!file.IsValid())
	{
		DBG("terrian","Can not open file: %s",fileName);
		return false;
	}

	if(m_heightData.m_data!=NULL)
		unloadHeightMap();

	m_heightData.m_data = new u8[size*size];

	if(m_heightData.m_data==NULL)
		return false;

	m_heightData.m_size = size;
	file.Read(m_heightData.m_data, size*size*sizeof(u8));

	DBG("terrian", "Load height map OK: %s", fileName);

	return true;
}

bool Terrian::unloadHeightMap()
{
	if(m_heightData.m_data!=NULL)
	{
		SAFE_DEL_ARRAY(m_heightData.m_data);
		m_heightData.m_size = 0;

		DBG("terrian", "Unload height map.");
	}

	return true;
}

void Terrian::buildMesh()
{
	int size = m_heightData.m_size;
	int vertexCount = size*size;
	int triangleCount = (size-1)*(size-1)*2;

	m_vertexArray.initTriangleListInterleaved(flt::renderer::VAF_COLOR|flt::renderer::VAF_UV, vertexCount, triangleCount);

	u32 vertexIndex = 0;
	flt::core::vector3df pos;
	flt::renderer::Colorf color;

	//set vertex data
	for(int y=0; y<size; y++)
	{
		for(int x=0; x<size; x++)
		{
			f32 height = getScaledHeightAtPoint(x,y);
			f32 trueH = getTrueHeightAtPoint(x,y);

			pos.X = (x-(size/2))*m_xyScale;
			pos.Y = (-y+(size/2))*m_xyScale;
			pos.Z = height;
			m_vertexArray.setInterleavedArrayVertexPos(vertexIndex, pos);
			//m_vertexArray.setInterleavedArrayUV(vertexIndex, 0, (float)y/(size-1), (float)x/(size-1));

			color.a = 0.3f;
			color.r = trueH/255.0f;
			color.g = trueH/255.0f;
			color.b = trueH/255.0f;
			m_vertexArray.setInterleavedArrayVertexColor(vertexIndex, color);

			vertexIndex++;
		}
	}

	//int buffersize = m_vertexArray.m_vertexCount * m_vertexArray.m_vertexSize;
	//for(int i=0; i<buffersize; i++)
	//{
	//	float v = m_vertexArray.m_interleavedArrayF32[i];
	//	DBG("terrian","i=%d,v=%f\n",i,v);
	//}

	u32 triangelIndex = 0;
    
    
    //  0----1
    //  |  / |
    //  | /  |
    //  2----3
    //

	//set indices
	for(int y=0; y<size-1; y++)
	{
		for(int x=0; x<size-1; x++)
		{
			
			u16 v0 = y*size+x;
            u16 v1 = y*size+x+1;
			u16 v2 = (y+1)*size+x;
            u16 v3 = (y+1)*size+x+1;
			
			//triangle top-left
			m_vertexArray.setInterleavedArrayIndex(triangelIndex++, v0, v2, v1);
			
			//triangle bottom-right
			m_vertexArray.setInterleavedArrayIndex(triangelIndex++, v1, v2, v3); 
			
		}
	}

	//build material
	flt::renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();

	flt::renderer::MaterialPtr mat = new flt::renderer::Material();
	mat->setName("terrian");

	flt::renderer::RenderTechnique* tech = mat->addTechnique();

	flt::renderer::RenderPass* pass = tech->addPass();

	//flt::renderer::TextureLayer* layer = pass->addTextureLayer();
	//layer->m_texture = texMgr.getRes("birds.tga");
	//layer->m_texState.EnvMode = renderer::TEXENV_MODULATE;

	flt::renderer::RenderStates& rs = pass->getRenderStates();				
	rs.FrontFaceOrder = renderer::FT_CCW;	
	rs.WireFrameMode = true;
	rs.BlendMode = renderer::BLEND_ENABLE;
	rs.BlendSrcFactor = renderer::BLD_SRC_ALPHA;
	rs.BlendDstFactor = renderer::BLD_ONE_MINUS_SRC_ALPHA;
	//rs.CullMode = renderer::CULL_NONE;

	mat->setActiveTechnique(0);
	flt::renderer::MaterialManager::getInstance().addRes(mat);
}

void Terrian::render(const renderer::IRendererPtr& renderer)
{
	renderer::MaterialPtr mat = renderer::MaterialManager::getInstance().getRes("terrian");

	renderer->setWorldTransform(flt::core::IdentityMatrix);

	renderer->setMaterial(mat);

	//renderer->enableLighting(true);
	renderer->drawPrimitives(m_vertexArray);

	//renderer->enableLighting(false);
}