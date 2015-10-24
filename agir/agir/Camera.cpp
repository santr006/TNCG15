#include "Camera.h"

Camera::Camera(World* w)
{
	theWorld = w;
	position = glm::vec3(0, 0, 0);
	upDirection = glm::vec3(0, 1, 0);
	lookAtDirection = glm::vec3(0, 0, -1);
	nearPlane = 1;
	farPlane = 2;
	widthInPixels = 1;
	heightInPixels = 1;
}

Camera::Camera(World* w, int width, int height)
{
	theWorld = w;
	position = glm::vec3(0, 0, 0);
	upDirection = glm::vec3(0, 1, 0);
	lookAtDirection = glm::vec3(0, 0, -1);
	nearPlane = 1;
	farPlane = 2;
	widthInPixels = width;
	heightInPixels = height;
}

Camera::~Camera(){}

//creates a ray from the position pos in the direction dir
//and returns the color the path of the ray defines
glm::vec3 Camera::generateRay(glm::vec3 pos, glm::vec3 dir)
{
	//create a color: black
	glm::vec3 result(0.f, 0.f, 0.f);

	//Send a ray from pos in the direction dir
	Ray r(pos, dir);

	//did the ray hit anything?

	//checks if the given ray intersects any object in the scene

	//check if the ray hits any bounding box
	//if there are bounding boxes in the scene
	if (theWorld->BBoxList.size() != 0) //check if the ray hits any bounding box
	{
		for (unsigned int i = 0; i < theWorld->BBoxList.size(); i++)
		{
			glm::vec3 boxIntersectionPoint;
			float iterationStep = 0.1f;

			//if it did check the objects inside it
			if (theWorld->BBoxList.at(i)->intersects(r, iterationStep, boxIntersectionPoint))
			{
				//create a new ray from the intersection point
				Ray newRay(boxIntersectionPoint, r.direction);

				//check if the ray hits any of the objects inside the bounding box
				for (unsigned int j = 0; j < theWorld->BBoxList.at(i)->objects.size(); j++)
				{
					//if it did the color will be white, if not the color is still black
					if (theWorld->BBoxList.at(i)->objects.at(j)->testRayIntersection(newRay))
					{
						result = glm::vec3(1.f, 1.f, 1.f); //std::cout << "hit" << std::endl;
					}
					//use object intersection point in a later stage
				}
			}
		}
	}
	else //check if the ray hits any object
	{
		Intersection* closestIntersection = nullptr;

		for (unsigned int o = 0; o < theWorld->objectList.size(); o++)
		{
			Intersection* current = nullptr;
			current = theWorld->objectList.at(o)->testRayIntersection(r);

			if (current != nullptr)
			{
				closestIntersection = current;
			}
		}

		if (closestIntersection != nullptr){
			//result = closestIntersection->color;
			result = generateShadowRay(closestIntersection);
		}
	}
	return result;
}

glm::vec3 Camera::generateShadowRay(Intersection* i)
{
	glm::vec3 dirToLight;

	Light* currentLight;
	Object3D* currentObject;

	bool isObscured = false;

	float localIllumination;
	glm::vec3 resultColor(0.f, 0.f, 0.f);

	// for all light sources
	for (int l = 0; l < theWorld->lightList.size(); l++)
	{
		//get current light source
		currentLight = theWorld->lightList.at(l);
		//get direction to it
		dirToLight = glm::normalize(currentLight->position - i->point);

		//create a ray
		Ray shadowRay(i->point, dirToLight, 0.1f);
		std::cout << "shadow ray pos: " << i->point.x << " " << i->point.y << " " << i->point.z << ", dir: "
			<< dirToLight.x << " " << dirToLight.y << " " << dirToLight.z << std::endl;
		//variable for obscuring object
		Intersection* obscuringIntersection = nullptr;

		//create variable to keep track if obscured
		isObscured = false;

		// go through all objects in the scene
		for (int o = 0; o < theWorld->objectList.size(); o++)
		{
			//get the object
			currentObject = theWorld->objectList.at(o);
			//check if the ray intersects the object
			//returns nullpoiter if the object wasn't hit
			obscuringIntersection = currentObject->testRayIntersection(shadowRay);
			//std::cout << "point " << obscuringIntersection->point.x << " " << obscuringIntersection->point.y << " " << obscuringIntersection->point.z << std::endl;
			std::cout << "hit object after " << shadowRay.tMin << std::endl;

			// if object is hit between surface point and the light source
			if (obscuringIntersection != nullptr){
				isObscured = true;

				//if (shouldRayContinue())
				//{
				//	glm::vec3 newDir = calcRandomReflectionDir(obscuringIntersection->surfaceNormal);
				//	resultColor = generateRay(obscuringIntersection->point, newDir);
				//	//	p += generateRay(closestIntersection->point, newDir);
				//	//	p += generateRay(closestIntersection->point, newDir);
				//	//	p += generateRay(closestIntersection->point, newDir);
				//}

				//resultColor += obscuringIntersection->color;

				// should check all light sources [!!!]

				std::cout << "object is obscuring light source\n";
				return resultColor;
			}
		}

		// if no object is between surface point and light source
		if (!isObscured)
		{
			// calculate the light locally
			//localIllumination = std::max(0.f, glm::dot(i->surfaceNormal, shadowRay.direction)); // diffuse reflection factor
			//resultColor = resultColor + i->color*(currentLight->intensity*localIllumination);

			std::cout << "calculating local light\n";
			resultColor = i->color;
		}
	}
	return resultColor;
}

//uses ray tracing to find a color for every pixel
void Camera::render()
{
	//create the picture to be rendered to
	Image im(widthInPixels, heightInPixels);
	std::cout << "created image" << std::endl;

	//place near cutting plane
	glm::vec3 center = position + lookAtDirection * nearPlane;
	float height = heightInPixels * STEP_BETWEEN_PIXELS;
	float width = widthInPixels * STEP_BETWEEN_PIXELS;
	glm::vec3 rightDir = glm::normalize(glm::cross(lookAtDirection, upDirection));

	glm::vec3 rightUpperCorner = center + height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftUpperCorner = center + height*0.5f * upDirection - width*0.5f * rightDir;
	glm::vec3 rightLowerCorner = center - height*0.5f * upDirection + width*0.5f * rightDir;
	glm::vec3 leftLowerCorner = center - height*0.5f * upDirection - width*0.5f * rightDir;

	std::cout << "rightUpperCorner (" << rightUpperCorner.x << ", " << rightUpperCorner.y << ", " << rightUpperCorner.z << ")" << std::endl;
	std::cout << "leftUpperCorner (" << leftUpperCorner.x << ", " << leftUpperCorner.y << ", " << leftUpperCorner.z << ")" << std::endl;
	std::cout << "rightLowerCorner (" << rightLowerCorner.x << ", " << rightLowerCorner.y << ", " << rightLowerCorner.z << ")" << std::endl;
	std::cout << "leftLowerCorner (" << leftLowerCorner.x << ", " << leftLowerCorner.y << ", " << leftLowerCorner.z << ")" << std::endl;

	//give a color to each pixel
	for (unsigned int i = 0; i < heightInPixels; i++)
	{
		for (unsigned int j = 0; j < widthInPixels; j++)
		{
			//to decide the color for this pixel send a ray from it
			glm::vec3 rayStart(STEP_BETWEEN_PIXELS*0.5f + leftUpperCorner.x + STEP_BETWEEN_PIXELS * j, -STEP_BETWEEN_PIXELS*0.5f + leftUpperCorner.y - STEP_BETWEEN_PIXELS * i, leftUpperCorner.z);
			glm::vec3 rayDir = rayStart - position; //the pixel's position - the cameras position

			//save the colors in the image
			im.setPixel(j, i, generateRay(rayStart, rayDir));
		}
	}

	//save the image to a file
	std::string fileName = "a.ppm";
	im.saveAsPPM(fileName.c_str());
}

glm::vec3 Camera::calcRandomReflectionDir(glm::vec3 sufaceNormal)
{
	//A probailtiy distribution function p(x) returns how likely it is that a value x will happen [0,1]
	//A cumulative distribution function F(x) returns how likely it is that a value less or equal to x will happen [0,1]
	//The curve F will rise quickly at the more important x values and stay around the same value for unimportant x values
	//If y = F(x) then x = F^-1(y)
	//If we choose some y with the same step between them we will most likely get x values where the curve rises
	//which is also very likely x values

	//The x will be the incoming direction of the light
	//If we randomize y we will get a very likely x/direction back

	//theta is rotation around x and phi is rotation around y
	//p(theta, phi) = pi^-1 * cos(theta) 
	//F(theta, phi) = pi^-1 * integral 0 to phi (dPhi) * integral 0 to theta (cos(theta)sin(theta)*dTheta) = phi / 2pi * (1 - cos^2(theta))
	//We split F into 2 parts and reverse them to get the values for theta and phi
	//Ftheta = 1 - cos^2(theta) => theta = cos^-1(sqrt(1 - Ftheta))
	//Fphi = phi / 2pi => phi = 2 * pi * Fphi

	//Get theta and phi
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float theta = glm::acos(glm::sqrt(random));
	random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float phi = 2 * PI * random;

	//create direction vector and rotate it asuming (0, 1, 0) is the normal
	glm::mat4 rotTheta = glm::rotate(glm::mat4(1.f), theta, glm::vec3(1, 0, 0)); //around x
	glm::mat4 rotPhi = glm::rotate(glm::mat4(1.f), phi, glm::vec3(0, 1, 0)); // around y
	glm::vec4 direction4(0, 1, 0, 1); //start at the normal
	direction4 = rotPhi * rotTheta * direction4;

	//rotate the vector so the direction becomes correct using the real normal
	//we want to find the theta and phi that will rotate our normal to the real normal
	//and use them to rotate the direction vector
	//we use Pythagora's

	//Find theta and phi
	float hyp = glm::sqrt(sufaceNormal.z * sufaceNormal.z + sufaceNormal.x * sufaceNormal.x);
	phi = glm::asin(sufaceNormal.x / hyp);
	theta = glm::asin(hyp / glm::length(sufaceNormal)); //always gives positive angle [0,pi]
	if (sufaceNormal.y < 0) //if y is negative the angle should be larger
		theta = 180 - theta;
	if (sufaceNormal.z < 0) //if z is negative the angle should be negative
		theta = -theta;

	//rotate
	rotTheta = glm::rotate(glm::mat4(1.f), theta, glm::vec3(1, 0, 0)); //around x
	rotPhi = glm::rotate(glm::mat4(1.f), phi, glm::vec3(0, 1, 0)); // around y
	direction4 = rotPhi * rotTheta * direction4;

	return glm::vec3(direction4.x, direction4.y, direction4.z);
}

bool Camera::shouldRayContinue()
{
	//should be russian roulette later
	if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < 0.5f)
		return true;
	return false;
}