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

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	

	std::cout << "Saving screenshot: " << fname << "\n";

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

}


// TODO
// Shortcut ease of use function for setting the value of a pixel.
void setPixel(int i, int j, vec3 rgb)
{
	int base = j * (3 * w) + (i * 3);//
	//int base = i * (3 * h) + (j * 3);
	pixels[base] = rgb.x;
	pixels[base + 1] = rgb.y; // ((float)base / (float)( 3 * w * h)) * 255.0f;
	pixels[base + 2] = rgb.z;
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
	const float useI = (float)i + 0.5f;
	const float useJ = (float)j + 0.5f;

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
	/*
	If sphere, apply the sphere's transformation on the ray before tracing.
	*/
	//cout << "Ray Trace Sphere: " << ray.x <<", "<< ray.y<<", " <<ray.z << " :: ";
	//vec4 usedRay = modelview * vec4(ray.x, ray.y, ray.z, 1);
	//vec3 modelRay = vec3(usedRay) / usedRay.w;
	vec3 modelRay = ray ;
	//vec3 modelRay = vec3(usedRay);

	//vec3 modelRay = Transform::applyTrans(modelview,ray + origin);
	//vec3 spherePos = vec3 ( modelview * sphereObj.transform * vec4(0,0,0,1));
	
	vec3 spherePos = vec3(sphereObj.transform * vec4(0, 0, 0, 1));
	//cout << spherePos.x << ", " << spherePos.y << ", " << spherePos.z << endl;
	vec3 rayOrigin = origin;
	//vec3 rayOrigin = Transform::applyTrans(modelview,origin);
	//vec3 rayOrigin = vec3(0,0,0);
	//cout << rayOrigin.x<<","<< rayOrigin.y<<", "<< rayOrigin.z << endl;

	float a = glm::dot(modelRay, modelRay);
	float b = 2 * glm::dot(modelRay, rayOrigin - spherePos);
	float c = glm::dot((rayOrigin - spherePos) ,  (rayOrigin - spherePos)) - (sphereObj.radius * sphereObj.radius);
	
	float determinant = (b * b) - (4 * a * c);
	//cout << determinant << endl;

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

		
		//cout << "Determinant not 0: " << t1 << "_" << t2 << ": " << finalT << endl;
		return finalT;
	}

	return -1;
}

// TODO
float rayTraceTriangle(const vec3& origin, const vec3& ray, const object & triangleObj)
{

	/*
	vec3 useRay = Transform::applyTrans(modelview, ray + origin);
	vec3 rayOrigin = Transform::applyTrans(modelview, origin);

	vec3 A = vec3(modelview * triangleObj.verticies[0].transform * vec4(0, 0, 0, 1));
	vec3 B = vec3(modelview * triangleObj.verticies[1].transform * vec4(0, 0, 0, 1));
	vec3 C = vec3(modelview * triangleObj.verticies[2].transform * vec4(0, 0, 0, 1));
	*/
	vec3 useRay = ray ;
	vec3 rayOrigin = origin;

	vec3 A = vec3(triangleObj.verticies[0].transform * vec4(0, 0, 0, 1));
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
TODO
Calcualte the resulting pixel color.
*/
glm::vec3 computeColor(const vec3 & ray, const float & T, const object * hitObj) {
	//vec3 hit(200, 80, 220);
	vec3 nothing(10,10,10);
	vec3 planeC(200, 80, 220);
	vec3 sphereC(90,200,30);
	if (T != INFINITY) {
		if (hitObj->type == sphere)
			return sphereC;
		return planeC;
	}

	return nothing;
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

				// if new closer object found in the trace, replace old closest.
				//if (T == -1)
					//cout << "no hit" << endl;
				
				//if (T >= 0)
				//	cout << T << "\t" << minT << endl;

				if (T >= 0 && T < minT)
				{
					//cout << "New Hit" << endl;
					minT = T;
					closestObj = &objects[i];
				}
				
			}

			// At this point, we have found the closest hit object.

			// CALCULATE COLOR
			vec3 color = computeColor(ray, minT, closestObj);
			//vec3 test(150, 240, 150);
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

	// Setup camera, and thus modelview
	//lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
	
	modelview = Transform::lookAt(eyeinit, center, upinit);

	

	
	if (argc > 2) {
		stringstream tcid;
		tcid << argv[1] << "." << argv[2];
	}

	//one time acceleration structure setup
	setupAcceleration();

	//compose image
	writeImage();
	//setPixel(0, 0, vec3(255,0,255));
	//setPixel(w-1, 0, vec3(255, 0, 255));
	//save image to file
	saveScreenshot("screenShot.png");
	
	FreeImage_DeInitialise();

	delete[] pixels;
	return 0;
}
