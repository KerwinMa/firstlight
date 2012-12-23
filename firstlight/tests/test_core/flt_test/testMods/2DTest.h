#ifndef FLT_2D_TEST_H
#define FLT_2D_TEST_H

#include "../ITestMod.h"

class Flt2DTest: public ITestMod
{
public:
	Flt2DTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}

	virtual bool init()
	{
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

		//2d drawing...
		renderer->begin2DMode();

		//renderer->enableLighting(false);

		renderer->setClip2D(core::rectf(-50,30,1000,180));

		renderer->set2DMaterial(flt::renderer::BIMT_2D_SOLID);

		renderer->drawRect2D(flt::core::rectf(10,10,200,250), flt::renderer::Color(127,0,255,0));

		renderer->set2DMaterial(flt::renderer::BIMT_2D_ALPHA_BLEND);

		renderer->drawRect2D(flt::core::rectf(30,30,120,120), flt::renderer::Color(128,255,255,255));	

		flt::renderer::Color colors[] = { flt::renderer::Color(50,255,0,0),
			flt::renderer::Color(255,0,255,0),
			flt::renderer::Color(255,0,0,255),
			flt::renderer::Color(255,255,255,255)};
		renderer->drawRect2D(flt::core::rectf(20,20,100,100), colors);

		renderer->set2DMaterial(flt::renderer::BIMT_2D_ADDITIVE);

		renderer->drawRect2D(flt::core::rectf(200,200,50,50), colors);

		//renderer->set2DMaterial(flt::renderer::BIMT_2D_TEXTURE_ADDITIVE_MODULATE_COLOR);

		//flt::core::rectf srcRect(10,10,150,150);

		//renderer->drawImage2D(renderer::TextureManager::getInstance().getRes("test.tga"), 500, 10, &srcRect, colors);

		renderer->set2DMaterial(flt::renderer::BIMT_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR);

		renderer->resetClip2D();

		TexturePtr tex = renderer::TextureManager::getInstance().getRes("flt.tga");

		int tintAlpha = 128;
		flt::renderer::Color colorsTint[] = { flt::renderer::Color(tintAlpha,255,0,0),
			flt::renderer::Color(tintAlpha,255,0,0),
			flt::renderer::Color(tintAlpha,255,0,0),
			flt::renderer::Color(tintAlpha,255,0,0)};
        
        core::matrix4 skewMat;
                
        skewMat.setRotationDegrees2D(80.0f,0.0);

        renderer->drawImage2D(tex, 100, 100, 0, colorsTint);
		renderer->drawImage2D(tex, 100, 100, 0, 0, &skewMat);	

		//renderer->resetClip2D();

		renderer->end2DMode();
	}


};

#endif //FLT_2D_TEST_H

