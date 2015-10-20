#pragma once

#include "Object3D.h"

class Plane3D : public Object3D
{
public:
	Plane3D(glm::mat4 w2l, glm::vec4 c)
	{
		worldToLocal = w2l, color = c;
		center = glm::vec3(worldToLocal[3]);
	}
	~Plane3D(){};

private:
};