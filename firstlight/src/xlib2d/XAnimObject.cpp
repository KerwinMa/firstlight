#include "XAnimObject.h"
#include "XSprite.h"

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

namespace flt
{

CXAnimObject::CXAnimObject()
	:m_xsprite(NULL),
	m_flag(0),
	m_curAnim(-1),
	m_curAFrame(0),
	m_frameTimer(0),
	m_x(0.0f),
	m_y(0.0f),
	m_scaleX(1.0f),
	m_scaleY(1.0f),
	m_angle(0.0f),
	m_flipX(false),
	m_flipY(false),
	m_sizeScaleX(1.0f),
	m_sizeScaleY(1.0f)
{	
	m_isLocalXformDirty = true;
	updateLocalXForm();
}

CXAnimObject::CXAnimObject(CXSprite* xsprite)
	:m_flag(0),
	m_curAnim(-1),
	m_curAFrame(0),
	m_frameTimer(0),
	m_scaleX(1.0f),
	m_scaleY(1.0f),
	m_angle(0.0f),
	m_flipX(false),
	m_flipY(false),
	m_sizeScaleX(1.0f),
	m_sizeScaleY(1.0f)
{
	FLT_ASSERT(xsprite!=NULL);

	m_xsprite = xsprite;
}

CXAnimObject::~CXAnimObject()
{
	m_xsprite = NULL;
}

void CXAnimObject::Init(CXSprite* xsprite)
{
	FLT_ASSERT(xsprite!=NULL);

	m_xsprite = xsprite;
	m_curAnim = -1;
	m_curAFrame = 0;
	m_frameTimer = 0;

#ifdef USE_FRAME_INTERP
	m_nextAFrame = 0;
	m_frameTimeMax = 0;
#endif
}

void CXAnimObject::SetSprite(CXSprite* xsprite)
{
	m_xsprite = xsprite;
	m_curAnim = -1;
	m_curAFrame = 0;
	m_frameTimer = 0;

#ifdef USE_FRAME_INTERP
	m_nextAFrame = 0;
	m_frameTimeMax = 0;
#endif
}


void CXAnimObject::SetAnim(int animID, bool loop/*=false*/)
{
	FLT_ASSERT(animID>=0);
	
	ClearFlag(AO_FLAG_ANIM_OVER|AO_FLAG_IS_LOOP);
	m_curAnim = animID;
	m_curAFrame = 0;

	m_frameTimer = m_xsprite->GetAFrameTime(m_curAnim,m_curAFrame);

#ifdef USE_FRAME_INTERP
	m_nextAFrame = m_curAFrame+1;
	if(m_nextAFrame>=m_xsprite->GetAnimFrameNum(m_curAnim))
	{
		m_nextAFrame--;
	}	

	m_frameTimeMax = m_frameTimer;
	m_percent = 1.0f;
#endif

	if(loop)
		SetFlag(AO_FLAG_IS_LOOP);
}

void CXAnimObject::SetAnimOnce(int animID, bool loop)
{
	if(m_curAnim!=animID)
		SetAnim(animID, loop);
}


void CXAnimObject::UpdateAnim(float dt)	
{
	if(sm_bPauseUpdate || m_curAnim<0)
		return;

	if(TestFlag(AO_FLAG_IS_LOOP|AO_FLAG_ANIM_OVER))
		ClearFlag(AO_FLAG_ANIM_OVER);

	m_frameTimer -= dt;
	if(m_frameTimer > 0)
	{
#ifdef USE_FRAME_INTERP
		m_percent = 1.0f-(m_frameTimeMax-m_frameTimer)/m_frameTimeMax;
#endif
		return;
	}

	m_curAFrame++;	

	int length = m_xsprite->GetAnimFrameNum(m_curAnim);
	if(m_curAFrame >= length)
	{
		SetFlag(AO_FLAG_ANIM_OVER);
		if(TestFlag(AO_FLAG_IS_LOOP))
		{
			m_curAFrame = 0;
		}
		else
		{
			m_curAFrame --;
		}
	}

	m_frameTimer = m_xsprite->GetAFrameTime(m_curAnim,m_curAFrame);

#ifdef USE_FRAME_INTERP
	m_nextAFrame = m_curAFrame+1;
	if(m_nextAFrame>=length)
	{
		m_nextAFrame--;
	}	

	m_frameTimeMax = m_frameTimer;
	m_percent = 1.0f;
#endif
}

void CXAnimObject::updateLocalXForm()
{
	if(m_isLocalXformDirty)
	{
		core::matrix4 matT, matRot;

		m_localXform.makeIdentity();
		//rotation center is frame space center
		core::vector3df centerOff(m_x, m_y, 0.0f);		
		matT.setTranslation(centerOff);
		
		float scale_x = m_scaleX * m_sizeScaleX;
		float scale_y = m_scaleY * m_sizeScaleY;

		m_localXform.setScale(core::vector3df(m_flipX?-scale_x:scale_x,m_flipY?-scale_y:scale_y,1.0f));
		matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-m_angle));
		
		m_localXform = matT*matRot*m_localXform;

		m_isLocalXformDirty = false;
	}
}

void CXAnimObject::Draw(const renderer::IRendererPtr& renderer, const core::matrix4* pParentXform)
{
	FLT_ASSERT(m_xsprite!=NULL);

	if(m_curAnim<0 || TestFlag(AO_FLAG_HIDE))
		return;

	updateLocalXForm();

	if(pParentXform!=0)
	{
		m_xsprite->SetObjectMatrix((*pParentXform)*m_localXform);
	}
	else
	{
		m_xsprite->SetObjectMatrix(m_localXform);
	}
	
	m_xsprite->DrawAframe(renderer, m_curAnim, m_curAFrame, 0, 0);
	m_xsprite->SetObjectMatrix(core::IdentityMatrix);
}

/*
void CXAnimObject::Draw(const renderer::IRendererPtr& renderer, float x, float y)
{
	FLT_ASSERT(m_xsprite!=NULL);

	if(m_curAnim<0 || TestFlag(AO_FLAG_HIDE))
		return;

	core::matrix4 matObj;
	
	if(!core::equals(m_scaleX,1.0f) || !core::equals(m_scaleY,1.0f) || m_flipX || m_flipY || !core::iszero(m_angle))
	{	
		core::matrix4 matT, matT2, matRot;
		//rotation center is frame space center
		core::vector3df centerOff(x, y, 0.0f);		
		matT.setTranslation(-centerOff);
		matT2.setTranslation(centerOff);
		matObj.setScale(core::vector3df(m_flipX?-m_scaleX:m_scaleX,m_flipY?-m_scaleY:m_scaleY,1.0f));
		matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-m_angle));
		
		matObj = matT2*matObj*matRot*matT;
	}	

	m_xsprite->SetObjectMatrix(matObj);
	m_xsprite->DrawAframe(renderer, m_curAnim, m_curAFrame, x, y);
	m_xsprite->SetObjectMatrix(core::IdentityMatrix);
		
}
*/

void CXAnimObject::DrawAnim(const renderer::IRendererPtr& renderer, int animID, float x, float y, bool loop/* =false */)
{
	if(m_curAnim!=animID || TestFlag(AO_FLAG_IS_LOOP)!=loop)
		SetAnim(animID, loop);

	SetPos(x,y);

	Draw(renderer);
}

bool CXAnimObject::sm_bPauseUpdate = false;

void CXAnimObject::SetPause(bool bPause)
{
	sm_bPauseUpdate = bPause;
}

void CXAnimObject::CalcAnimFrameRect(int anim, int aframeID, core::rectf& rect)
{
	FLT_ASSERT(m_xsprite!=NULL);	

	core::matrix4 matObj;

	if(!core::equals(m_scaleX,1.0f) || !core::equals(m_scaleY,1.0f) 
		|| !core::equals(m_sizeScaleX,1.0f) || !core::equals(m_sizeScaleY,1.0f)
		|| m_flipX || m_flipY || !core::iszero(m_angle))
	{	
		core::matrix4 matT, matT2, matRot;
		//rotation center is frame space center	
		
		float scale_x = m_scaleX * m_sizeScaleX;
		float scale_y = m_scaleY * m_sizeScaleY;

		matObj.setScale(core::vector3df(m_flipX?-scale_x:scale_x,m_flipY?-scale_y:scale_y,1.0f));
		matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-m_angle));

		matObj = matObj*matRot;
	}	

	m_xsprite->SetObjectMatrix(matObj);	

	m_xsprite->CalcAnimFrameRect(anim, aframeID, rect);

	m_xsprite->SetObjectMatrix(core::IdentityMatrix);
}

}//namespace flt
