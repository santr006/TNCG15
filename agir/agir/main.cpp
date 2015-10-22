#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <iomanip>

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
	int width = 200, height = 200;

	Object3D* objectToAdd;
	BoundingBox* bboxToAdd;
	Light* lightToAdd;

	//objectToAdd = new Sphere3D(glm::vec3(0.f, 0.f, -1.5f), glm::vec2(0.0f, 0.0f), 0.25f, glm::vec3(0.5f, 0.5f, 0.f));
	//world.objectList.push_back(objectToAdd);
	//objectToAdd = new Box3D(glm::vec3(0.f, 0.f, -1.5f), glm::vec2(0.79f, 0.f), glm::vec3(-0.2f, -0.2f, -1.7f), glm::vec3(1.f));
	//world.objectList.push_back(objectToAdd);
	objectToAdd = new Plane3D(glm::vec3(0.f, 0.f, -1.5f), glm::vec2(0.5f, 0.5f), glm::vec2(1.f), glm::vec3(0.f, 1.f, 0.f));
	world.objectList.push_back(objectToAdd);

	//lightToAdd = new Light(glm::vec3(0.f), glm::vec3(1.f), 1.f);
	//world.addLight(lightToAdd);

	Camera cam(&world, width, height); // create a camera with default constructor -> at (0,0,0)
	cam.render();

	/*
	std::cout << "Rendering test" << std::endl;
	// Image render test
	Image im(640, 480);
	glm::vec3 color(1.0f, 0.0f, 0.0f);
	im.fillImage(color);

	//Create camera
	Camera cam();

	//Render image
	//cam.render();

	//im.fillImage(color);
	string fileName = "render.ppm";
	im.saveAsPPM(fileName.c_str() );

	// End of rendering

	//  Return to OS

	*/
	system("pause");
	return 0;
}