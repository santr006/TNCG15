#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <iomanip>

#include "Image.h"
#include "Camera.h"
#include "Box3D.h"
#include "Object3D.h"
#include "Sphere3D.h"

using namespace std;

int main(int argc, char **argv)
{
	World world;
	int width = 1280, height = 720;

	Object3D* objectToAdd;
	BoundingBox* bboxToAdd;
	Light* lightToAdd;

	// Add geometry to the world

	objectToAdd = new Sphere3D(glm::vec3(0.f, 0.f, -1.5f), glm::vec2(0.0f, 0.0f), 0.3f, glm::vec3(1.f, 0.f, 0.f));
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Sphere3D(glm::vec3(1.f, 1.f, -2.f), glm::vec2(0.0f, 0.0f), 0.1f, glm::vec3(0.f, 1.f, 0.f));
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Sphere3D(glm::vec3(1.f, 2.f, 9.8f), glm::vec2(0.0f, 0.0f), 0.9f, glm::vec3(0.f, 1.f, 0.f));
	world.objectList.push_back(objectToAdd);

	//objectToAdd = new Sphere3D(glm::vec3(0.5f, 0.3f, -3.f), glm::vec2(0.0f, 0.0f), 1.f, glm::vec3(0.f, 0.f, 1.f));
	//world.objectList.push_back(objectToAdd);

	// Add lights to the world

	lightToAdd = new Light(glm::vec3(1.f,2.f,10.f), glm::vec3(1.f));
	world.lightList.push_back(lightToAdd);

	// Add a camera to the world

	Camera cam(&world, width, height); // create a camera with default constructor -> at (0,0,0)
	cam.render();

	// return to OS

	system("pause");
	return 0;
}