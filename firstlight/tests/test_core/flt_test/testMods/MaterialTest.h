#ifndef FLT_MATERIAL_TEST_H
#define FLT_MATERIAL_TEST_H

#include "../ITestMod.h"

class MaterialTest: public ITestMod
{
public:
	MaterialTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}

	virtual bool init()
	{
		//--------------------------------
		//载入图片纹理
		renderer::IRendererPtr renderer = m_devicePtr->getRenderer();
		Image* img = renderer->createImageFromFile("res/flt.tga");//this function use raw file system, not archive
		delete img;

		//-------------------------------
		//测试纹理载入，使用了renderer内部的TextureManager
		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();

		renderer::TexturePtr tex = texMgr.getRes("flt.tga");
		renderer::TexturePtr tex2 = texMgr.getRes("flt.tga");
		//texMgr->removeRes(tex->getName()); //从管理器去除，这样再get这个资源，就会重新创建，如果确认这个资源不需要再用了，first 保证所有引用ptr失效，and then调用这个remove

		//--------------------------------
		//载入材质脚本
		{
			MaterialPtr mat = MaterialManager::getInstance().getRes("mat2da.material");
			mat->setName("material1");		
			RenderPass * pass = mat->getActiveTechnique()->getPass(0);
			pass->getRenderStates().DepthTest = false;
		}
		//手工创建材质并加入材质管理器	
		{
			MaterialPtr mat = new Material();
			mat->setName("material2");

			RenderTechnique* tech = mat->addTechnique();

			RenderPass* pass = tech->addPass();

			TextureLayer* layer = pass->addTextureLayer();
			layer->m_texture = texMgr.getRes("test.tga");
			layer->m_texState.EnvMode = TEXENV_MODULATE;

			RenderStates& rs = pass->getRenderStates();				
			rs.FrontFaceOrder = FT_CCW;				

			mat->setActiveTechnique(0);
			MaterialManager::getInstance().addRes(mat);		
		}

		//set some global states
		m_devicePtr->getRenderer()->enableLighting(false);
		m_devicePtr->getRenderer()->setSceneAmbientLightColor(renderer::Colorf(1.0f,1.0f,1.0f));

		//--------------------------------

		int k_quards_num = 5;
		m_testQuards.initForQuardList3df(renderer::VAF_COLOR, k_quards_num);
		for(int i=0; i<k_quards_num; i++)
		{
			f32 x = i*10;
			f32 y = i*10;

			u32 r = m_devicePtr->getRandS32(0,256);
			u32 g = m_devicePtr->getRandS32(0,256);
			u32 b = m_devicePtr->getRandS32(0,256);
            
            renderer::Color color1(128,r,g,b);

			m_testQuards.appendQuard3df(core::vector3df(x,y,-100.0f), 15.0f, 15.0f, 
				&color1);
		}	
		m_testQuards.compile();


		k_quards_num = 10;
		m_testQuards2.initForQuardList3df(renderer::VAF_COLOR, k_quards_num);
		for(int i=0; i<k_quards_num; i++)
		{
			f32 x = i*12-k_quards_num*12/2.0f;
			f32 y = 50;

			u32 r = m_devicePtr->getRandS32(0,256);
			u32 g = m_devicePtr->getRandS32(0,256);
			u32 b = m_devicePtr->getRandS32(0,256);
            
            renderer::Color color1(128,r,g,b);

			m_testQuards2.appendQuard3df(core::vector3df(x,y,-100.0f), 10.0f, 10.0f, 
				&color1);
		}
		m_testQuards2.compile();	

		return true;
	}

	virtual void destroy()
	{
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		MaterialPtr mat = MaterialManager::getInstance().getRes("material1");
		mat->setTexture(0,0,renderer::TextureManager::getInstance().getRes("flt.tga"));

		renderer->setWorldTransform(flt::core::IdentityMatrix);

		renderer->setMaterial(mat);

		//renderer->enableLighting(true);
		renderer->drawPrimitives(m_testQuards);

		//renderer->enableLighting(false);
		mat = MaterialManager::getInstance().getRes("material2");	
		renderer->setMaterial(mat);
		renderer->drawPrimitives(m_testQuards2);

	}

private:
	renderer::VertexArray m_testQuards, m_testQuards2;
};

#endif //FLT_MATERIAL_TEST_H

