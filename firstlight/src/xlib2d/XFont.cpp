#include "XFont.h"
#include "firstlight.h" //temp,make compile ok

namespace flt
{


CXFont::CXFont()
	:CXSprite(),
	 m_charSpacing(0),m_lineSpacing(0),m_fontHeight(0),
	 m_scaleX(1.0f),m_scaleY(1.0f),m_angle(0.0f)
{	 
	m_materialType = renderer::BIMT_2D_TEXTURE_ALPHA_BLEND_MODULATE_COLOR;
}

CXFont::~CXFont()
{

}

int CXFont::MapCharToModule(char c)
{
	//now, ASCII only
	int frame = c - ' ';
	return frame;
}

int CXFont::GetFontHeight() 
{ 
	if(m_fontHeight==0)
	{		
		m_fontHeight = m_module_cheights[0];
	}

	return m_fontHeight;
}

int CXFont::GetLineHeight() 
{ 
	if(m_fontHeight==0)
		GetFontHeight();

	return m_fontHeight+m_lineSpacing ; 
}

void CXFont::DrawTextLine(const renderer::IRendererPtr& renderer, const char* text, int x, int y, int align)
{
	renderer->set2DMaterial((renderer::BUILT_IN_MATERIAL_TYPE)m_materialType);	

	if((align&ALIGN_BOTTOM)!=0)
		y-=(int)(GetLineHeight()*m_scaleY);
	else if((align&ALIGN_VCENTER)!=0)
		y-=(int)((GetLineHeight()*m_scaleY/2.0f));

	const char* p = text;
	int len = strlen(text);
	int sx = x;
	while(len-->0)
	{
		char c = *(p++);
		if(c==' ')
		{
			x += (m_module_cwidths[0]*m_scaleX + m_charSpacing);
			continue;
		}
		else if(c=='\n')
		{
			y+= GetLineHeight()*m_scaleY;
			x = sx;
			continue;
		}
		else if(c=='\t')
		{
			x += 4*(m_module_cwidths[0]*m_scaleX+m_charSpacing);
			continue;
		}		

		int module = MapCharToModule(c);			
		
		if(m_scaleX!=1.0f || m_scaleY!=1.0f || m_angle!=0.0f)
			DrawModuleEx(renderer, module, x, y, m_scaleX, m_scaleY, m_angle);
		else
			DrawModule(renderer, module, x, y);
		
		int w = m_module_cwidths[module]*m_scaleX + m_charSpacing;
		x+=w;		
	}		
}

}//namespace flt
