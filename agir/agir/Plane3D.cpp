#include "Plane3D.h"


Plane3D::Plane3D(glm::vec3 pos, glm::vec3 rot, glm::vec2 dim, glm::vec3 col)
{
	position = pos;
	//rotation = rot;
	dimensions = dim;
	color = col;
}


Plane3D::~Plane3D()
{

}
