#ifndef FLT_COLOR_H
#define FLT_COLOR_H

#include "common.h"
#include "core/fltMath.h"

namespace flt
{	
namespace renderer
{

class Color
{
public:
	u8 A,R,G,B;
	
	Color() {}

	//! Constructs the color from 4 values representing the alpha, red, green and blue component.
	/** Must be values between 0 and 255. */
	Color (u8 a, u8 r, u8 g, u8 b)
		:A(a),R(r),G(g),B(b)
	{}

	Color (const Color& rhs)
	{
		A = rhs.A;
		R = rhs.R;
		G = rhs.G;
		B = rhs.B;
	}

	void set(u8 a, u8 r, u8 g, u8 b)
	{
		A = a;
		R = r;
		G = g;
		B = b;
	}
	
	//! Returns the luminance of the color.
	f32 getLuminance() const
	{
		return 0.3f*R + 0.59f*G + 0.11f*B;
	}

	//! Returns the average intensity of the color.
	u32 getAverage() const
	{
		return (u8)( (u32)R + G + B ) / 3;
	}								

	//! Converts color to 32 bit 8888 RGBA color (GL)		

	void toRGBA32(u8* dest) const
	{
		*dest =  R;
		*++dest =  G;
		*++dest =  B;
		*++dest =  A;
	}

	u32 toRGBA32() const
	{
		u32 data;
		toRGBA32((u8*)&data);
		return data;
	};

	//! Converts color to 32 bit 8888 BGRA color (DX)

	void toBGRA32(u8* dest) const
	{
		*dest =  B;
		*++dest =  G;
		*++dest =  R;
		*++dest =  A;
	}

	u32 toBGRA32() const
	{
		u32 data;
		toBGRA32((u8*)&data);
		return data;
	};

	

	//! Compares the color to another color.
	/** \return True if the colors are the same, and false if not. */		
	bool operator==(const Color& other) const 
	{ 
		return (*(u32*)this) == (*(u32*)&other); 
	}

	//! Compares the color to another color.
	/** \return True if the colors are different, and false if they are the same. */
	bool operator!=(const Color& other) const 
	{ 
		return (*(u32*)this) != (*(u32*)&other); 
	}
	

	//! Adds two colors, result is clamped to 0..255 values
	/** \param other Color to add to this color
	\return Addition of the two colors, clamped to 0..255 values */
	Color operator+(const Color& other) const
	{
		return Color(flt::core::min_((u32)A + other.A, (u32)255u),
			flt::core::min_((u32)R + other.R, (u32)255u),
			flt::core::min_((u32)G + other.G, (u32)255u),
			flt::core::min_((u32)B + other.B, (u32)255u));
	}

	//! Interpolates the color with a f32 value to another color
	/** \param other: Other color
	\param d: value between 0.0f and 1.0f
	\return Interpolated color. */		
	Color getInterpolated(const Color &other, f32 d) const
	{
		d = core::clamp(d, 0.f, 1.f);
		return Color((u8)flt::core::clamp<f32>(flt::core::lerp<f32>(A, other.A, d), (f32)0.0f, (f32)255.0f ),
			(u8)flt::core::clamp<f32>(flt::core::lerp<f32>(R, other.R, d), (f32)0.0f, (f32)255.0f ),
			(u8)flt::core::clamp<f32>(flt::core::lerp<f32>(G, other.G, d), (f32)0.0f, (f32)255.0f ),
			(u8)flt::core::clamp<f32>(flt::core::lerp<f32>(B, other.B, d), (f32)0.0f, (f32)255.0f ));
	}

	//! Returns interpolated color. ( quadratic )
	/** \param c1: first color to interpolate with
	\param c2: second color to interpolate with
	\param d: value between 0.0f and 1.0f. */
	Color getInterpolated_quadratic(const Color& c1, const Color& c2, f32 d) const
	{
		// this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
		d = core::clamp(d, 0.f, 1.f);
		const f32 inv = 1.f - d;
		const f32 mul0 = inv * inv;
		const f32 mul1 = 2.f * d * inv;
		const f32 mul2 = d * d;

		return Color(core::clamp<s32>(core::floor32(A * mul0 + c1.A * mul1 + c2.A * mul2 ), 
			(s32)0, (s32)255 ),
			core::clamp<s32>(core::floor32(R * mul0 + c1.R * mul1 + c2.R * mul2 ), 
			(s32)0, (s32)255 ),
			core::clamp<s32>(core::floor32(G * mul0 + c1.G * mul1 + c2.G * mul2 ), 
			(s32)0, (s32)255 ),
			core::clamp<s32>(core::floor32(B * mul0 + c1.B * mul1 + c2.B * mul2 ), 
			(s32)0, (s32)255 ));
	}

};

//! Class representing a color with four floats.
/** The color values for red, green, blue
and alpha are each stored in a 32 bit floating point variable.
So all four values may be between 0.0f and 1.0f.
Another, faster way to define colors is using the class SColor, which
stores the color values in a single 32 bit integer.
*/
class Colorf
{
public:
	//! Default constructor for SColorf.
	/** Sets red, green and blue to 0.0f and alpha to 1.0f. */
	Colorf() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

	//! Constructs a color from up to four color values: red, green, blue, and alpha.
	/** \param r: Red color component. Should be a value between
	0.0f meaning no red and 1.0f, meaning full red.
	\param g: Green color component. Should be a value between 0.0f
	meaning no green and 1.0f, meaning full green.
	\param b: Blue color component. Should be a value between 0.0f
	meaning no blue and 1.0f, meaning full blue.
	\param a: Alpha color component of the color. The alpha
	component defines how transparent a color should be. Has to be
	a value between 0.0f and 1.0f, 1.0f means not transparent
	(opaque), 0.0f means fully transparent. */
	Colorf(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

	//! Constructs a color from 32 bit Color.
	/** \param c: 32 bit color from which this SColorf class is
	constructed from. */
	Colorf(Color c)
	{
		const f32 inv = 1.0f / 255.0f;
		r = c.R * inv;
		g = c.G * inv;
		b = c.B * inv;
		a = c.A * inv;
	}

	Colorf& operator=(const Colorf& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		a = rhs.a;

		return *this;
	}

	//! Converts this color to a SColor without floats.
	Color toColor() const
	{
		return Color((u32)(a*255.0f), (u32)(r*255.0f), (u32)(g*255.0f), (u32)(b*255.0f));
	}

	bool operator==(const Colorf& other) const 
	{ 
		return core::equals(r,other.r) && core::equals(g,other.g) && core::equals(b,other.b) && core::equals(a,other.a) ;
	}

	bool operator!=(const Colorf& other) const 
	{ 
		return !core::equals(r,other.r) || !core::equals(g,other.g) || !core::equals(b,other.b) || !core::equals(a,other.a) ;
	}

	//! Sets three color components to new values at once.
	/** \param rr: Red color component. Should be a value between 0.0f meaning
	no red (=black) and 1.0f, meaning full red.
	\param gg: Green color component. Should be a value between 0.0f meaning
	no green (=black) and 1.0f, meaning full green.
	\param bb: Blue color component. Should be a value between 0.0f meaning
	no blue (=black) and 1.0f, meaning full blue. */
	void set(f32 rr, f32 gg, f32 bb) {r = rr; g =gg; b = bb; }

	//! Sets all four color components to new values at once.
	/** \param aa: Alpha component. Should be a value between 0.0f meaning
	fully transparent and 1.0f, meaning opaque.
	\param rr: Red color component. Should be a value between 0.0f meaning
	no red and 1.0f, meaning full red.
	\param gg: Green color component. Should be a value between 0.0f meaning
	no green and 1.0f, meaning full green.
	\param bb: Blue color component. Should be a value between 0.0f meaning
	no blue and 1.0f, meaning full blue. */
	void set(f32 aa, f32 rr, f32 gg, f32 bb) {a = aa; r = rr; g =gg; b = bb; }

	//! Interpolates the color with a f32 value to another color
	/** \param other: Other color
	\param d: value between 0.0f and 1.0f
	\return Interpolated color. */
	Colorf getInterpolated(const Colorf &other, f32 d) const
	{
		d = core::clamp(d, 0.f, 1.f);
		const f32 inv = 1.0f - d;
		return Colorf(other.r*inv + r*d,
			other.g*inv + g*d, other.b*inv + b*d, other.a*inv + a*d);
	}

	//! Returns interpolated color. ( quadratic )
	/** \param c1: first color to interpolate with
	\param c2: second color to interpolate with
	\param d: value between 0.0f and 1.0f. */
	inline Colorf getInterpolated_quadratic(const Colorf& c1, const Colorf& c2,
		f32 d) const
	{
		d = core::clamp(d, 0.f, 1.f);
		// this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
		const f32 inv = 1.f - d;
		const f32 mul0 = inv * inv;
		const f32 mul1 = 2.f * d * inv;
		const f32 mul2 = d * d;

		return Colorf (r * mul0 + c1.r * mul1 + c2.r * mul2,
			g * mul0 + c1.g * mul1 + c2.g * mul2,
			g * mul0 + c1.b * mul1 + c2.b * mul2,
			a * mul0 + c1.a * mul1 + c2.a * mul2);
	}


	//! Sets a color component by index. R=0, G=1, B=2, A=3
	void setColorComponentValue(s32 index, f32 value)
	{
		switch(index)
		{
		case 0: r = value; break;
		case 1: g = value; break;
		case 2: b = value; break;
		case 3: a = value; break;
		}
	}

public:

	//! red color component
	f32 r;

	//! green color component
	f32 g;

	//! blue component
	f32 b;

	//! alpha color component
	f32 a;
};

//! global const colors
extern const Color ColorWhite;
extern const Color ColorBlack;
extern const Color ColorRed;
extern const Color ColorGreen;
extern const Color ColorBlue;

} //end namespace renderer
} //end namespace flt

#endif //FLT_COLOR_H