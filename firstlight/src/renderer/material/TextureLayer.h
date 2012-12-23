#ifndef FLT_TEXTURE_LAYER_H
#define FLT_TEXTURE_LAYER_H

#include "BaseObject.h"
#include "sharePtr.h"
#include "TextureState.h"
#include "stream/DataStream.h"

namespace flt
{
namespace renderer
{	

FLT_FORWARD_PTR(Texture)

class Texture: public BaseObject
{
public:
	Texture();
	virtual ~Texture();

	virtual void create(bool holdImage=false) {}

	//virtual bool isValid(){ return false; }	

	u32 getWidth() { return m_width; }

	u32 getHeight() { return m_height; }

	u32 getOriginalWidth() { return m_originalWidth; }

	u32 getOriginalHeight() { return m_originalHeight; }

	bool hasAligned() { return m_width!=m_originalWidth || m_height!=m_originalHeight; }

	void getMaxUVOfAligned(f32 &u, f32 &v)
	{
		u = (f32)m_originalWidth/m_width;
		v = (f32)m_originalHeight/m_height;
	}

	bool hasAlphaChannel() { return m_hasAlphaChannel; }

	bool hasMipMaps() { return m_hasMipMaps; }

protected:
	u32 m_width;
	u32 m_height;
	u32 m_originalWidth;
	u32 m_originalHeight;
	bool m_hasAlphaChannel;
	bool m_hasMipMaps;
};

class TextureLayer
{
public:
	TextureLayer();
	~TextureLayer();

	TextureLayer& operator=( const TextureLayer& rhs );

	bool operator!=(const TextureLayer& rhs) const;

public:
	TexturePtr m_texture;
	TextureState m_texState;

	TextureLayer(const TextureLayer& rhs);
};

} //end namespace renderer
} //end namespace flt

#endif //FLT_TEXTURE_LAYER_H