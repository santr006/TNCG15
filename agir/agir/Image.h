#pragma once

#include "Rgb.h"

const Rgb BLACK(0.0f);
const Rgb WHITE(1.0f);
const Rgb RED(1.0f, 0.0f, 0.0f);
const Rgb GREEN(0.0f, 1.0f, 0.0f);
const Rgb BLUE(0.0f, 0.0f, 1.0f);
const Rgb STAR_WARS_COLOR(229 / 225.f, 177 / 255.f, 58 / 255.f);

class Image
{
public:
	// ctors and dtor
	Image() : width(640), height(360) { pixelValues = new float[3 *width*height]; } // default image
	Image(const unsigned int &w, const unsigned int &h) : width(w), height(h){ pixelValues = new float[3 * w*h]; }
	~Image() { delete[] pixelValues; }

	// public methods
	void setPixel(unsigned int &x, unsigned int &y, Rgb* color);
	void fillImage(Rgb* color); // mostly used for testing
	void saveAsPPM(const char *filename);

	static const Rgb BLACK, WHITE, RED, GREEN, BLUE, STAR_WARS_COLOR;  // preset colors

private:

	float *pixelValues;
	unsigned int width, height; // image resolution
};