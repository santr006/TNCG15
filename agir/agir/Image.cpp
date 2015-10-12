#include "Image.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>

void Image::setPixel(unsigned int &x, unsigned int &y, glm::vec3 &color)
{
	pixelValues[(x + y*width) * 3] = color.r;
	pixelValues[(x + y*width) * 3 + 1] = color.g;
	pixelValues[(x + y*width) * 3 + 2] = color.b;
}

void Image::fillImage(glm::vec3 &color){

	for (unsigned int y = 0; y < height; y++){
		for (unsigned int x = 0; x < width; x++){
			setPixel(x, y, color);
		}
	}
}

void Image::saveAsPPM(const char *filename)
{
	std::cout << "Beginning render" << std::endl;
	if (width == 0 || height == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }

	std::ofstream outFile;

	try {
		outFile.open(filename, std::ios::binary);					// Need to spec. binary mode for Windows
		if (outFile.fail()) throw("Can't open output file");
		outFile << "P6\n" << width << " " << height << "\n255\n";	// Specific header for PPM images
		float r, g, b;

		float* currentPixel = pixelValues;



		// loop over each pixel in the image, clamp from 0 to 255, convert to char format and write to file. 

		for (unsigned int h = 0; h < height; ++h){
			for (unsigned int w = 0; w < width; ++w){

				r = std::max(0.0f, roundf(std::min(1.f, currentPixel[0]) * 255));
				g = std::max(0.0f, roundf(std::min(1.f, currentPixel[1]) * 255));
				b = std::max(0.0f, roundf(std::min(1.f, currentPixel[2]) * 255));

				outFile << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
				//std::cout << "Color at " << w << ":" << h << " = " << r << ":" << g  << ":" << b << std::endl;

				currentPixel += 3;
			}
		}
		outFile.close();
		std::cout << "Successful render" << std::endl;
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		outFile.close();
	}
}
