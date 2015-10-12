#pragma once

#include <vector>
#include "Object3D.h"
#include "Light.h"
#include "BoundingBox.h"

class World
{
public:
	// c-tors and d-tors
	World();
	~World();

	// public methods
	void addObject3D(Object3D* o);
	void addLight(Light* l);

	std::vector<Object3D*> objectList;
	std::vector<Light*> lightList;
	std::vector<BoundingBox*> BBoxList;
};