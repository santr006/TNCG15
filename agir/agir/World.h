#pragma once

#include <vector>
#include "Object3D.h"
#include "Light.h"

class World
{
public:
	// c-tors and d-tors
	World();
	~World();

	// public methods
	void addObject3D(Object3D* o);
	void addLight(Light* l);
	
private:
	std::vector<Object3D*> objectList;
	std::vector<Light*> lightList;
};