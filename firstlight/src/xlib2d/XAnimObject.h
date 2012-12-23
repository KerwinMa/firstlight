#ifndef __XANIMOBJECT_H__
#define __XANIMOBJECT_H__

#include "common.h"
#include "sharePtr.h"
#include "core/rect.h"
#include "core/vector2d.h"
#include "core/matrix4.h"

namespace flt
{

static const int AO_FLAG_FLIP_X = (1<<0) ;
static const int AO_FLAG_ANIM_OVER = (1<<1) ;
static const int AO_FLAG_IS_LOOP = (1<<2) ;
static const int AO_FLAG_HIDE = (1<<3) ;
static const int AO_FLAG_USEABLE = (1<<4) ; //for actor pool

class CXSprite;

namespace renderer
{
	FLT_FORWARD_PTR(IRenderer)
}

class CXAnimObject
{
public:
	CXAnimObject();
	CXAnimObject(CXSprite* xsprite);
	virtual ~CXAnimObject();

	void Init(CXSprite* xsprite);
	void SetSprite(CXSprite* xsprite); //maybe set to NULL, let the obj unusable
	CXSprite* GetSprite() { return m_xsprite; }

	void SetFlag(int flag) { m_flag |= flag; }
	void ClearFlag(int flag) { m_flag &= (~flag); }
	bool TestFlag(int flag) { return ((m_flag & flag) != 0); }
	
	void SetAnim(int animID, bool loop=false);	
	void SetAnimOnce(int animID, bool loop=false); //set anim only if it is not current anim.

	void UpdateAnim(float dt);	
	void Draw(const renderer::IRendererPtr& renderer, const core::matrix4* pParentXform=0);
	//void Draw(const renderer::IRendererPtr& renderer, float x, float y);
	void DrawAnim(const renderer::IRendererPtr& renderer, int animID, float x, float y, bool loop=false);

	void SetPos(const core::vector2df& pos) { m_x = pos.X; m_y = pos.Y; m_isLocalXformDirty = true; }
	void SetPos(float x, float y) { m_x = x; m_y = y; m_isLocalXformDirty = true; }
	void SetScale(float scale) { m_scaleX = m_scaleY = scale; m_isLocalXformDirty = true; }
	void SetScale(float scaleX, float scaleY) { m_scaleX = scaleX; m_scaleY = scaleY; m_isLocalXformDirty = true; }
	void SetRotation(float angle) { m_angle = angle; m_isLocalXformDirty = true; }
	void SetFlipX(bool flipX) { m_flipX = flipX; m_isLocalXformDirty = true; }
	void SetFlipY(bool flipY) { m_flipY = flipY; m_isLocalXformDirty = true; }
	void SetSizeScale(float sx, float sy) { m_sizeScaleX=sx; m_sizeScaleY=sy; m_isLocalXformDirty = true;}

	static void SetPause(bool bPause);

	int GetCurAFrame() { return m_curAFrame; }

	int GetCurAnim() { return m_curAnim; }

	void CalcAnimFrameRect(int anim, int aframeID, core::rectf& rect);

protected:
	CXSprite* m_xsprite;	
	int m_curAnim;
	int m_curAFrame;
	
#ifdef USE_FRAME_INTERP
	//for interpolate
	int m_nextAFrame;
	float m_percent; //how much curAframe apply
	int m_frameTimeMax;
#endif

	int m_frameTimer;
	int m_flag;

private:
	float m_x, m_y;
	float m_scaleX;
	float m_scaleY;
	float m_angle;
	bool m_flipX, m_flipY;

	//size scale is base scale
	float m_sizeScaleX;
	float m_sizeScaleY;

	core::matrix4 m_localXform;

	bool m_isLocalXformDirty;

	void updateLocalXForm();

	static bool sm_bPauseUpdate;
};

}//namespace flt

#endif //__XANIMOBJECT_H__
