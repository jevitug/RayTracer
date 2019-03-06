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
	float alpha = glm::tan(fovx / 2.0f) * ( (useJ - (w / 2.0f)) / (w / 2.0f) );
	float beta = glm::tan(fovy / 2.0f) * ( ((h / 2.0f) - useI) / (h / 2.0f)  );

	vec3 ray = glm::normalize( (alpha * uVec) + (beta * vVec) - wVec);

	return ray;
}

// TODO
//Remeber to convert object's transform into modelview (camera) space before attempting raytrace.
// The Ray is in modelview space.
float rayTraceSphere(const object & sphereObj)
{
	/*
	If sphere, apply the sphere's transformation on the ray before tracing.
	*/
	return 0;
}

// TODO
//Remeber to convert object's transform into modelview (camera) space before attempting raytrace.
// The Ray is in modelview space.
float rayTraceTriangle(const object & triangleObj)
{
	return 0;
}


/*
TODO
Calcualte the resulting pixel color.
*/
glm::vec3 computeColor(const vec3 & ray, const float & T, const object * hitObj) {
	return vec3();
}



void writeImage() {
	for (int pixelW = 0; pixelW < w; pixelW++) 
	{
		for (int pixelH = 0; pixelH < h; pixelH++) 
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
					T = rayTraceSphere(objects[i]);
				}
				else if (objects[i].type == triangle)
				{
					T = rayTraceTriangle(objects[i]);
				}
				else if (objects[i].type == triangleNorm)
				{
					T = rayTraceTriangle(objects[i]);
				}

				// if new closer object found in the trace, replace old closest.
				if (T < minT)
				{
					minT = T;
					closestObj = &objects[i];
				}
			}

			// At this point, we have found the closest hit object.

			// CALCULATE COLOR
			vec3 color = computeColor(ray, minT, closestObj);
			vec3 test(150, 240, 150);
			setPixel(pixelW, pixelH, test);
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
	modelview = Transform::lookAt(eyeinit, center, upinit);



	
	if (argc > 2) {
		stringstream tcid;
		tcid << argv[1] << "." << argv[2];
	}

	//one time acceleration structure setup
	setupAcceleration();

	//compose image
	writeImage();

	//save image to file
	saveScreenshot("screenShot.png");
	
	FreeImage_DeInitialise();

	delete[] pixels;
	return 0;
}
