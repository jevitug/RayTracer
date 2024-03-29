/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "FreeImage.h"
#include "Transform.h" 

using namespace std;
#include "variables.h" 
#include "readfile.h"



// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, float* values) 
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename) 
{
	vertex * vertexArray = nullptr;
	vertex * vertexNormalArray = nullptr;

	int maxVertex = 0;
	int maxVertexNormal = 0;

	unsigned int vertCount = 0;
	unsigned int vertNormalCount = 0;

    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                int i; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "light") {
                    if (numused == numLights) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 8, values); // Position/color for lts.
                        if (validinput) {

                            // YOUR CODE FOR HW 2 HERE. DONE
                            // Note that values[0...7] shows the read in values 
                            // Make use of lightposn[] and lightcolor[] arrays in variables.h
                            // Those arrays can then be used in display too.  
							lightposn[numused * 4] = values[0];
							lightposn[(numused * 4) + 1] = values[1];
							lightposn[(numused * 4) + 2] = values[2];
							lightposn[(numused * 4) + 3] = values[3];
							lightcolor[(numused * 4)] = values[4];
							lightcolor[(numused * 4) + 1] = values[5];
							lightcolor[(numused * 4) + 2] = values[6];
							lightcolor[(numused * 4) + 3] = values[7];

                            ++numused; 
                        }
                    }
                }

                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 4, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 4; i++) {
                            ambient[i] = values[i]; 
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        for (i = 0; i < 4; i++) {
                            diffuse[i] = values[i]; 
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        for (i = 0; i < 4; i++) {
                            specular[i] = values[i]; 
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                        for (i = 0; i < 4; i++) {
                            emission[i] = values[i]; 
                        }
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1];

						// Initialize pixel array
						int pix = w * h;
						pixels = new BYTE[3 * pix];
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {

                        // DONE- YOUR CODE FOR HW 2 HERE
                        // Use all of values[0...9]
                        // You may need to use the upvector fn in Transform.cpp
                        // to set up correctly. 
                        // Set eyeinit upinit center fovy in variables.h
						// look from, at, up
						eyeinit = vec3(values[0],values[1],values[2]);
						center = vec3(values[3], values[4], values[5]);
						upinit = Transform::upvector(vec3(values[6],values[7],values[8]),center - eyeinit);
						fovy = values[9];

						// TODO: Unsure what to set this too.
						fovx = fovy; 
                    }
                }

				// VERTEX BLOCK=========================================================
				else if (cmd == "maxverts")
				{
					validinput = readvals(s, 10, values);
					if (validinput)
					{
						int maxVertex = values[0];
						vertex * vertexArray = new vertex[maxVertex];
					}
				}


				else if (cmd == "maxvertnorms") 
				{
						
					validinput = readvals(s, 10, values);
					if (validinput)
					{
						int maxVertexNormal = values[0];
						vertex * vertexNormalArray = new vertex[maxVertexNormal];
					}
					
				}


				// process vertex
				else if (cmd == "vertex") 
                {
					validinput = readvals(s, 10, values);
					if (validinput)
					{
						
						for (int i = 0; i < 3; i++) {
							vertexArray[vertCount].transform = Transform::translate(values[0], values[1], values[2]);
						}
						vertCount++;
					}
				}

				// done
				else if (cmd == "vertexnormal") 
                {
					validinput = readvals(s, 10, values);
					if (validinput)
					{
						
						for (int i = 0; i < 3; i++) {
							vertexNormalArray[vertNormalCount].transform = Transform::translate(values[0], values[1], values[2]);
							vertexNormalArray[vertNormalCount].normal = glm::vec3(values[0 + 3], values[1 + 3], values[2 + 3]);

						}
						vertNormalCount++;
					}
				}

				else if (cmd == "tri" || cmd == "trinormal") 
				{
					if (numobjects == maxobjects) { // No more objects 
						cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
					}
					else
					{

						validinput = readvals(s, 10, values);
						if (validinput)
						{
							object * obj = &(objects[numobjects]);

							obj->outerTransform = transfstack.top();

							for (i = 0; i < 4; i++) {
								(obj->ambient)[i] = ambient[i];
								(obj->diffuse)[i] = diffuse[i];
								(obj->specular)[i] = specular[i];
								(obj->emission)[i] = emission[i];
							}
							obj->shininess = shininess;

							if (cmd == "tri")
							{
								obj->verticies[0] = vertexArray[(int)values[0]];
								obj->verticies[1] = vertexArray[(int)values[1]];
								obj->verticies[2] = vertexArray[(int)values[2]];
								obj->type = triangle;
							}
							else if (cmd == "trinormal")
							{
								obj->verticies[0] = vertexNormalArray[(int)values[0]];
								obj->verticies[1] = vertexNormalArray[(int)values[1]];
								obj->verticies[2] = vertexNormalArray[(int)values[2]];
								obj->type = triangleNorm;
							}

						}
						++numobjects;
					}
                }


                // TODO 
				// Commands that Demand creating an object.
                else if (cmd == "sphere") {
                    if (numobjects == maxobjects) { // No more objects 
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                    } 
					else 
					{
                        validinput = readvals(s, 1, values); 
                        if (validinput) {
                            object * obj = &(objects[numobjects]); 
                            
							
							obj->radius = values[3]; 

                            // Set the object's light properties
                            for (i = 0; i < 4; i++) {
                                (obj->ambient)[i] = ambient[i]; 
                                (obj->diffuse)[i] = diffuse[i]; 
                                (obj->specular)[i] = specular[i]; 
                                (obj->emission)[i] = emission[i];
                            }
                            obj->shininess = shininess; 

                            // Set the object's transform
							// Apply current transformation to position
							obj->transform = transfstack.top() * Transform::translate(values[0], values[1], values[2]);
							obj->outerTransform = transfstack.top();
				

							obj->type = sphere;
							
                        }
                        ++numobjects; 
                    }
                }

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file. 
                        // Also keep in mind what order your matrix is!
						rightmultiply(Transform::translate(values[0],values[1],values[2]),transfstack);

                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.  
                        // Also keep in mind what order your matrix is!
						rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {

                        // YOUR CODE FOR HW 2 HERE. 
                        // values[0..2] are the axis, values[3] is the angle.  
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.  
                        // Note that rotate returns a mat3. 
                        // Also keep in mind what order your matrix is!
						rightmultiply(glm::mat4(Transform::rotate(values[3], vec3(values[0], values[1], values[2]))), transfstack);

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

        // Set up initial position for eye, up and amount
        // As well as booleans 

        eye = eyeinit; 
        up = upinit; 
        amount = 5;
        sx = sy = 1.0;  // keyboard controlled scales in x and y 
        tx = ty = 0.0;  // keyboard controllled translation in x and y  
        useGlu = false; // don't use the glu perspective/lookat fns

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
