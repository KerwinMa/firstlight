#ifndef PIC_DANCER_TEST_H
#define PIC_DANCER_TEST_H

#include "../ITestMod.h"
#include "picdance/boneAnim.h"
#include "picdance/kfAnim/kfAnimManager.h"

class PicDancerTest: public ITestMod
{
public:
	PicDancerTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}

	virtual bool init()
	{
		////////// test kfAnim ///////////////
		kfAnim::AnimManager::newInstance();

		kfAnim::Anim* anim = kfAnim::AnimManager::getInstance().getRes("anim_test.json");

		m_dancer.buildBody();
		m_dancer.bindAnim(anim);	

		MaterialPtr mat = MaterialManager::getInstance().getRes("mat2da.material");
		mat->setName("danceMaterial");	
		mat->setTexture(0,0,renderer::TextureManager::getInstance().getRes("test.tga"));


		return true;
	}

	virtual void destroy()
	{
		kfAnim::AnimManager::deleteInstance();
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;

		m_dancer.update(dt);
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		MaterialPtr mat = MaterialManager::getInstance().getRes("danceMaterial");

		m_dancer.render(renderer,mat);		
	}

private:
	PicDancer m_dancer;
};

#endif //PIC_DANCER_TEST_H

