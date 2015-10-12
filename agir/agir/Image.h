#pragma once

#include <glm.hpp>

class Image
{
public:
	// ctors and dtor
	Image() : width(640), height(360) { pixelValues = new float[3 * width*height]; } // default image
	Image(const unsigned int &w, const unsigned int &h) : width(w), height(h){ pixelValues = new float[3*w*h]; }
	~Image() { delete[] pixelValues; }

	// public methods
	void setPixel(unsigned int &x, unsigned int &y, glm::vec3 &color);
	void fillImage(glm::vec3 &color); // mostly used for testing
	void saveAsPPM(const char *filename);

	//static Rgb BLACK(0.0f); //WHITE, RED, GREEN, BLUE, STAR_WARS_COLOR;  // preset colors
private:

	float *pixelValues;
	unsigned int width, height; // image resolution
};