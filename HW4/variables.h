/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
vec3 center(0.0, 0.0, 0.0); // Center look at point 
int amountinit = 5;
int w = 500, h = 500; // width and height 
float fovy = 90.0; // For field of view
float fovx = 0;
#else 
EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN int amountinit;
EXTERN int w, h;
EXTERN float fovy;
EXTERN float fovx;
#endif

//!!!
// ADDED BY DANIEL: Used to store all the pixels. This is what is written out as the image and will be graded.
EXTERN BYTE *pixels;

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
EXTERN unsigned int vertexshader, fragmentshader, shaderprogram; // shaders
EXTERN mat4 projection, modelview; // The mvp matrices
EXTERN unsigned int projectionPos, modelviewPos; // Uniform locations of the above matrices
static enum { view, translate, scale } transop; // which operation to transform 
enum shape { sphere, triangle, triangleNorm };


// Maxinum number of recursive calls for light bouncing.
EXTERN int maxDepth;
EXTERN string fileName;

// Materials (read from file) 
// With multiple objects, these are colors for each.
const float defaultAmbient[3] = { 0.2f, 0.2f, 0.2f };
EXTERN double ambient[3];
EXTERN double diffuse[3];
EXTERN double specular[3];
EXTERN double emission[3];
EXTERN double shininess;

EXTERN float attenuation[3]; //global attenuation: const, linear, quadratic


const int maxlights = 20;
 enum lightType { directional, point };
EXTERN struct light {
	lightType type;
	vec3 dirPos;
	vec3 color;
} lights[maxlights];
EXTERN int lightsUsed;                     // How many lights are used

EXTERN struct vertex {
	mat4 transform;

	// normal of vertex. use if needed.
	glm::vec3 normal;
}; 


// For multiple objects, read from a file.  
const int maxobjects = 1000;
EXTERN int numobjects;
EXTERN struct object {
	shape type;
	float radius;		//UNTRANSFORMED radius. Not effected by any scaling.
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emission[3];
	float shininess;
	mat4 transform;		// final world poistion = outerTransform * baseTransform
	mat4 baseTransform; // innitaial position of the object, without transformation stack applied to it. 
	mat4 outerTransform; // transform stack applied to object.
	vec3 normal;

	// verticies of triangle is this is a trinangle
	vertex verticies[3];

} objects[maxobjects];
