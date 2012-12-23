#include "TextureLayer.h"

namespace flt
{
namespace renderer
{


Texture::Texture()
	:m_hasMipMaps(false),
	 m_hasAlphaChannel(false),
	 m_width(0), 
	 m_height(0),
	 m_originalWidth(0),
	 m_originalHeight(0)
{
}

Texture::~Texture()
{
}

TextureLayer::TextureLayer()
{

}

TextureLayer::~TextureLayer()
{

}

TextureLayer& TextureLayer::operator=( const TextureLayer& rhs )
{
	if(this==&rhs)
		return *this;

	m_texture = rhs.m_texture;

	m_texState = rhs.m_texState;

	return *this;
}

bool TextureLayer::operator!=(const TextureLayer& rhs) const
{
	if(this==&rhs)
		return false;

	return m_texture != rhs.m_texture || m_texState != rhs.m_texState;
}

}// end namespace renderer
}// end namespace flt