#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <iomanip>
#include <glm.hpp>

#include "Image.h"
#include "Camera.h"
#include "Object3D.h"
#include "Sphere3D.h"

using namespace std;

int main(int argc, char **argv)
{	
	World* world = new World();

	//Object3D* o = new Sphere3D(glm::translate<float>(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f), 3.f);
	world->addObject3D(o);

	//  Return to OS
	system("pause");
	return 0;
}