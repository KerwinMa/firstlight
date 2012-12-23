
#ifndef FLT_COLOR_CONVERTER_H
#define FLT_COLOR_CONVERTER_H

#include "common.h"
#include "renderer/image/Image.h"

namespace flt
{

class ColorConverter
{
public:

	//! Creates a 16 bit A1R5G5B5 color
	static inline u16 RGBA16(u32 r, u32 g, u32 b, u32 a=0xFF)
	{
		return ((a & 0x80) << 8 |
			(r & 0xF8) << 7 |
			(g & 0xF8) << 2 |
			(b & 0xF8) >> 3);
	}


	//! Creates a 16 bit A1R5G5B5 color
	static inline u16 RGB16(u32 r, u32 g, u32 b)
	{
		return RGBA16(r,g,b, 0xFF);
	}


	//! Converts a 32bit (X8R8G8B8) color to a 16bit A1R5G5B5 color
	static inline u16 X8R8G8B8toA1R5G5B5(u32 color)
	{
		return (0x8000 |
			( color & 0x00F80000) >> 9 |
			( color & 0x0000F800) >> 6 |
			( color & 0x000000F8) >> 3);
	}


	static inline u16 A1R5G5B5(u8 r, u8 g, u8 b)
	{
		return (0x8000 | (u16)r >> 9 | (u16)g >> 6 | (u16)b >> 3);
	}


	//! Converts a 32bit (A8R8G8B8) color to a 16bit A1R5G5B5 color
	static inline u16 A8R8G8B8toA1R5G5B5(u32 color)
	{
		return (( color & 0x80000000) >> 16|
			( color & 0x00F80000) >> 9 |
			( color & 0x0000F800) >> 6 |
			( color & 0x000000F8) >> 3);
	}

	static inline u16 A1R5G5B5(u8 a, u8 r, u8 g, u8 b)
	{
		return ( a ? 0x8000 : 0 | (u16)r >> 9 | (u16)g >> 6 | (u16)b >> 3);
	}


	//! Converts a 32bit (A8R8G8B8) color to a 16bit R5G6B5 color
	static inline u16 A8R8G8B8toR5G6B5(u32 color)
	{
		return (( color & 0x00F80000) >> 8 |
			( color & 0x0000FC00) >> 5 |
			( color & 0x000000F8) >> 3);
	}

	//! Create a 16bit R5G6B5 color
	static inline u16 R5G6B5(u8 r, u8 g, u8 b)
	{
		return ((u16)r >> 8 | (u16)g >> 5 | (u16)b >> 3);
	}


	//! Convert A8R8G8B8 Color from A1R5G5B5 color
	/** build a nicer 32bit Color by extending dest lower bits with source high bits. */
	static inline u32 A1R5G5B5toA8R8G8B8(u16 color)
	{
		return ( (( -( (s32) color & 0x00008000 ) >> (s32) 31 ) & 0xFF000000 ) |
			(( color & 0x00007C00 ) << 9) | (( color & 0x00007000 ) << 4) |
			(( color & 0x000003E0 ) << 6) | (( color & 0x00000380 ) << 1) |
			(( color & 0x0000001F ) << 3) | (( color & 0x0000001C ) >> 2)
			);
	}


	//! Returns A8R8G8B8 Color from R5G6B5 color
	static inline u32 R5G6B5toA8R8G8B8(u16 color)
	{
		return 0xFF000000 |
			((color & 0xF800) << 8)|
			((color & 0x07E0) << 5)|
			((color & 0x001F) << 3);
	}


	//! Returns A1R5G5B5 Color from R5G6B5 color
	static inline u16 R5G6B5toA1R5G5B5(u16 color)
	{
		return 0x8000 | (((color & 0xFFC0) >> 1) | (color & 0x1F));
	}


	//! Returns R5G6B5 Color from A1R5G5B5 color
	static inline u16 A1R5G5B5toR5G6B5(u16 color)
	{
		return (((color & 0x7FE0) << 1) | (color & 0x1F));
	}


	//! Returns the alpha component from A1R5G5B5 color
	static inline u8 getA1R5G5BAlpha(u16 color)
	{
		return ((color >> 15)&0x1);
	}


	//! Returns the red component from A1R5G5B5 color.
	/** Shift left by 3 to get 8 bit value. */
	static inline u8 getA1R5G5BRed(u16 color)
	{
		return ((color >> 10)&0x1F);
	}


	//! Returns the green component from A1R5G5B5 color
	/** Shift left by 3 to get 8 bit value. */
	static inline u8 getA1R5G5BGreen(u16 color)
	{
		return ((color >> 5)&0x1F);
	}


	//! Returns the blue component from A1R5G5B5 color
	/** Shift left by 3 to get 8 bit value. */
	static inline u8 getA1R5G5BBlue(u16 color)
	{
		return (color & 0x1F);
	}


	//! Returns the average from a 16 bit A1R5G5B5 color
	static inline u8 getA1R5G5BAverage(s16 color)
	{
		return ((getA1R5G5BRed(color)<<3) + (getA1R5G5BGreen(color)<<3) + (getA1R5G5BBlue(color)<<3)) / 3;
	}


	//! converts a monochrome bitmap to A1R5G5B5
	static void convert1BitTo16Bit(const u8* in, s16* out, s32 width, s32 height, s32 linepad=0, bool flip=false);

	//! converts a 4 bit palettized image to A1R5G5B5
	static void convert4BitTo16Bit(const u8* in, s16* out, s32 width, s32 height, const s32* palette, s32 linepad=0, bool flip=false);

	//! converts a 8 bit palettized image to A1R5G5B5
	static void convert8BitTo16Bit(const u8* in, s16* out, s32 width, s32 height, const s32* palette, s32 linepad=0, bool flip=false);

	//! converts R8G8B8 16 bit data to A1R5G5B5 data
	static void convert16BitTo16Bit(const s16* in, s16* out, s32 width, s32 height, s32 linepad=0, bool flip=false);

	//! copies R8G8B8 24 bit data to 24 data, and flips and 
	//! mirrors the image during the process.
	static void convert24BitTo24Bit(const u8* in, u8* out, s32 width, s32 height, s32 linepad=0, bool flip=false, bool bgr=false);

	//! Resizes the surface to a new size and converts it at the same time
	//! to an A8R8G8B8 format, returning the pointer to the new buffer.
	static void convert16bitToA8R8G8B8andResize(const s16* in, s32* out, s32 newWidth, s32 newHeight, s32 currentWidth, s32 currentHeight);
	
	//! copies X8R8G8B8 32 bit data, and flips and 
	//! mirrors the image during the process.
	static void convert32BitTo32Bit(const s32* in, s32* out, s32 width, s32 height, s32 linepad, bool flip=false);


	//! functions for converting one image format to another efficiently
	//! and hopefully correctly.
	//!
	//! \param sP pointer to source pixel data
	//! \param sN number of source pixels to copy
	//! \param dP pointer to destination data buffer. must be big enough
	//! to hold sN pixels in the output format.
	static void convert_A1R5G5B5toR8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_A1R5G5B5toB8G8R8(const void* sP, s32 sN, void* dP);
	static void convert_A1R5G5B5toA8R8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_A1R5G5B5toA1R5G5B5(const void* sP, s32 sN, void* dP);
	static void convert_A1R5G5B5toR5G6B5(const void* sP, s32 sN, void* dP);

	static void convert_A8R8G8B8toR8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_A8R8G8B8toB8G8R8(const void* sP, s32 sN, void* dP);
	static void convert_A8R8G8B8toA8R8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_A8R8G8B8toA1R5G5B5(const void* sP, s32 sN, void* dP);
	static void convert_A8R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP);
	static void convert_A8R8G8B8toR3G3B2(const void* sP, s32 sN, void* dP);

	static void convert_R8G8B8toR8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_R8G8B8toA8R8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_R8G8B8toA1R5G5B5(const void* sP, s32 sN, void* dP);
	static void convert_R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP);

	static void convert_R5G6B5toR5G6B5(const void* sP, s32 sN, void* dP);
	static void convert_R5G6B5toR8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_R5G6B5toB8G8R8(const void* sP, s32 sN, void* dP);
	static void convert_R5G6B5toA8R8G8B8(const void* sP, s32 sN, void* dP);
	static void convert_R5G6B5toA1R5G5B5(const void* sP, s32 sN, void* dP);
	//static void convert_viaFormat(const void* sP, ECOLOR_FORMAT sF, s32 sN,
	//			void* dP, ECOLOR_FORMAT dF);
};


} // end namespace flt

#endif

