#pragma once

#include <windows.h>
#include <iostream>

namespace nc
{
	struct Color
	{
		float r, g, b;

		Color() : r{ 0 }, g{ 0 }, b{ 0 } {}
		Color(float r, float g, float b) : r{ r }, g{ g }, b{ b } {}

		float& operator [] (size_t index) { return (&r)[index]; }
		const float& operator [] (size_t index) const { return (&r)[index]; }

		Color operator + (const Color& c) const { return { r + c.r, g + c.g, b + c.b }; }
		Color operator - (const Color& c) const { return { r - c.r, g - c.g, b - c.b }; }
		Color operator * (const Color& c) const { return { r * c.r, g * c.g, b * c.b }; }
		Color operator / (const Color& c) const { return { r / c.r, g / c.g, b / c.b }; }

		Color operator + (float s) const { return { r + s, g + s, b + s }; }
		Color operator - (float s) const { return { r - s, g - s, b - s }; }
		Color operator * (float s) const { return { r * s, g * s, b * s }; }
		Color operator / (float s) const { return { r / s, g / s, b / s }; }

		Color& operator += (const Color& c) { r += c.r; g += c.g; b += c.b; return *this; }
		Color& operator -= (const Color& c) { r -= c.r; g -= c.g; b -= c.b; return *this; }
		Color& operator *= (const Color& c) { r *= c.r; g *= c.g; b *= c.b; return *this; }
		Color& operator /= (const Color& c) { r /= c.r; g /= c.g; b /= c.b; return *this; }

		Color& operator += (float s) { r += s; g += s; b += s; return *this; }
		Color& operator -= (float s) { r -= s; g -= s; b -= s; return *this; }
		Color& operator *= (float s) { r *= s; g *= s; b *= s; return *this; }
		Color& operator /= (float s) { r /= s; g /= s; b /= s; return *this; }

		friend std::istream& operator >> (std::istream& stream, Color& v);

		COLORREF Pack888() const;
		operator COLORREF() const { return Pack888(); }

		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;
	};

	inline COLORREF Color::Pack888() const
	{
		// BYTE = 1 byte | 8 bits
		// WORD = 2 bytes | 16 bits
		// DWORD = 4 bytes | 32 bits
		BYTE _r = static_cast<BYTE>(r * 255.0f); // (0 - 1) -> (0 - 255)
		BYTE _g = static_cast<BYTE>(g * 255.0f);
		BYTE _b = static_cast<BYTE>(b * 255.0f);

		return (_r | (_g << 8) | (_b << 16)); // DWORD( x | b | g | r )
	}
}
