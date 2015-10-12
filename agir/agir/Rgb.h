#pragma once

#include <iostream>

class Rgb
{
public:
	// ctor:s and d:tor
	Rgb() : r(0), g(0), b(0) {}
	Rgb(float f) : r(f), g(f), b(f) {}
	Rgb(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	~Rgb() {};

	// operator overloads
	bool operator != (const Rgb &c) const { return c.r != r || c.g != g || c.b != b; }
	Rgb& operator *= (const Rgb &rgb) { r *= rgb.r, g *= rgb.g, b *= rgb.b; return *this; }
	Rgb& operator += (const Rgb &rgb) { r += rgb.r, g += rgb.g, b += rgb.b; return *this; }
	Rgb& operator += (const float f) { r += f, g += f, b += f; return *this; }					// rgb + float
	friend float& operator += (float &f, const Rgb rgb){ f += (rgb.r + rgb.g + rgb.b) / 3.f; return f; }
	friend std::ostream& operator<< (std::ostream &os, const Rgb& rgb){
		os << "R: " << rgb.r << ", G: " << rgb.g << ", B: " << rgb.b;
	}

	// public methods
	void setRGB(const Rgb& RGB) { r = RGB.r, g = RGB.g, b = RGB.b; }
	void setRGB(float _r, float _g, float _b) { r = _r, g = _g, b = _b; }
	void setRGB(float f) { r = f, g = f, b = f; }

	float getR(){ return r; }
	float getG(){ return g; }
	float getB() { return b; }

private:
	float r, g, b;
};