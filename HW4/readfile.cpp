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

	// set default value
	//ambient[0] = defaultAmbient[0];
	//ambient[1] = defaultAmbient[1];
	//ambient[2] = defaultAmbient[2];
	ambient[0] = 0.2f;
	ambient[1] = 0.2f;
	ambient[2] = 0.2f;
	

	// set default value
	attenuation[0] = 1;
	attenuation[1] = 0;
	attenuation[2] = 0;

	emission[0] = 0;
	emission[1] = 0;
	emission[2] = 0;

	specular[0] = 0;
	specular[1] = 0;
	specular[2] = 0;
	
	shininess = 0;

	maxDepth = 5;

	fileName = "screenShot.png";

	numobjects = 0;
	lightsUsed = 0;

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
                if (cmd == "directional" || cmd == "point") {
                    if (lightsUsed == maxlights) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << lightsUsed << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {


							

							lights[lightsUsed].dirPos.x = values[0];
							lights[lightsUsed].dirPos.y = values[1];
							lights[lightsUsed].dirPos.z = values[2];

							lights[lightsUsed].color.x = values[3];
							lights[lightsUsed].color.y = values[4];
							lights[lightsUsed].color.z = values[5];

							
							if (cmd == "directional")
							{
								lights[lightsUsed].type = directional;
								cout << "directional light created";
							}
							else
							{
								lights[lightsUsed].type = point;
								cout << "Point light created";
							}

							cout << lightsUsed << endl;
                            ++lightsUsed; 
                        }
                    }
                }


				else if (cmd == "maxdepth")
				{
					validinput = readvals(s, 1, values); // depth value
					if (validinput)
					{
						maxDepth = values[0];
						cout << "Read maxdepth: " << maxDepth << endl;
					}
				}

				else if (cmd == "output")
				{
						s >> fileName;
						cout << "Read output: " << fileName << endl;
				}


				// light attenuation command
				else if (cmd == "attenuation")
				{
					validinput = readvals(s, 3, values); // attenuation values 
					if (validinput) 
					{
						attenuation[0] = values[0];
						attenuation[1] = values[1];
						attenuation[2] = values[2];
						cout << "Attenuation Read" << endl;
					}
				}


                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i]; 
                        }
						cout << "Ambient Read: " <<ambient[0]<< ", "<< ambient[1] << ", "<< ambient[2] << ", "<< endl;
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i]; 
                        }
						cout << "diffuse Read" << endl;
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i]; 
                        }
						cout << "specular Read" << endl;
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i]; 
                        }
						cout << "emission Read" << endl;
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
						cout << "Shininess Read" << endl;
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1];

						// Initialize pixel array
						int pix = w * h;
						pixels = new BYTE[3 * pix];
						std::cout << "Size Read: " << w << ", " << h << endl;
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
						//upinit = Transform::upvector(vec3(values[6],values[7],values[8]),center - eyeinit);
						upinit = Transform::upvector(vec3(values[6], values[7], values[8]), center - eyeinit);
						fovy = values[9];

						// TODO: Unsure what to set this too.
						float radY = glm::radians(fovy);
				
						fovx = glm::degrees ( 2.0f * atan( tan(radY / 2.0f) * ((float)w / (float)h)  )  );
						cout << fovx<<", " <<fovy << " , "<< endl;
						cout << "Camera Read" << endl;
                    }
                }

				// VERTEX BLOCK=========================================================
				else if (cmd == "maxverts")
				{
					validinput = readvals(s, 1, values);
					if (validinput)
					{
						int maxVertex = values[0];
						vertexArray = new vertex[maxVertex];
						cout << "Maxverts Read: " << maxVertex << endl;
					}
				}


				else if (cmd == "maxvertnorms") 
				{
						
					validinput = readvals(s, 1, values);
					if (validinput)
					{
						int maxVertexNormal = values[0];
						vertexNormalArray = new vertex[maxVertexNormal];
						cout << "maxvert NORMAL read: " << maxVertexNormal << endl;
					}
					
				}


				// process vertex
				else if (cmd == "vertex") 
                {
					validinput = readvals(s, 3, values);
					if (validinput)
					{
						
						
						vertexArray[vertCount].transform = transfstack.top() * Transform::translate(values[0], values[1], values[2]);
						cout << "Vertex Read: " << vertCount << ", " << (vertexArray[vertCount].transform * vec4(0,0,0,1)).x << endl;
						vertCount++;
						
					}
				}

				// done
				else if (cmd == "vertexnormal") 
                {
					validinput = readvals(s, 6, values);
					if (validinput)
					{
						
						//for (int i = 0; i < 3; i++) {
							vertexNormalArray[vertNormalCount].transform = transfstack.top() * Transform::translate(values[0], values[1], values[2]);
							glm::vec4 temp = transfstack.top() *  glm::vec4(values[3], values[4], values[5], 1);
							vertexNormalArray[vertNormalCount].normal = glm::vec3(temp) / temp.w;
							//vertexNormalArray[vertNormalCount].normal = glm::vec3(values[3], values[4], values[5]);

						//}
						cout << "Vertex Normal Read: " << vertNormalCount<< ", " <<( vertexNormalArray[vertNormalCount].transform * vec4(0,0,0,1) ).x << endl;
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

						validinput = readvals(s, 3, values);
						if (validinput)
						{
							object * obj = &(objects[numobjects]);

							obj->outerTransform = transfstack.top();

							for (i = 0; i < 3; i++) {
								(obj->ambient)[i] = ambient[i];
								(obj->diffuse)[i] = diffuse[i];
								(obj->specular)[i] = specular[i];
								(obj->emission)[i] = emission[i];
							}
							obj->shininess = shininess;


							

							


							if (cmd == "tri")
							{
								vertex vert1, vert2, vert3;

								// apply transform to vertices
								vert1.transform = transfstack.top() * vertexArray[(int)values[0]].transform;
								vert2.transform = transfstack.top() * vertexArray[(int)values[1]].transform;
								vert3.transform = transfstack.top() * vertexArray[(int)values[2]].transform;
								
								obj->verticies[0] = vert1;
								obj->verticies[1] = vert2;
								obj->verticies[2] = vert3;
								obj->type = triangle;
							}
							else if (cmd == "trinormal")
							{
								vertex vert1, vert2, vert3;

								vert1.transform = transfstack.top() * vertexNormalArray[(int)values[0]].transform;
								vert2.transform = transfstack.top() * vertexNormalArray[(int)values[1]].transform;
								vert3.transform = transfstack.top() * vertexNormalArray[(int)values[2]].transform;

								vert1.normal = vec3(transfstack.top() * vec4(vertexNormalArray[(int)values[0]].normal, 0));
								vert2.normal = vec3(transfstack.top() *  vec4(vertexNormalArray[(int)values[1]].normal, 0));
								vert3.normal = vec3(transfstack.top() * vec4(vertexNormalArray[(int)values[2]].normal, 0));

								obj->verticies[0] = vert1;
								obj->verticies[1] = vert2;
								obj->verticies[2] = vert3;
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
                        validinput = readvals(s, 4, values); 
                        if (validinput) {
                            object * obj = &(objects[numobjects]); 
                            
							
							obj->radius = values[3]; 

                            // Set the object's light properties
                            for (i = 0; i < 3; i++) {
                                (obj->ambient)[i] = ambient[i]; 
                                (obj->diffuse)[i] = diffuse[i]; 
                                (obj->specular)[i] = specular[i]; 
                                (obj->emission)[i] = emission[i];
                            }
                            obj->shininess = shininess; 

                            // Set the object's transform
							// Apply current transformation to position
							obj->baseTransform = Transform::translate(values[0], values[1], values[2]);
							obj->transform = transfstack.top() * Transform::translate(values[0], values[1], values[2]);
							obj->outerTransform = transfstack.top();
				

							obj->type = sphere;
							
                        }
						//cout << "Read Sphere" <<  
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
       
        useGlu = false; // don't use the glu perspective/lookat fns

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
