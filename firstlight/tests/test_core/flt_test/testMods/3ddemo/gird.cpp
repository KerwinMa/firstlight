#include "gird.h"

Grid::Grid(int rows, int cols, float girdWidth)
{
	m_rows = rows;
	m_cols = cols;
	m_girdWidth = girdWidth;
}

void Grid::buildMesh()
{
	int vertexCount = m_rows * m_cols;
	int triangleCount = (m_cols-1)*(m_rows-1)*2;
	m_vertexArray.initTriangleListInterleaved(renderer::VAF_COLOR, vertexCount, triangleCount);

	int vertexIndex = 0;
	core::vector3df pos;
	pos.Z = 0;
	renderer::Colorf color;
	color.a = 1.0f;

	int totoalWidthX = (m_cols-1)*m_girdWidth;
	int totoalWidthY = (m_rows-1)*m_girdWidth;

	for(int i=0; i<m_rows; i++)
	{
		for(int j=0; j<m_cols; j++)
		{
			pos.X = j*m_girdWidth - totoalWidthX/2;
			pos.Y = i*m_girdWidth - totoalWidthY/2;
			m_vertexArray.setInterleavedArrayVertexPos(vertexIndex, pos);

			if(i%2==0)
			{
				//if(j%2==0)
					color.set(1,1,1);
				//else
				//	color.set(1,0,0);
			}
			else
			{
				//if(j%2==0)
					color.set(0,0,1);
				//else
				//	color.set(0,1,0);
			}
				
			m_vertexArray.setInterleavedArrayVertexColor(vertexIndex, color);
		
			vertexIndex++;
		}
	}

	//indices

	// v3 -- v2
	// |    / |
	// |   /  |
	// |  /   |
	// v1 -- v4

	int triangleIndex = 0;

	for(int i=0; i<m_rows-1; i++)
	{
		for(int j=0; j<m_cols-1; j++)
		{
			//left-top triangle
			int v1 = i*m_cols+j;
			int v2 = (i+1)*m_cols+j+1;
			int v3 = (i+1)*m_cols+j;
			m_vertexArray.setInterleavedArrayIndex(triangleIndex++, v3, v1, v2);

			//right-bottom triangle
			int v4 = i*m_cols+j+1;
			m_vertexArray.setInterleavedArrayIndex(triangleIndex++, v2, v1, v4);
		}
	}

	//build material
	flt::renderer::MaterialManager& materialMgr = renderer::MaterialManager::getInstance();
	flt::renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();

	flt::renderer::MaterialPtr mat = new flt::renderer::Material();
	mat->setName("__grid");

	flt::renderer::RenderTechnique* tech = mat->addTechnique();

	flt::renderer::RenderPass* pass = tech->addPass();

	//flt::renderer::TextureLayer* layer = pass->addTextureLayer();
	//layer->m_texture = texMgr.getRes("birds.tga");
	//layer->m_texState.EnvMode = renderer::TEXENV_MODULATE;

	flt::renderer::RenderStates& rs = pass->getRenderStates();				
	rs.FrontFaceOrder = renderer::FT_CCW;	

	mat->setActiveTechnique(0);
	materialMgr.addRes(mat);
}

void Grid::render(const renderer::IRendererPtr& renderer)
{
	renderer::MaterialPtr mat = renderer::MaterialManager::getInstance().getRes("__grid");

	renderer->setWorldTransform(flt::core::IdentityMatrix);

	renderer->setMaterial(mat);

	//renderer->enableLighting(true);
	renderer->drawPrimitives(m_vertexArray);
}