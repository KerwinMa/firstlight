
#ifndef __XFONT_H__
#define __XFONT_H__

#include "xlib2d/XSprite.h"

namespace flt
{

enum FONT_ALIGN
{
	ALIGN_LEFT		= 0x00,
	ALIGN_HCENTER	= 0x01,
	ALIGN_RIGHT		= 0x02,
	ALIGN_TOP		= 0x00,
	ALIGN_VCENTER	= 0x10,
	ALIGN_BOTTOM	= 0x20,
	ALIGN_HVCENTER	= 0x11
};

FLT_FORWARD_PTR(CXFont)

class CXFont: public CXSprite
{
public:
	CXFont();
	~CXFont();

	void SetCharSpacing(int charSpacing) { m_charSpacing = charSpacing; }
	void SetLineSpacing(int lingSpacing) { m_lineSpacing = lingSpacing; }	
	void DrawTextLine(const renderer::IRendererPtr& renderer, const char* text, int x, int y, int align=ALIGN_LEFT|ALIGN_TOP);
	//void DrawTextMultiLine();

	int GetFontHeight();
	int GetLineHeight();

	void SetScale(float scaleX, float scaleY)
	{
		m_scaleX = scaleX;
		m_scaleY = scaleY;
	}

	void SetAngle(float angle)
	{
		m_angle = angle;
	}


private:
	int m_fontHeight;	//Height of SPACE
	int m_charSpacing;
	int m_lineSpacing;

	float m_scaleX, m_scaleY;
	float m_angle;

	int MapCharToModule(char c);	

	
};

}//namespace flt

#endif //__XFONT_H__
