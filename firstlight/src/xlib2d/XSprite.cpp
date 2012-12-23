#include "XSprite.h"
#include "firstlight.h" //temp,make compile ok

namespace flt
{


CXSprite::CXSprite(void)
	:m_materialType(renderer::BIMT_2D_ALPHA_BLEND),m_useColor(false)
{
	InitSpriteData();
	m_objectMatrix.makeIdentity();	
}

CXSprite::~CXSprite(void)
{	
	SafeReleaseSpriteData();
}

void CXSprite::SetTexture(const renderer::TexturePtr& texture) 
{ 	
	m_texture = texture; 
}

void CXSprite::InitSpriteData()
{
	m_moduleNum = 0;
	m_module_cxs = NULL;
	m_module_cys = NULL;
	m_module_cwidths = NULL;
	m_module_cheights = NULL;

	m_fmoduleNum = 0;
	m_fmodule_moduleIndices = NULL;
	m_fmodule_flags = NULL;
	m_fmodule_offsetXs = NULL;
	m_fmodule_offsetYs = NULL;
	m_fmodule_rotations = NULL;

	m_frameNum = 0;
	m_frame_fmoduleNums = NULL;
	m_frame_fmoduleStarts = NULL;

	m_aframeNum = 0;
	m_aframe_frameIndices = NULL;
	m_aframe_times = NULL;
	m_aframe_flags = NULL;
	m_aframe_offsetXs= NULL;
	m_aframe_offsetYs = NULL;

	m_animNum = 0;
	m_anim_aframeNums = NULL;
	m_anim_aframeStarts = NULL;	
}

void CXSprite::SafeReleaseSpriteData()
{
	m_moduleNum = 0;
	SAFE_DEL_ARRAY(m_module_cxs);
	SAFE_DEL_ARRAY(m_module_cys);
	SAFE_DEL_ARRAY(m_module_cwidths);
	SAFE_DEL_ARRAY(m_module_cheights);

	m_fmoduleNum = 0;
	SAFE_DEL_ARRAY(m_fmodule_moduleIndices);
	SAFE_DEL_ARRAY(m_fmodule_flags);
	SAFE_DEL_ARRAY(m_fmodule_offsetXs);
	SAFE_DEL_ARRAY(m_fmodule_offsetYs);
	SAFE_DEL_ARRAY(m_fmodule_rotations);

	m_frameNum = 0;
	SAFE_DEL_ARRAY(m_frame_fmoduleNums);
	SAFE_DEL_ARRAY(m_frame_fmoduleStarts);

	m_aframeNum = 0;
	SAFE_DEL_ARRAY(m_aframe_frameIndices);
	SAFE_DEL_ARRAY(m_aframe_times);
	SAFE_DEL_ARRAY(m_aframe_flags);
	SAFE_DEL_ARRAY(m_aframe_offsetXs);
	SAFE_DEL_ARRAY(m_aframe_offsetYs);

	m_animNum = 0;
	SAFE_DEL_ARRAY(m_anim_aframeNums);
	SAFE_DEL_ARRAY(m_anim_aframeStarts);
}

void CXSprite::DrawFrameEx(const renderer::IRendererPtr& renderer, int frame, float x, float y, float scalex, float scaley, float angle/*=0.0f*/)
{		
	core::matrix4 matT, matT2, matRot;
	//rotation center is frame space center
	core::vector3df centerOff(x, y, 0.0f);		
	matT.setTranslation(-centerOff);
	matT2.setTranslation(centerOff);
	m_objectMatrix.setScale(core::vector3df(scalex,scaley,1.0f));
	matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-angle));

	m_objectMatrix = matT2*m_objectMatrix*matRot*matT;

	DrawFrame(renderer, frame, x, y);	

	m_objectMatrix.makeIdentity();
}

void CXSprite::DrawFrame(const renderer::IRendererPtr& renderer, int frame, float x, float y)
{	
	renderer->set2DMaterial((renderer::BUILT_IN_MATERIAL_TYPE)m_materialType);

	int fmoduleNum = m_frame_fmoduleNums[ frame ]; // number of frame-modules in this frame

	for ( int fmodule = 0; fmodule < fmoduleNum; fmodule++ )
	{
		DrawFModule(renderer, frame, fmodule, x, y );
	}
}

void CXSprite::DrawAframe(const renderer::IRendererPtr& renderer, int anim, int aframe, float x, float y)
{
	int off   = m_anim_aframeStarts[ anim ] + aframe;
	int frame = m_aframe_frameIndices[ off ];

	float xx = x + m_aframe_offsetXs[off];
	float yy = y + m_aframe_offsetYs[off];

	DrawFrame(renderer, frame, xx, yy);
}

void CXSprite::DrawFModule(const renderer::IRendererPtr& renderer, int frame, int fmodule, float x, float y)
{
	int off      = m_frame_fmoduleStarts[ frame ] + fmodule;
	int fm_flags = m_fmodule_flags[ off ];
	int index    = m_fmodule_moduleIndices[ off ];
	float rotation = m_fmodule_rotations[ off ];	

	float xx = x+ m_fmodule_offsetXs[off];
	float yy = y+ m_fmodule_offsetYs[off];
		
	DrawModule(renderer, index, xx, yy, rotation);	
}

void CXSprite::DrawModule(const renderer::IRendererPtr& renderer, int module, float x, float y, float rotation/*=0.0f*/)
{
	FLT_ASSERT(m_texture.isValid());		

	core::rectf srcRect(m_module_cxs[module], m_module_cys[module], m_module_cwidths[module], m_module_cheights[module]);

	core::matrix4 matFinal;

	if(!core::iszero(rotation))
	{
		core::matrix4 matT, matT2, matRot;
		//rotation center is the module center
		core::vector3df centerOff(x+srcRect.getWidth()/2.0f,y+srcRect.getHeight()/2.0f, 0.0f);		
		matT.setTranslation(-centerOff);
		matT2.setTranslation(centerOff);
		matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,rotation));

		matRot = matT2*matRot*matT;

		matFinal = m_objectMatrix * matRot;
	}
	else
	{
		matFinal = m_objectMatrix;
	}

	renderer->drawImage2D(m_texture, (s32)x, (s32)y, &srcRect, m_useColor?m_colors:0, &matFinal);
}

//Note: the scale center and rotate center of module is the (x,y), please use frame if want to scale/rotate around center
void CXSprite::DrawModuleEx(const renderer::IRendererPtr& renderer, int module, float x, float y, float scalex, float scaley, float angle/*=0.0f*/)
{
	core::matrix4 matFinal;
	core::matrix4 matT, matT2, matRot;
	//rotation center is frame space center
	core::vector3df centerOff(x, y, 0.0f);		
	matT.setTranslation(-centerOff);
	matT2.setTranslation(centerOff);
	m_objectMatrix.setScale(core::vector3df(scalex,scaley,1.0f));
	matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,-angle));

	matFinal = matT2*m_objectMatrix*matRot*matT;	
	
	core::rectf srcRect(m_module_cxs[module], m_module_cys[module], m_module_cwidths[module], m_module_cheights[module]);

	renderer->set2DMaterial((renderer::BUILT_IN_MATERIAL_TYPE)m_materialType);
	renderer->drawImage2D(m_texture, (s32)x, (s32)y, &srcRect, m_useColor?m_colors:0, &matFinal);

	m_objectMatrix.makeIdentity();
}

bool CXSprite::LoadFromFile(const char* fileName)
{	
	SafeReleaseSpriteData();	

	FileStream file;
	ArchiveManager::getInstance().openFile(file, fileName);
	
	if(!file.IsOpen())
		return false;

	DataStream stream(file);
	
	//read modules
	m_moduleNum = stream.ReadUnsignedShort();
	m_module_cxs = new u16[m_moduleNum];
	m_module_cys = new u16[m_moduleNum];
	m_module_cwidths = new u16[m_moduleNum];
	m_module_cheights = new u16[m_moduleNum];
	for(int i=0; i<m_moduleNum; i++)
	{
		m_module_cxs[i] = stream.ReadUnsignedShort();
		m_module_cys[i] = stream.ReadUnsignedShort();
		m_module_cwidths[i] = stream.ReadUnsignedShort();
		m_module_cheights[i] = stream.ReadUnsignedShort();
	}

	//read fmodules
	m_fmoduleNum = stream.ReadUnsignedShort();
	m_fmodule_moduleIndices = new u8[m_fmoduleNum];
	m_fmodule_flags = new u8[m_fmoduleNum];
	m_fmodule_offsetXs = new s16[m_fmoduleNum];
	m_fmodule_offsetYs = new s16[m_fmoduleNum];
	m_fmodule_rotations = new f32[m_fmoduleNum];
	for(int i=0; i<m_fmoduleNum; i++)
	{
		m_fmodule_moduleIndices[i] = stream.ReadUnsignedByte();
		m_fmodule_flags[i] = stream.ReadUnsignedByte();
		m_fmodule_offsetXs[i] = stream.ReadShort();
		m_fmodule_offsetYs[i] = stream.ReadShort();
		m_fmodule_rotations[i] = (f32)stream.ReadShort();
	}

	//read frames
	m_frameNum = stream.ReadUnsignedShort();
	m_frame_fmoduleNums = new u16[m_frameNum];
	m_frame_fmoduleStarts = new u16[m_frameNum];
	for(int i=0; i<m_frameNum; i++)
	{
		m_frame_fmoduleNums[i] = stream.ReadUnsignedShort();
		m_frame_fmoduleStarts[i] = stream.ReadUnsignedShort();
	}

	//read aframes
	m_aframeNum = stream.ReadUnsignedShort();
	m_aframe_frameIndices = new u16[m_aframeNum];
	m_aframe_times = new u8[m_aframeNum];
	m_aframe_flags = new u8[m_aframeNum];
	m_aframe_offsetXs = new s16[m_aframeNum];
	m_aframe_offsetYs = new s16[m_aframeNum];
	for(int i=0; i<m_aframeNum; i++)
	{
		m_aframe_frameIndices[i] = stream.ReadUnsignedShort();
		m_aframe_times[i] = stream.ReadUnsignedByte();
		m_aframe_flags[i] = stream.ReadUnsignedByte();
		m_aframe_offsetXs[i] = stream.ReadShort();
		m_aframe_offsetYs[i] = stream.ReadShort();
	}

	//read anims
	m_animNum = stream.ReadUnsignedShort();
	m_anim_aframeNums = new u16[m_animNum];
	m_anim_aframeStarts = new u16[m_animNum];
	for(int i=0; i<m_animNum; i++)
	{
		m_anim_aframeNums[i] = stream.ReadUnsignedShort();
		m_anim_aframeStarts[i] = stream.ReadUnsignedShort();
	}	

	return true;
}

void CXSprite::CalcAnimFrameRect(int anim, int aframeID, core::rectf& outRect)
{
	core::aabbox3df aabb;	

	int off   = m_anim_aframeStarts[ anim ] + aframeID;
	int frame = m_aframe_frameIndices[ off ];

	float x = m_aframe_offsetXs[off];
	float y = m_aframe_offsetYs[off];

	int fmoduleNum = m_frame_fmoduleNums[ frame ]; // number of frame-modules in this frame

	bool firstPoint = true;

	for ( int fmodule = 0; fmodule < fmoduleNum; fmodule++ )
	{
		int off      = m_frame_fmoduleStarts[ frame ] + fmodule;
		int fm_flags = m_fmodule_flags[ off ];
		int module    = m_fmodule_moduleIndices[ off ];
		float rotation = m_fmodule_rotations[ off ];	

		float xx = x+ m_fmodule_offsetXs[off];
		float yy = y+ m_fmodule_offsetYs[off];

		core::rectf srcRect(m_module_cxs[module], m_module_cys[module], m_module_cwidths[module], m_module_cheights[module]);

		core::matrix4 matFinal;

		if(!core::iszero(rotation))
		{
			core::matrix4 matT, matT2, matRot;
			//rotation center is the module center
			core::vector3df centerOff(xx+srcRect.getWidth()/2.0f,yy+srcRect.getHeight()/2.0f, 0.0f);		
			matT.setTranslation(-centerOff);
			matT2.setTranslation(centerOff);
			matRot.setRotationDegrees(core::vector3df(0.0f,0.0f,rotation));

			matRot = matT2*matRot*matT;

			matFinal = m_objectMatrix * matRot;
		}
		else
		{
			matFinal = m_objectMatrix;
		}

		FLT_ASSERT(m_texture.isValid());

		//compute rect

		f32 quardWidth, quardHeight;

		quardWidth = (f32)m_texture->getOriginalWidth();
		quardHeight = (f32)m_texture->getOriginalHeight();

		
		quardWidth = (quardWidth > srcRect.getWidth()) ? srcRect.getWidth():quardWidth;
		quardHeight =(quardHeight > srcRect.getHeight()) ? srcRect.getHeight():quardHeight;
			

		//core::rectf rect((f32)x, (f32)y, quardWidth, quardHeight);

		f32 m_posF32[8];

		m_posF32[0] = xx;
		m_posF32[1] = yy;

		//v1
		m_posF32[2] = xx;
		m_posF32[3] = yy+quardHeight;	

		//v2
		m_posF32[4] = xx+quardWidth;
		m_posF32[5] = yy;	

		//v3
		m_posF32[6] = xx+quardWidth;
		m_posF32[7] = yy+quardHeight;

		core::vector3df tmpVec;
		
		for(int i=0; i<4; i++)
		{
			tmpVec.set(m_posF32[i*2],m_posF32[i*2+1],0.0f);
			matFinal.transformVect(tmpVec);
			m_posF32[i*2] = tmpVec.X;
			m_posF32[i*2+1] = tmpVec.Y;

			if(firstPoint)
			{
				aabb.reset(m_posF32[i*2],m_posF32[i*2+1],0.0f);
				firstPoint = false;
			}
			else
			{
				aabb.addInternalPoint(m_posF32[i*2],m_posF32[i*2+1],0.0f);
			}
		}

	}

	outRect.UpperLeftCorner.X = aabb.MinEdge.X;
	outRect.UpperLeftCorner.Y = aabb.MinEdge.Y;
	outRect.LowerRightCorner.X = aabb.MaxEdge.X;
	outRect.LowerRightCorner.Y = aabb.MaxEdge.Y;
}

}//namespace flt
