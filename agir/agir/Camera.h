/******************************************************
  A class for the a camera that will render an image
  of the scene using ray tracing  
 ******************************************************/

#pragma once

#include <glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "Image.h"
#include "Ray.h"
#include "World.h"
#include "Object3D.h"

#include <cmath>

//the distance between two pixels in world coordinates
const float STEP_BETWEEN_PIXELS = 0.005f;

class Camera
{
public:
	Camera(World* w);
	Camera(World* w, int width, int height);
	~Camera();
	
	//uses ray tracing to find a color for every pixel
	void render();	

	//creates a ray from the position pos in the direction dir
	//and returns the color the path of the ray defines
	glm::vec3 generateRay(glm::vec3 pos, glm::vec3 dir);
	glm::vec3 generateShadowRay(Intersection* i);
	//glm::vec3 phongShading(Object3D* o, Light* l,Intersection* i);

	glm::vec3 calcRandomReflectionDir(glm::vec3 surfaceNormal);
	bool shouldRayContinue();

	World* theWorld; // pointer to the world that will be rendered
	glm::vec3 position; //position of the observer
	glm::vec3 upDirection; //the direction that is up for the camera, must be normalized
	glm::vec3 lookAtDirection; //the direction that is forward for the camera, must be normalized
	float nearPlane; //the distance from the camera in the forward direction to the near cutting plane
	float farPlane; //the distance from the camera in the forward direction to the far cutting plane
	unsigned int widthInPixels; //the number of pixels left to right in the image that will be rendered
	unsigned int heightInPixels; //the number of pixels up to down in the image that will be rendered
};