#ifndef FLT_TEXTURE_STATE_H
#define FLT_TEXTURE_STATE_H

#include "renderer/RendererDefs.h"
#include "renderer/Color.h"

namespace flt
{	
namespace renderer
{	

class TextureState
{
public:		
	TextureState();
	~TextureState(){};

	bool operator!=(const TextureState& rhs) const;

public:

	TEX_WRAP_MODE WrapModeU;
	TEX_WRAP_MODE WrapModeV;
	//Colorf BorderColor;

	TEX_FILTER_MODE MinFilterMode, MagFilterMode;		

	TEX_ENV_MODE EnvMode;
	Colorf EnvBlendColor;

	//Texture Combine
	TEX_COMBINE_FUNC CombineFuncRGB;
	TEX_COMBINE_FUNC CombineFuncAlpha;
	TEX_COMBINE_SOURCE CombineSrc0RGB;
	TEX_COMBINE_SOURCE CombineSrc1RGB;
	TEX_COMBINE_SOURCE CombineSrc2RGB;
	TEX_COMBINE_SOURCE CombineSrc0Alpha;
	TEX_COMBINE_SOURCE CombineSrc1Alpha;
	TEX_COMBINE_SOURCE CombineSrc2Alpha;
	TEX_COMBINE_OP CombineOp0RGB;
	TEX_COMBINE_OP CombineOp1RGB;
	TEX_COMBINE_OP CombineOp2RGB;
	TEX_COMBINE_OP CombineOp0Alpha;
	TEX_COMBINE_OP CombineOp1Alpha;
	TEX_COMBINE_OP CombineOp2Alpha;
	f32 CombineRGBScale;
	f32 CombineAlphaScale;

};


} //end namespace renderer
} //end namespace flt

#endif //FLT_TEXTURE_STATE_H