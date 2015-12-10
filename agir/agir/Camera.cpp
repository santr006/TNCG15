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
	glm::vec3 p(0, 0, 0);

	//Send a ray from pos in the direction dir
	Ray r(pos, dir);

	//did the ray hit anything?

	//checks if the given ray intersects any object in the scene

	//check if the ray hits any bounding box
	//if there are bounding boxes in the scene
	if (theWorld->BBoxList.size() != 0) //check if the ray hits any bounding box
	{
		//At the moment we don't use bounding boxes

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
					Intersection* object = theWorld->BBoxList.at(i)->objects.at(j)->testRayIntersection(newRay); 
					if (object != nullptr)
					{
						p = glm::vec3(1.f, 1.f, 1.f); //std::cout << "hit" << std::endl;
						delete object;
					}
					//use object intersection point in a later stage
				}
			}
		}
	}
	else //check if the ray hits any object
	{
		Intersection* closestIntersection = nullptr;

		//find the closest intersection point
		for (unsigned int j = 0; j < theWorld->objectList.size(); j++)
		{
			Intersection* current = nullptr;
			//std::cout << "testing if camera see this object, color " << theWorld->objectList.at(j)->color.x << " " << theWorld->objectList.at(j)->color.y << " " << theWorld->objectList.at(j)->color.z << std::endl;
			current = theWorld->objectList.at(j)->testRayIntersection(r);

			if (current != nullptr)
			{
				if (closestIntersection == nullptr || current->distance < closestIntersection->distance)
				{
					//delete old closest intersection
					delete closestIntersection;
					closestIntersection = current;
					//std::cout << "it did" << std::endl;
				}
				else //delete current
					delete current;
			}
			else
				;// std::cout << "it didn't" << std::endl;
		}

		//if an intersection was found
		if (closestIntersection != nullptr)
		{
			if (!closestIntersection->light)
			{
				//std::cout << "sees object with color " << closestIntersection->color.x << " " << closestIntersection->color.y << " " << closestIntersection->color.z << std::endl;
				//Find the radiance (L) for this point
				//L(x -> out) = pi * fr(x, Win, Wout) * L(x <- Win)
				//fr comes from the BRDF (can ba a constant)
				//L(x <- Win) needs to be calculated by sending a ray in the direction Win
				//and find the radiance of the point it hits
				//but only if the ray shouldn't be terminated
				glm::vec3 incomingRadiance(0, 0, 0);
				float probabilityToContinue = 0.1f;

				//If the object is transparent it is a perfect reflector and creates a refracted ray
				/*if (closestIntersection->transparent)
				{
					//calculate the perfectly reflected ray
					//glm::vec3 newDir = calcPerfectReflectionDir(r.direction, closestIntersection->surfaceNormal);
					
					//calculate the perfectly refracted ray from air to glass
					glm::vec3 newDir = calcPerfectRefractionDir(r.direction, closestIntersection->surfaceNormal, 1.f, 1.5f);

					//calculate point where the ray leaves the object
					Ray newRay(closestIntersection->point, newDir);
					//Intersection* leavingPoint = closestIntersection->object->testRayIntersectionInside(newRay);

					//calculate the perfectly refracted ray from glass to air
					newDir = calcPerfectRefractionDir(newRay.direction, leavingPoint->surfaceNormal, 1.5f, 1.f);

					//reflection inside the glass?

					//add them to the incoming radiance with the appropriate importance
					incomingRadiance = generateRay(leavingPoint->point, newDir);
				}
				else
				{*/
					//Should we send a new ray in a random direction
					//or end the path here and send shadow rays to the light sources to see if the point is lit?
					glm::vec3 newDir = calcRandomReflectionDir(closestIntersection->surfaceNormal, probabilityToContinue);
					//returns (0, 0, 0) if the ray shouldn't continue
					//determined by Russian roulette

					if (newDir.x != 0 || newDir.y != 0 || newDir.z != 0) //a direction can't be 0
					{
						//send a ray in the random direction newDir
						incomingRadiance = generateRay(closestIntersection->point, newDir);
						//std::cout << "new dir " << newDir.x << " " << newDir.y << " " << newDir.z << std::endl;
						//std::cout << "bonus color " << incomingRadiance.x << " " << incomingRadiance.y << " " << incomingRadiance.z << std::endl;
						//it's okay if the ray didn't hit anything

						/*if (incomingRadiance == glm::vec3(0.f))
							//send shadow rays
							incomingRadiance = closestIntersection->color;**/
					}
				//}

///				//Send shadow rays to find the color of this object if light reaches it
				glm::vec3 selfIllumination(0, 0, 0);

				for (int l = 0; l < theWorld->lightList.size(); l++)
				{
					//get current light source
					Light* currentLight = theWorld->lightList.at(l);

					if (!currentLight->areaLight)
					{
						//handle point light
						selfIllumination += handlePointLightSource(currentLight, closestIntersection, theWorld);
						//std::cout << "selfIllumination: " << selfIllumination.x << " " << selfIllumination.y << " " << selfIllumination.z << std::endl;
					}
					else
					{
						//handle area light
						selfIllumination += handleAreaLightSource(currentLight, closestIntersection, theWorld);
					}
///
				}

				//The BRDF for a Lambertian reflector is a constant reflection coeficient / pi
				float fr = closestIntersection->reflectionCoef / PI;
				//???????Whould it be better to calculated the BRDF at object?????

				//the division by probabilityToContinue is added because of Russian roulette in clacRandomReflectionDir
				p = selfIllumination + PI / probabilityToContinue * fr * incomingRadiance;
			}
			//if the ray's start position is in the camera position this ray sees light sources
			else if (pos == position)
				p = closestIntersection->color;

			delete closestIntersection;
		}
	}

	//std::cout << "color for ray " << p.x << " " << p.y << " " << p.z << std::endl;
	return p;
}

//uses ray tracing to find a color for every pixel
void Camera::render()
{
	//create the picture to be rendered to
	Image im(widthInPixels, heightInPixels);
	std::cout << "created image" << std::endl;

	float stepBetweenRays = STEP_BETWEEN_PIXELS / RAY_FACTOR_PER_PIXEL;

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
			//to decide the color for this pixel send a number of rays from it
			glm::vec3 color(0, 0, 0);
			for (int k = 0; k < RAY_FACTOR_PER_PIXEL * RAY_FACTOR_PER_PIXEL; k++)
			{
				glm::vec3 rayWillGoThrough(stepBetweenRays*0.5f + leftUpperCorner.x + STEP_BETWEEN_PIXELS * j, -stepBetweenRays*0.5f + leftUpperCorner.y - STEP_BETWEEN_PIXELS * i, leftUpperCorner.z);
				glm::vec3 rayDir = glm::normalize(rayWillGoThrough - position); //the pixel's position - the cameras position

				//std::cout << "setting pixel in (" << rayStart.x << ", " << rayStart.y << ", " << rayStart.z << ")" << std::endl;
				//std::cout << "dir (" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << ")" << std::endl;

				color += generateRay(position, rayDir);
			}
			//std::cout << "color " << color.x << " " << color.y << " " << color.z << std::endl;
			//save the colors in the image
			im.setPixel(j, i, color / (RAY_FACTOR_PER_PIXEL * RAY_FACTOR_PER_PIXEL));
		}
		if ((i+1)%20 == 0)
			std::cout << (float)(i + 1)/heightInPixels * 100.f << std::endl;
	}

	//save the image to a file
	std::string fileName = "b.ppm";
	im.saveAsPPM(fileName.c_str());
}

glm::vec3 Camera::calcRandomReflectionDir(glm::vec3 surfaceNormal, float probabilityOfSuccess)
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

	//We need to determine if the ray should continue of end at this surface
	//We do this with Russian roulette
	//The function F can give results [0,1]
	//We will rescale the function and make that span smaller by dividing our phi with a value P [0, 1]
	//If phi then becomes larger than 2 pi the ray doesn't continue
	//We decided P to be the variable probabilityOfSuccess sent into the function
	//P = 0.1 means 1/10 chance that a ray will be reflected. When a ray get's reflected it will have a 10 times higher importance
	phi = phi / probabilityOfSuccess;
	if (phi > 2*PI)
	{
		//std::cout << "died " << theta << std::endl;
		return glm::vec3(0, 0, 0);
	}
	//std::cout << "theta: " << theta << " phi: " << phi << std::endl;

	//create direction vector and rotate it assuming (0, 1, 0) is the normal
	glm::mat4 rotTheta = glm::rotate(glm::mat4(1.f), theta, glm::vec3(1, 0, 0)); //around x
	glm::mat4 rotPhi = glm::rotate(glm::mat4(1.f), phi, glm::vec3(0, 1, 0)); // around y
	glm::vec4 direction4(0, 1, 0, 1); //start at the normal
	direction4 = rotPhi * rotTheta * direction4;
	//std::cout << "new dir local: " << direction4.x << " " << direction4.y << " " << direction4.z << std::endl;

	//define local coordinate system by taking the cross product of the normal and the y-axis
	//then cross that vector with the normal to get three axises
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 y = glm::normalize(surfaceNormal);
	glm::vec3 x = glm::normalize(glm::cross(y, up));
	glm::vec3 z = glm::normalize(glm::cross(x, y));

	//create a translation matrix to transform the random direction from local coordinates to world coordinates
	glm::mat4 trans(x.x, x.y, x.z, 0.f, //coloumn wise
					y.x, y.y, y.z, 0.f, 
					z.x, z.y, z.z, 0.f, 
					0.f, 0.f, 0.f, 1.f);
	direction4 = trans * direction4;
	//std::cout << "new dir world: " << direction4.x << " " << direction4.y << " " << direction4.z << std::endl;



	//rotate the vector so the direction becomes correct using the real normal
	//we want to find the theta and phi that will rotate our normal to the real normal
	//and use them to rotate the direction vector
	//we use Pythagora's

	/*//Find theta and phi
	float hyp = glm::sqrt(sufaceNormal.z * sufaceNormal.z + sufaceNormal.x * sufaceNormal.x);
	phi = glm::asin(glm::abs(sufaceNormal.x) / hyp);
	if (sufaceNormal.x > 0)
	{
		if (sufaceNormal.z < 0)
			phi = PI + phi;
		else
			phi = 2 * PI - phi;
	}
	else
	{
		if (sufaceNormal.z < 0)
			phi = PI - phi;
	}

	theta = glm::asin(hyp / glm::length(sufaceNormal)); //always gives positive angle [0,pi/2]

	//rotate
	rotTheta = glm::rotate(glm::mat4(1.f), theta, glm::vec3(1, 0, 0)); //around x
	rotPhi = glm::rotate(glm::mat4(1.f), phi, glm::vec3(0, 1, 0)); // around y
	direction4 = rotPhi * rotTheta * direction4;*/

	return glm::normalize(glm::vec3(direction4.x, direction4.y, direction4.z));

}

glm::vec3 Camera::handlePointLightSource(Light* currentLight, Intersection* closestIntersection, World* theWorld)
{
	bool isObscured = false;

	//get direction to the light
	glm::vec3 dirToLight = glm::normalize(currentLight->position - closestIntersection->point);

	//create a ray to the light source
	Ray shadowRay(closestIntersection->point, dirToLight, true);
	/*std::cout << "shadow ray pos: " << closestIntersection->point.x << " " << closestIntersection->point.y << " " << closestIntersection->point.z << ", dir: "
	<< dirToLight.x << " " << dirToLight.y << " " << dirToLight.z << std::endl;*/

	//variable to store eventual obscuring object
	Intersection* obscuringIntersection = nullptr;

	//check t-value until the light
	Intersection* theLight = currentLight->testRayIntersection(shadowRay);

	//if the ray didn't intersect the light the light can't shine in this direction
	if (theLight == nullptr)
		return glm::vec3(0.f);

	// go through all objects in the scene
	for (int o = 0; o < theWorld->objectList.size(); o++)
	{
		//get the object
		Object3D* currentObject = theWorld->objectList.at(o);

		//check if the shadow ray intersects the object
		//returns nullpoiter if the object wasn't hit
		//std::cout << "testing if anything is in the way for the light source" << std::endl;
		obscuringIntersection = currentObject->testRayIntersection(shadowRay);

		//std::cout << "point " << obscuringIntersection->point.x << " " << obscuringIntersection->point.y << " " << obscuringIntersection->point.z << std::endl;
		//std::cout << "hit object after " << shadowRay.tMin << std::endl;

		// if object is hit
		if (obscuringIntersection != nullptr)
		{
			//if the obscuring object is behind the light source it is not obscuring
			if (obscuringIntersection->distance < theLight->distance)
			{
				isObscured = true;
				delete obscuringIntersection;
				//std::cout << "object is obscuring light source, has color " << obscuringIntersection->color.x << " " << obscuringIntersection->color.y << " " << obscuringIntersection->color.z << std::endl;
				break;
			}
			delete obscuringIntersection;
		}
	}
	delete theLight;

	if (!isObscured)
		return closestIntersection->color * glm::max(0.f, glm::dot(closestIntersection->surfaceNormal, shadowRay.direction));
	return glm::vec3(0.f);
}

glm::vec3 Camera::handleAreaLightSource(Light* currentLight, Intersection* closestIntersection, World* theWorld)
{
	//std::cout << "area light source in scene" << std::endl;
	//when we have an area light source, objects can get hit by many light rays from different points on the light's surface
	//we need to find points that give us a good representation of the light source
	//and send shadow rays too all these points					

	glm::vec3 radianceFromThisLightSource(0.f);
	int noOfShadowRays = 5;
	for (int r = 0; r < noOfShadowRays; r++)
	{
		bool isObscured = false;

		//find random point on the plane
		glm::vec3 pointOnLight = currentLight->randomPointOnAreaLight();

		//create a ray from the object point to the point on the light source
		Ray shadowRay(closestIntersection->point, pointOnLight - closestIntersection->point, true);
		//std::cout << "ray dir " << shadowRay.direction.x << " " << shadowRay.direction.y << " " << shadowRay.direction.z << std::endl;

		//send a shadow ray along the ray
		//variable to store eventual obscuring object
		Intersection* obscuringIntersection = nullptr;

		//check t-value until the light
		Intersection* theLight = currentLight->testRayIntersection(shadowRay);

		//if the ray didn't intersect the light the light can't shine in this direction
		if (theLight == nullptr)
			break;

		// go through all objects in the scene
		for (int o = 0; o < theWorld->objectList.size(); o++)
		{
			//get the object
			Object3D* currentObject = theWorld->objectList.at(o);

			//check if the shadow ray intersects the object
			//returns nullpoiter if the object wasn't hit
			//std::cout << "testing if anything is in the way for the light source" << std::endl;
			obscuringIntersection = currentObject->testRayIntersection(shadowRay);

			//std::cout << "point " << obscuringIntersection->point.x << " " << obscuringIntersection->point.y << " " << obscuringIntersection->point.z << std::endl;
			//std::cout << "hit object after " << shadowRay.tMin << std::endl;

			// if object is hit
			if (obscuringIntersection != nullptr)
			{
				//if the obscuring object is behind the light source it is not obscuring
				if (obscuringIntersection->distance < theLight->distance)
				{
					isObscured = true;
					delete obscuringIntersection;
					//std::cout << "object is obscuring light source, has color " << obscuringIntersection->color.x << " " << obscuringIntersection->color.y << " " << obscuringIntersection->color.z << std::endl;
					break;
				}
				delete obscuringIntersection;
			}
		}
		delete theLight;

		//sum the radiance
		if (!isObscured)
		{
			radianceFromThisLightSource += closestIntersection->color * glm::max(0.f, glm::dot(closestIntersection->surfaceNormal, shadowRay.direction));
		}
	}

	//get the average of the radiance
	return radianceFromThisLightSource / (float)noOfShadowRays;
}

glm::vec3 Camera::calcPerfectReflectionDir(glm::vec3 incomingDir, glm::vec3 surfaceNormal){
	//The new direction is with the same angle from the normal
	//which mean that if the incoming direction is rotated wiht pi around the normal
	//we get the negative new direction

	glm::vec4 newDir = glm::rotate(glm::mat4(1.f), PI, surfaceNormal) * glm::vec4(incomingDir, 1);

	return glm::normalize(glm::vec3(-newDir.x, -newDir.y, -newDir.z));
}

glm::vec3 Camera::calcPerfectRefractionDir(glm::vec3 incomingDir, glm::vec3 surfaceNormal, float n1, float n2){
	//snell's law: sin theta1 / sin theta2 = n2 / n1
	//theta1 is the angle between the incoming direction and the surface normal which both have lenght 1
	float cosTheta1 = glm::dot(surfaceNormal, -incomingDir);
	float theta1 = glm::acos(cosTheta1);

	//if theta1 is too large and n1 > n2 no ray is refracted
	if (n1 * sin(theta1) / n2 > 1) // theta1 is too large
		return glm::vec3(0.f);

	//theta2 is the angle between the negative normal and the refracted direction
	float theta2 = glm::asin(n1 * glm::sin(theta1) / n2);

	//rotate around incomingDir x surfaceNormal
	//angle: PI - theta1 + theta2
	glm::vec4 newDir = glm::rotate(glm::mat4(1.f), PI - theta1 + theta2, glm::cross(incomingDir, surfaceNormal)) * glm::vec4(incomingDir, 1);
	
	return glm::normalize(glm::vec3(-newDir.x, -newDir.y, -newDir.z));
}