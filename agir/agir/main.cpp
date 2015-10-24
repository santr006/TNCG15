#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <iomanip>
#include <time.h>

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
	int width = 1, height = 1;

	Object3D* objectToAdd;
	BoundingBox* bboxToAdd;
	Light* lightToAdd;

	// colors
	glm::vec3 colorWall(0.8f);

	// Add objects to the world

	//back
	objectToAdd = new Plane3D(glm::vec3(0.f, 0.f, -2.5f), glm::vec3(0.f), glm::vec2(1.f), glm::vec3(1.f));
	world.objectList.push_back(objectToAdd);

	//objectToAdd = new Sphere3D(glm::vec3(0.f, 0.6f, -1.5f), glm::vec3(0.f), 0.3f, glm::vec3(1.f,0.f,0.f));
	//world.objectList.push_back(objectToAdd);


/*	//right
	objectToAdd = new Plane3D(glm::vec3(0.5f, 0.f, -2.f), glm::vec3(0.f,PI/2, 0.f), glm::vec2(1.f), glm::vec3(1.f));
	world.objectList.push_back(objectToAdd);
	//left
	objectToAdd = new Plane3D(glm::vec3(-0.5f, 0.f, -2.f), glm::vec3(0.f, PI / 2, 0.f), glm::vec2(1.f), glm::vec3(1.f));
	world.objectList.push_back(objectToAdd);
	//upper
	objectToAdd = new Plane3D(glm::vec3(0.f, 0.5f, -2.f), glm::vec3(PI / 2,0.f , 0.f), glm::vec2(1.f), glm::vec3(1.f));
	world.objectList.push_back(objectToAdd);
	//lower
	objectToAdd = new Plane3D(glm::vec3(0.f, -0.5f, -2.f), glm::vec3(PI / 2, 0.f, 0.f), glm::vec2(1.f), glm::vec3(1.f));
	world.objectList.push_back(objectToAdd);

	*/

	// Add lights to the world

	lightToAdd = new Light(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f));
	world.lightList.push_back(lightToAdd);

	// Add a camera to the world

	Camera cam(&world, width, height); // create a camera with default constructor -> at (0,0,0)
	time_t start = time(0);
	cam.render();

	double renderTime = difftime(time(0), start);

	std::cout << "Render time: " << renderTime << " s." << std::endl;

	// return to OS

	system("pause");
	return 0;
}