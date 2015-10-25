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
						p = glm::vec3(1.f, 1.f, 1.f); //std::cout << "hit" << std::endl;
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
					closestIntersection = current;
					//std::cout << "it did" << std::endl;
				}
			}
			else
				;// std::cout << "it didn't" << std::endl;
		}

		//if an intersection was found
		if (closestIntersection != nullptr)
		{
			//std::cout << "sees object with color " << closestIntersection->color.x << " " << closestIntersection->color.y << " " << closestIntersection->color.z << std::endl;
			//Find the radiance (L) for this point
			//L(x -> out) = pi * fr(x, Win, Wout) * L(x <- Win)
			//fr comes from the BRDF (can ba a constant)
			//L(x <- Win) needs to be calculated by sending a ray in the direction Win
			//and find the radiance of the point it hits
			//but only if the ray shouldn't be terminated
			glm::vec3 incomingRadiance(0, 0, 0);

			//Should we send a new ray in a random direction
			//or end the path here and send shadow rays to the light sources to see if the point is lit?
			float probabilityToContinue = 0.1f;
			glm::vec3 newDir = calcRandomReflectionDir(closestIntersection->surfaceNormal, probabilityToContinue);
			//returns (0, 0, 0) if the ray shouldn't continue
			//determined by Russian roulette

			if (newDir.x != 0 || newDir.y != 0 || newDir.z != 0) //a direction can't be 0
			{
				//send a ray in the random direction newDir
				//std::cout << "bounce" << std::endl;
				incomingRadiance = generateRay(closestIntersection->point, newDir);
				if (incomingRadiance == glm::vec3(0.f))
					//send shadow rays
					incomingRadiance = closestIntersection->color;
			}

///			//Send shadow rays to find the color of this object if light reaches it
			glm::vec3 selfIllumination(0, 0, 0);
			bool isObscured = false;

			for (int l = 0; l < theWorld->lightList.size(); l++)
			{
				//reset the bool
				isObscured = false;

				//get current light source
				Light* currentLight = theWorld->lightList.at(l);
				//get direction to the light
				glm::vec3 dirToLight = glm::normalize(currentLight->position - closestIntersection->point);

				//create a ray to the light source
				Ray shadowRay(closestIntersection->point, dirToLight, true);
				/*std::cout << "shadow ray pos: " << closestIntersection->point.x << " " << closestIntersection->point.y << " " << closestIntersection->point.z << ", dir: "
					<< dirToLight.x << " " << dirToLight.y << " " << dirToLight.z << std::endl;*/
				
				//variable to store eventual obscuring object
				Intersection* obscuringIntersection = nullptr;

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
						isObscured = true;
						//std::cout << "object is obscuring light source, has color " << obscuringIntersection->color.x << " " << obscuringIntersection->color.y << " " << obscuringIntersection->color.z << std::endl;
						break;
					}
				}
				if (!isObscured)
					selfIllumination += closestIntersection->color * glm::max(0.f, glm::dot(closestIntersection->surfaceNormal, shadowRay.direction));
///
			}

			//The BRDF for a Lambertian reflector is a constant reflection coeficient / pi
			float fr = closestIntersection->reflectionCoef / PI;

//???????Whould it be better to calculated the BRDF at object?????

			//the division by probabilityToContinue is added because of Russian roulette in clacRandomReflectionDir
			p = selfIllumination + PI / probabilityToContinue * fr * incomingRadiance;
			//p = closestIntersection->color;
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
				glm::vec3 rayStart(stepBetweenRays*0.5f + leftUpperCorner.x + STEP_BETWEEN_PIXELS * j, -stepBetweenRays*0.5f + leftUpperCorner.y - STEP_BETWEEN_PIXELS * i, leftUpperCorner.z);
				glm::vec3 rayDir = rayStart - position; //the pixel's position - the cameras position

				//std::cout << "setting pixel in (" << rayStart.x << ", " << rayStart.y << ", " << rayStart.z << ")" << std::endl;
				//std::cout << "dir (" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << ")" << std::endl;

				color += generateRay(rayStart, rayDir);
			}
			//std::cout << "color " << color.x << " " << color.y << " " << color.z << std::endl;
			//save the colors in the image
			im.setPixel(j, i, color / (RAY_FACTOR_PER_PIXEL * RAY_FACTOR_PER_PIXEL));
		}
	}

	//save the image to a file
	std::string fileName = "a.ppm";
	im.saveAsPPM(fileName.c_str());
}

glm::vec3 Camera::calcRandomReflectionDir(glm::vec3 sufaceNormal, float probabilityOfSuccess)
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
	//std::cout << "survived" << std::endl;

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
	direction4 = rotPhi * rotTheta * direction4;

	return glm::vec3(direction4.x, direction4.y, direction4.z);

}