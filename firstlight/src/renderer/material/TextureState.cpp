#include "TextureState.h"

namespace flt
{
namespace renderer
{

TextureState::TextureState()
	:WrapModeU(TEXW_REPEAT),
	 WrapModeV(TEXW_REPEAT),
	 //BorderColor(0.0f,0.0f,0.0f,0.0f),
	 MinFilterMode(TEXF_BILINEAR),
	 MagFilterMode(TEXF_BILINEAR),
	 EnvMode(TEXENV_REPLACE),
	 EnvBlendColor(0.0f,0.0f,0.0f,1.0f),
	 CombineFuncRGB(TEXCBF_MODULATE),
	 CombineFuncAlpha(TEXCBF_MODULATE),
	 CombineSrc0RGB(TEXCBS_TEXTURE),
	 CombineSrc1RGB(TEXCBS_PREVIOUS),
	 CombineSrc2RGB(TEXCBS_CONSTANT),
	 CombineSrc0Alpha(TEXCBS_TEXTURE),
	 CombineSrc1Alpha(TEXCBS_PREVIOUS),
	 CombineSrc2Alpha(TEXCBS_CONSTANT),
	 CombineOp0RGB(TEXCBOP_SRC_COLOR),
	 CombineOp1RGB(TEXCBOP_SRC_COLOR),
	 CombineOp2RGB(TEXCBOP_SRC_ALPHA),
	 CombineOp0Alpha(TEXCBOP_SRC_ALPHA),
	 CombineOp1Alpha(TEXCBOP_SRC_ALPHA),
	 CombineOp2Alpha(TEXCBOP_SRC_ALPHA),
	 CombineRGBScale(1.0f),
	 CombineAlphaScale(1.0f)
{
}

bool TextureState::operator!=(const TextureState& rhs) const
{
	if(this==&rhs)
		return false;

	return WrapModeU != rhs.WrapModeU ||
	 WrapModeV != rhs.WrapModeV ||
	 MinFilterMode != rhs.MinFilterMode  ||
	 MagFilterMode!=rhs.MagFilterMode||
	 EnvMode!=rhs.EnvMode||
	 EnvBlendColor!=rhs.EnvBlendColor||
	 CombineFuncRGB!=rhs.CombineFuncRGB||
	 CombineFuncAlpha!=rhs.CombineFuncAlpha||
	 CombineSrc0RGB!=rhs.CombineSrc0RGB||
	 CombineSrc1RGB!=rhs.CombineSrc1RGB||
	 CombineSrc2RGB!=rhs.CombineSrc2RGB||
	 CombineSrc0Alpha!=rhs.CombineSrc0Alpha||
	 CombineSrc1Alpha!=rhs.CombineSrc1Alpha||
	 CombineSrc2Alpha!=rhs.CombineSrc2Alpha||
	 CombineOp0RGB!=rhs.CombineOp0RGB||
	 CombineOp1RGB!=rhs.CombineOp1RGB||
	 CombineOp2RGB!=rhs.CombineOp2RGB||
	 CombineOp0Alpha!=rhs.CombineOp0Alpha||
	 CombineOp1Alpha!=rhs.CombineOp1Alpha||
	 CombineOp2Alpha!=rhs.CombineOp2Alpha||
	 CombineRGBScale!=rhs.CombineRGBScale||
	 CombineAlphaScale!=rhs.CombineAlphaScale;

}

}// end namespace renderer
}// end namespace flt
