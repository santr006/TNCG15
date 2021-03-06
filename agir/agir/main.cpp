#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <iomanip>
#include <cmath>

#include "Image.h"
#include "Camera.h"
#include "Box3D.h"
#include "Object3D.h"
#include "Sphere3D.h"
#include "Plane3D.h"

using namespace std;

int main(int argc, char **argv)
{
	World world;
	int width = 400, height = 400;

	Object3D* objectToAdd;
	BoundingBox* bboxToAdd;
	Light* lightToAdd;

	// Add lights to the world

	lightToAdd = new Light(glm::vec3(0.49999f, 0.0f, -2.5f), glm::vec3(1.f), 1.f, true, glm::vec2(0.2f), glm::vec3(0.f, PI / 2, 0.f));
	world.addLight(lightToAdd);
	world.objectList.push_back(lightToAdd);
	lightToAdd = new Light(glm::vec3(0.0f, 0.49999f, -2.5f), glm::vec3(1.f), 1.f, true, glm::vec2(0.2f), glm::vec3(-PI / 2, 0.f, 0.f));
	world.addLight(lightToAdd);
	world.objectList.push_back(lightToAdd);
	//lightToAdd = new Light(glm::vec3(1.f, 1.0f, 0.f), glm::vec3(1.f), 1.f, true, glm::vec2(0.2f), glm::vec3(0.f, PI / 2, 0.f));
	//world.addLight(lightToAdd);


	//to show shadows
	/*objectToAdd = new Box3D(glm::vec3(0.f, 0.f, -2.f), glm::vec3(PI / 6, -3*PI / 4, 0.f), glm::vec3(0.3f), glm::vec3(0.f, 0.3f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);*/
	//objectToAdd = new Sphere3D(glm::vec3(0.f, 0.f, -2.5f), glm::vec3(0.0f), 0.18f, glm::vec3(0.3f, 0.3f, 0.3f));
	//world.objectList.push_back(objectToAdd);	
	/*objectToAdd = new Plane3D(glm::vec3(0.35f, 0.f, -1.3f), glm::vec3(0.f, 0.f, PI / 4), glm::vec2(0.2f), glm::vec3(1.f, 1.f, 1.f));
	world.objectList.push_back(objectToAdd);*/

	//walls
	objectToAdd = new Plane3D(glm::vec3(0.f, 0.0f, -3.f), glm::vec3(0.0f), glm::vec2(1.f), glm::vec3(0.f, 0.f, 0.5f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Plane3D(glm::vec3(0.5f, 0.f, -2.5f), glm::vec3(0.0f, PI / 2, 0.f), glm::vec2(1.f), glm::vec3(0.f, 0.5f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Plane3D(glm::vec3(-0.5f, 0.f, -2.5f), glm::vec3(0.0f, -PI / 2, 0.f), glm::vec2(1.f), glm::vec3(0.f, 0.5f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Plane3D(glm::vec3(0.f, 0.5f, -2.5f), glm::vec3(-PI / 2, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.5f, 0.f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Plane3D(glm::vec3(0.f, -0.5f, -2.5f), glm::vec3(PI / 2, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.5f, 0.f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);
	
	//objects
	objectToAdd = new Sphere3D(glm::vec3(-0.2f, -0.3f, -2.3f), glm::vec3(0.f), 0.2f, glm::vec3(0.8f, 0.f, 0.f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Sphere3D(glm::vec3(-0.3f, 0.2f, -2.8f), glm::vec3(0.0f), 0.08f, glm::vec3(0.0f, 0.4f, 0.8f), 0.8f);
	world.objectList.push_back(objectToAdd);
	objectToAdd = new Sphere3D(glm::vec3(0.3f, 0.3f, -2.5f), glm::vec3(0.0f), 0.1f, glm::vec3(0.0f, 0.8f, 0.8f), 0.8f);
	world.objectList.push_back(objectToAdd);

	// Add a camera to the world

	Camera cam(&world, width, height); // create a camera with default constructor -> at (0,0,0)

	srand(time(0));

	cam.render();

	// return to OS

	system("pause");

	return 0;
}