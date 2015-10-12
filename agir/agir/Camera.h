/******************************************************
 * A class for the a camera that will render an image *
 * of the scene using ray tracing					  *
 ******************************************************/


#pragma once
#include <glm.hpp>
#include <vector>
#include "Rgb.h"
#include "Image.h"
#include "Ray.h"

//the distance between two pixels in world coordinates
const float STEP_BETWEEN_PIXELS = 0.005;

class Camera
{
public:
	Camera();
	~Camera();
	
	//uses ray tracing to find a color for every pixel
	void render();	

	//creates a ray from the position pos in the direction dir
	//and returns the color the path of the ray defines
	Rgb generateRay(glm::vec3 pos, glm::vec3 dir);

	
	glm::vec3 position; //position of the observer
	glm::vec3 upDirection; //the direction that is up for the camera, must be normalized
	glm::vec3 lookAtDirection; //the direction that is forward for the camera, must be normalized
	float nearPlane; //the distance from the camera in the forward direction to the near cutting plane
	float farPlane; //the distance from the camera in the forward direction to the far cutting plane
	unsigned int widthInPixels; //the number of pixels left to right in the image that will be rendered
	unsigned int heightInPixels; //the number of pixels up to down in the image that will be rendered
};
