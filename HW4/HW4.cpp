/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "glm.0.9.7.1/build/native/include/glm/glm.hpp"
#include "glm.0.9.7.1/build/native/include/glm/gtc/matrix_transform.hpp"
#include "Transform.h" 
#include "FreeImage.h"

using namespace std;

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" 

//Grader grader;
//bool allowGrader = false;


void saveScreenshot(string fname) {

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
	

	std::cout << "Saving screenshot: " << fname << "\n";

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

}


// TODO
// Shortcut ease of use function for setting the value of a pixel.
void setPixel(int i, int j, vec3 rgb)
{
	int base = j * (3 * w) + (i * 3);//
	//int base = i * (3 * h) + (j * 3);
	pixels[base] = rgb.z;
	pixels[base + 1] = rgb.y; // ((float)base / (float)( 3 * w * h)) * 255.0f;
	pixels[base + 2] = rgb.x;
	return;
}


// The Ray is in modelview space. Done?
glm::vec3 createRay(const int& i, const int& j)
{
	// for use in ray calculations
	vec3 wVec = glm::normalize(eyeinit - center);
	vec3 uVec = glm::normalize(glm::cross(upinit, wVec));
	vec3 vVec = glm::normalize(glm::cross(wVec, uVec));

	// REMEBER: Using midpoint of each pixel. This means that we add 0.5 to each i/j value
	//const float useI = (float)i + 0.5f;
	//const float useJ = (float)j + 0.5f;
	const float useI = (float)j + 0.5f; 
	const float useJ = (float)i + 0.5f;

	// TODO: I'm not sure what fovx should be set to.
	//cout << fovx <<endl ;
	const float radX = glm::radians(fovx);
	const float radY = glm::radians(fovy);
	float alpha = glm::tan(radX / 2.0f) * ( (useJ - (w / 2.0f)) / (w / 2.0f) );
	float beta = glm::tan(radY / 2.0f) * ( ((h / 2.0f) - useI) / (h / 2.0f)  );

	vec3 ray = glm::normalize( (alpha * uVec) + (beta * vVec) - wVec);

	return  ray;
}

// TODO
// If return -1, then no collision occured.
float rayTraceSphere(const vec3& origin, const vec3& ray, const object & sphereObj)
{
	
	// Apply inverse transform to ray, then ray trace against the initial transform of the sphere.
	vec3 modelRay =  vec3(glm::inverse(sphereObj.outerTransform) * vec4(ray.x, ray.y, ray.z, 0));
	
	vec3 spherePos = vec3(sphereObj.baseTransform * vec4(0, 0, 0, 1)); // get the untransformed sphere position.
	
	vec3 rayOrigin = vec3(glm::inverse(sphereObj.outerTransform) * vec4(origin.x,origin.y,origin.z,1));
	

	float a = glm::dot(modelRay, modelRay);
	float b = 2 * glm::dot(modelRay, rayOrigin - spherePos);
	float c = glm::dot((rayOrigin - spherePos) ,  (rayOrigin - spherePos)) - (sphereObj.radius * sphereObj.radius);
	
	float determinant = (b * b) - (4 * a * c);

	// No intersection
	if (determinant < 0) {
		return -1;
	}
	else
	{
		float t1 = (-b + sqrt(determinant)) / (2 * a);
		float t2 = (-b - sqrt(determinant)) / (2 * a);
		
		
		float finalT = -1;

		if (t1 < 0 && t2 < 0)
			finalT = -1;
		else if (t1 >= 0 && t2 < 0)
			finalT = t1;
		else if (t2 >= 0 && t1 < 0)
			finalT = t2;
		else
		{
			finalT = fmin(t1, t2);
		}

		return finalT;	
	}
	return -1;
}


float rayTraceTriangle(const vec3& origin, const vec3& ray, const object & triangleObj)
{

	
	vec3 useRay = ray ;
	vec3 rayOrigin = origin;

	vec3 A = vec3( triangleObj.verticies[0].transform * vec4(0, 0, 0, 1));
	vec3 B = vec3( triangleObj.verticies[1].transform * vec4(0, 0, 0, 1));
	vec3 C = vec3( triangleObj.verticies[2].transform * vec4(0, 0, 0, 1));


	vec3 n = glm::normalize(glm::cross(C - A, B - A));
	
	float t = (glm::dot(A, n) - glm::dot(rayOrigin, n)) / glm::dot(useRay,n);

	if (t < 0)
		return -1;

	vec3 Apnormal = glm::cross(n, C - B) / (glm::dot(glm::cross(n, C - B), (A - C)));
	vec3 Bpnormal = glm::cross(n, A - C) / (glm::dot(glm::cross(n, A - C), (B - A)));
	vec3 Cpnormal = glm::cross(n, B - A) / (glm::dot(glm::cross(n, B - A), (C - B)));

	float Apw = glm::dot(-Apnormal, C);
	float Bpw = glm::dot(-Bpnormal, A);
	float Cpw = glm::dot(-Cpnormal, B);

	vec3 P = rayOrigin + useRay * t;
	float a = glm::dot(Apnormal, P) + Apw;
	float b = glm::dot(Bpnormal, P) + Bpw;
	float c = glm::dot(Cpnormal, P) + Cpw;

	if (a < 0 || a > 1) {
		return -1;
	}
	if (b < 0 || b > 1) {
		return -1;
	}
	if (c < 0 || c > 1) {
		return -1;
	}

	return t;
}


/*
* Ray trace the light, at the light index.
* Returns the distance from the surfacePoint to the light.
* if path to light is blocked, returns -1.
*/
float rayTraceLight(int lightIndex, vec3 surfacePoint)
{
	vec3 sToLight = lights[lightIndex].dirPos - surfacePoint;
	float lightDist = glm::length(sToLight);


	for (int i = 0; i < numobjects; i++)
	{
		float T = -1;
		if (objects[i].type == sphere)
		{
			T = rayTraceSphere(surfacePoint, sToLight, objects[i]);

		}
		else if (objects[i].type == triangle)
		{
			T = rayTraceTriangle(surfacePoint, sToLight, objects[i]);
		}
		else if (objects[i].type == triangleNorm)
		{
			T = rayTraceTriangle(surfacePoint, sToLight, objects[i]);
		}

		// hit an object and it is closer than the light.
		if (T != -1 && T < lightDist)
		{
			return -1;
		}
	}
	return lightDist;
}


vec3 getSurfaceNormal(const vec3 & ray, const vec3 & rayOrigin, const object * hitObj)
{
	return vec3(0, 0, 1);
}


vec3 actualColorCalc(const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const vec3 mydiffuse, const vec3 myspecular, const float myshininess, lightType type)
{

	float nDotL = dot(normal, direction);
	vec3 lambert = mydiffuse * lightcolor * std::fmax(nDotL, 0.0f);

	float nDotH = dot(normal, halfvec);
	vec3 phong = myspecular * lightcolor * std::pow(std::fmax(nDotH, 0.0f), myshininess);

	vec3 retval = lambert + phong;
	return retval;
}


/*
TODO
Calcualte the resulting pixel color.
*/
glm::vec3 computeColor(const vec3 & ray, const vec3 & rayOrigin, const float & T, const object * hitObj)
{
	/*
	//Debug objects code.
	vec3 nothing(10, 10, 10);
	vec3 planeC(200, 80, 220);
	vec3 sphereC(90, 200, 30);
	if (T != INFINITY) {
		if (hitObj->type == sphere)
			return sphereC;
		return planeC;
	}
	return nothing;
	*/
	
	if (T == INFINITY)
		return vec3(0, 0, 0);

	vec3 finalColor(0,0,0);
	vec3 surfacePoint = T * ray + rayOrigin;

	finalColor.x = (hitObj->ambient[0] + hitObj->emission[0]);

	finalColor.y = (hitObj->ambient[1] + hitObj->emission[1]);

	finalColor.z = (hitObj->ambient[2] + hitObj->emission[2]);

	//finalColor.x = (hitObj->ambient[0]);

	//finalColor.y = (hitObj->ambient[1]);

	//finalColor.z = (hitObj->ambient[2]);

	vec3 diffuse,specular;
	diffuse.x = hitObj->diffuse[0];
	diffuse.y = hitObj->diffuse[1];
	diffuse.z = hitObj->diffuse[2];
	specular.x = hitObj->specular[0];
	specular.y = hitObj->specular[1];
	specular.z = hitObj->specular[2];

	// both of these could be wrong
	vec3 eyePosition = eyeinit;
	vec3 eyeDirection = glm::normalize(eyePosition - surfacePoint);

	
	// loop through lights.
	for (int i = 0; i < lightsUsed; i++)
	{

		if (lights[i].type = directional)
		{
			vec3 direction = glm::normalize(lights[i].dirPos);
			vec3 half = glm::normalize(direction + eyeDirection);
			vec3 normal = getSurfaceNormal(ray, rayOrigin, hitObj);
			vec3 color = actualColorCalc(direction, lights[i].color,normal,half,diffuse, specular, hitObj->shininess,directional);
			finalColor += color;
		}
		else if (lights[i].type = point)
		{
			//check if this light reaches this point.
			float dist = rayTraceLight(i, surfacePoint);
			if (dist == -1)
			{
				// hit did not occur. ignore this light.
				continue;
			}

			//vec3 direction = ;
			//vec3 half = ;
			//vec3 normal = getSurfaceNormal(ray, rayOrigin, hitObj);
			//vec3 color = actualColorCalc(direction, lights[i].color, normal, half, diffuse, specular, hitObj->shininess,point);

		}




		// IMPORTANT: use the global var maxDepth to determine the number of recursive calls that should be done for reflection. maxDepth

	}
	
	vec3 correctFinalColor = finalColor * 255.0f;
	//cout << correctFinalColor.x << ","<< correctFinalColor.y << ","<< correctFinalColor.z << endl;
	return (correctFinalColor);
	//return vec3(100,100,100);
	
}




void writeImage() {
	
	for (int pixelH = 0; pixelH < h; pixelH++) 
	{
		for (int pixelW = 0; pixelW < w; pixelW++)
		{
			// RAY TRACE START --- This implementation will need to change if we pick the grid acceleration structure
			// reminder to self: i is pixelW, j is pixelH
			
			// calculate/create ray
			vec3 ray = createRay(pixelW,pixelH);

			// current closest distance T along the ray to the closest object
			float minT = INFINITY;
			// closest object found so far.
			object * closestObj = nullptr;

			// do ray cast
			for (int i = 0; i < numobjects; i++) 
			{
				
				
				float T = 0;
				if (objects[i].type == sphere)
				{
					T = rayTraceSphere(eyeinit, ray, objects[i]);
					
				}
				else if (objects[i].type == triangle)
				{
					T = rayTraceTriangle(eyeinit, ray, objects[i]);
				}
				else if (objects[i].type == triangleNorm)
				{
					T = rayTraceTriangle(eyeinit, ray, objects[i]);
				}


				if (T >= 0 && T < minT)
				{
					//cout << "New Hit" << endl;
					minT = T;
					closestObj = &objects[i];
				}
				
			}

			// At this point, we have found the closest hit object.

			// CALCULATE COLOR
			vec3 color = computeColor(ray, eyeinit, minT, closestObj);
			vec3 myine(0, 1, 1);
			//setPixel(pixelW, pixelH, myine * 255.0f);
			setPixel(pixelW, pixelH, color);
		}
	}

}



// where the acceleration structure is setup, so it can be used during rendering.
void setupAcceleration() 
{

}



int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << "Usage: transforms scenefile\n";
		exit(-1);
	}
	//FreeImage_AcquireMemory
	FreeImage_Initialise();

	// Read file
	readfile(argv[1]);
	cout << "DONE READING" << endl;
	
	
	if (argc > 2) {
		stringstream tcid;
		tcid << argv[1] << "." << argv[2];
	}

	//one time acceleration structure setup
	setupAcceleration();

	//compose image
	writeImage();

	//save image to file
	saveScreenshot(fileName);
	
	FreeImage_DeInitialise();

	delete[] pixels;
	return 0;
}
