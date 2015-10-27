/******************************************************
 * A class for the a camera that will render an image *
 * of the scene using ray tracing					  *
 ******************************************************/

#pragma once

#include <glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "Image.h"
#include "Ray.h"
#include "World.h"
#include "Object3D.h"

#include <cmath>

//the distance between two pixels in world coordinates
const float STEP_BETWEEN_PIXELS = 0.0025f;
const float RAY_FACTOR_PER_PIXEL = 2.f;

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

	//calculates a new direction for a ray that hit a surface
	//determines with Russian roulette if the ray should continue
	//returns no directoin = (0, 0, 0) if the ray shouldn't continue
	glm::vec3 calcRandomReflectionDir(glm::vec3 surfaceNormal, float probabilityOfSuccess);


	glm::vec3 handlePointLightSource(Light* currentLight, Intersection* closestIntersection, World* theWorld);
	glm::vec3 handleAreaLightSource(Light* currentLight, Intersection* closestIntersection, World* theWorld);


	World* theWorld; // pointer to the world that will be rendered
	glm::vec3 position; //position of the observer
	glm::vec3 upDirection; //the direction that is up for the camera, must be normalized
	glm::vec3 lookAtDirection; //the direction that is forward for the camera, must be normalized
	float nearPlane; //the distance from the camera in the forward direction to the near cutting plane
	float farPlane; //the distance from the camera in the forward direction to the far cutting plane
	unsigned int widthInPixels; //the number of pixels left to right in the image that will be rendered
	unsigned int heightInPixels; //the number of pixels up to down in the image that will be rendered
};
