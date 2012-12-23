#ifndef XLIB_TEST_H
#define XLIB_TEST_H

#include "../ITestMod.h"

class XLibTest: public ITestMod
{
public:
	XLibTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr)
	{
	}

	virtual bool init()
	{
		XSpriteManager &xspMgr = XSpriteManager::getInstance();
		renderer::TextureManager& texMgr = renderer::TextureManager::getInstance();
		XFontManager &fontMgr = XFontManager::getInstance();

		m_testXSprite = xspMgr.getRes("ft.xsp");		
		m_testXSprite->SetTexture(texMgr.getRes("ft.jpg")); //test jpg

		m_testXAnim = new CXAnimObject();
		m_testXAnim->SetSprite(m_testXSprite);
		m_testXAnim->SetAnim(0,true);

		m_testFont = fontMgr.getRes("font.xsp");	
		m_testFont->setName("TestFont");
		m_testFont->SetTexture(texMgr.getRes("font.tga"));

		return true;
	}

	virtual void destroy()
	{		
		delete m_testXAnim;		
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;

		m_testXAnim->UpdateAnim(dt);
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		renderer->begin2DMode();		
		
		m_testXAnim->SetScale(0.5f);
		m_testXAnim->SetRotation(90);
		m_testXAnim->SetFlipX(true);
		m_testXAnim->SetFlipY(true);
		m_testXAnim->SetPos(50,100);
		m_testXAnim->Draw(renderer);

		m_testXAnim->SetScale(1.5f);
		m_testXAnim->SetRotation(-30);
		m_testXAnim->SetFlipX(false);
		m_testXAnim->SetFlipY(false);
		m_testXAnim->SetPos(80,300);
		m_testXAnim->Draw(renderer);

		//test draw module & frame of xsprite	
		static float scl = 0.1f;
		static int scl_dir = 1;
		static float rot = 0.0f;

		m_testXSprite->SetMaterial2D(renderer::BIMT_2D_ADDITIVE);
		m_testXSprite->DrawModuleEx(renderer, 1, 100, 100, scl*2, scl*2, rot);	
		
		m_testXSprite->DrawFrameEx(renderer, 3, 100, 300, scl, scl, rot);

		m_testXSprite->SetMaterial2D(renderer::BIMT_2D_ALPHA_BLEND);

		rot+=10.f;
		if(rot>360)
			rot=0.0f;
		scl += 0.05f*scl_dir;
		if(scl>0.8f || scl<0.1f)
			scl_dir*=-1;
		
		m_testFont->SetColor(renderer::Color(255,0,200,0));
		m_testFont->SetScale(1.0f, 1.0f);
		m_testFont->SetAngle(20);
		m_testFont->DrawTextLine(renderer, "Hello\n world!", 100, 30, ALIGN_BOTTOM);

		m_testFont->UnsetColor();
		m_testFont->SetScale(0.5f, 1.0f);
		m_testFont->SetAngle(0);
		m_testFont->DrawTextLine(renderer, "Hello\n world!", 20, 30);

		renderer->end2DMode();
	}

private:
	CXSpritePtr m_testXSprite;
	CXAnimObject* m_testXAnim;
	CXFontPtr m_testFont;
};

#endif //XLIB_TEST_H

