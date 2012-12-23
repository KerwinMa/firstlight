

#ifndef __XSPRITE_H__
#define __XSPRITE_H__

#include "common.h"
#include "sharePtr.h"
#include "stream/FileStream.h"
#include "core/matrix4.h"
#include "renderer/Color.h"
#include "BaseObject.h"
#include "sharePtr.h"

namespace flt
{

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)

	FLT_FORWARD_PTR(Texture)
}

FLT_FORWARD_PTR(CXSprite)

class CXSprite: public BaseObject
{
public:
	
	//sprite transform, same to J2ME-MIDP2.0
	static const int TRANS_NONE = 0;
	static const int TRANS_ROT90 = 5;
	static const int TRANS_ROT180 = 3;
	static const int TRANS_ROT270 = 6;
	static const int TRANS_MIRROR = 2;
	static const int TRANS_MIRROR_ROT90 = 7;
	static const int TRANS_MIRROR_ROT180 = 1;
	static const int TRANS_MIRROR_ROT270 = 4;		


public:
	CXSprite();
	~CXSprite(void);

	bool LoadFromFile(const char* fileName);
	
	void SetTexture(const renderer::TexturePtr& texture);

	//these methods are used with XAnimObject, and XAnimObject should set object matrix each frame
	void DrawFrame(const renderer::IRendererPtr& renderer, int frame, float x, float y);
	void DrawAframe(const renderer::IRendererPtr& renderer, int anim, int aframe, float x, float y);
	void DrawFModule(const renderer::IRendererPtr& renderer, int frame, int fmodule, float x, float y);
	void DrawModule(const renderer::IRendererPtr& renderer, int module, float x, float y, float rotation=0.0f);

	//these two methods are used standalone (a.k. Not using with XAnimObject)
	//objectMatrix is overried by the prameters
	void DrawFrameEx(const renderer::IRendererPtr& renderer, int frame, float x, float y, float scalex, float scaley, float angle=0.0f);
	void DrawModuleEx(const renderer::IRendererPtr& renderer, int module, float x, float y, float scalex, float scaley, float angle=0.0f);
	
	int GetAnimNum()
	{
		return m_animNum;
	}

	int GetAnimFrameNum(int anim)
	{
		return m_anim_aframeNums[anim];
	}

	float GetAFrameTime(int animID, int aframeID)
	{
		int aframeStart = m_anim_aframeStarts[animID];

		return m_aframe_times[aframeStart+aframeID]*1000.0f/30.0f; //30fps
	}

	int GetModuleWidth(int module) { return m_module_cwidths[module]; }
	int GetModuleHeight(int module) { return m_module_cheights[module]; }

	void SetMaterial2D(int materialType) { m_materialType = materialType; }

	//set matrix from XAnimObject, so can apply Object level scale & rotation when drawing sprite
	void SetObjectMatrix(const core::matrix4& objectMatrix) { m_objectMatrix = objectMatrix; }

	void SetColor(const renderer::Color& color) 
	{ 
		for(int i=0; i<4; ++i)
			m_colors[i] = color; 

		m_useColor = true;
	}

	void SetColor(int corner, const renderer::Color& color)
	{
		m_colors[corner] = color;
		m_useColor = true;
	}

	void UnsetColor()
	{
		m_useColor = false;
	}

	void CalcAnimFrameRect(int anim, int aframeID, core::rectf& rect);

protected:
	
	renderer::TexturePtr m_texture;
	//GLfixed m_texEnvMode;

	int m_materialType;

	//bool m_ownerTexture ;

	//matrix set by parent animation object
	core::matrix4 m_objectMatrix;	

	renderer::Color m_colors[4];

	bool m_useColor;

	/////////////////////////////

	//The layout of a sprite is:
	// Modules: 	for now only support one texture
	// FModules: pack fmodule datas one frame after another, that is to say frame don't share fmodules, and their fmodule data only pack in a big arrays
	// Frames: only hold fmodule num and first fmoudle index for each frame
	// AFrames: pack aframe datas one anim after another
	// Anims: only hold aframe num and first aframe index for each anim

	//Modules
	int m_moduleNum;
	u16* m_module_cxs;		//clip rect of the module on texture
	u16* m_module_cys;		//the clip x, y maybe larger than 255 on large textures
	u16* m_module_cwidths;	//but the clip width, height is limited under 256
	u16* m_module_cheights;	//to save memory, and the module should not so bigger

	//FModules
	int m_fmoduleNum;
	u8* m_fmodule_moduleIndices;	//indices to the module, max 256 modules on a texture
	u8* m_fmodule_flags;
	s16* m_fmodule_offsetXs;			//offset x on the frame
	s16* m_fmodule_offsetYs;			//offset y on the frame
	f32* m_fmodule_rotations;			//rotation of the fmodule

	//Frames
	int m_frameNum;
	u16* m_frame_fmoduleNums;	//number of fmoudles on the frame
	u16* m_frame_fmoduleStarts; // first fmoudle's index in the frame
	//s16* m_frame_rects;

	//AFrames
	int m_aframeNum;
	u16* m_aframe_frameIndices; //indices to frame,
	u8* m_aframe_times;	//time delays of a aframe
	u8* m_aframe_flags;
	s16* m_aframe_offsetXs;
	s16* m_aframe_offsetYs;

	//Animations
	int m_animNum;
	u16*	m_anim_aframeNums; //number of aframe on the anim
	u16* m_anim_aframeStarts;	//first aframe index in the anim

	void InitSpriteData();
	void SafeReleaseSpriteData();




};

}//namespace flt

#endif //__XSPRITE_H__
