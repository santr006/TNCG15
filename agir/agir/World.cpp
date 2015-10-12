#include "World.h"

World::~World()
{
	lightList.erase(lightList.begin(), lightList.end());
	lightList.clear();

	objectList.erase(objectList.begin(), objectList.end());
	objectList.clear();
}

void World::addObject3D(Object3D* o)
{
	objectList.push_back(o);
}

void World::addLight(Light* l)
{
	lightList.push_back(l);
}