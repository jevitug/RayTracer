// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.
#include <iostream>
#include "Transform.h"

// Helper rotation function.  Please implement this. Done.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
	vec3 norm = glm::normalize(axis);
	float rads = glm::radians(degrees);
	glm::mat3 outMat = glm::cos(rads) * glm::mat3() + \
		(1 - glm::cos(rads)) * (glm::mat3x3(norm, vec3(0), vec3(0)) * glm::transpose(glm::mat3x3(norm, vec3(0), vec3(0)))) + \
		glm::sin(rads) * mat3(vec3(0, norm.z, -norm.y), vec3(-norm.z, 0, norm.x), vec3(norm.y, -norm.x, 0));

	//std::cout << "called\n";
	return outMat;
}

// Done.
void Transform::left(float degrees, vec3& eye, vec3& up)
{
	mat3 rotMat = rotate(degrees, glm::normalize(up));
	vec3 oldEye = eye;
	eye = rotMat * eye;
	up = (rotMat * (up + oldEye)) - eye;

}

// Done.
void Transform::up(float degrees, vec3& eye, vec3& up)
{
	mat3 rotMat = rotate(degrees, glm::normalize(glm::cross(eye, up)));
	vec3 oldEye = eye;
	eye = rotMat * eye;
	up = (rotMat * (up + oldEye)) - eye;
}

// Done.
mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 w = glm::normalize(eye - center);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::normalize(glm::cross(w, u));

	mat4 frameMat(mat3(u, v, w));
	frameMat = glm::transpose(frameMat);


	mat4 eyeMat;
	eyeMat[3][0] = -eye.x;
	eyeMat[3][1] = -eye.y;
	eyeMat[3][2] = -eye.z;

	mat4 finalMat = frameMat * eyeMat;

	return finalMat;
}

// Done.
mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	//mat4 ret;

	mat4 ret(0.0f);

	/*
	float d = 1.0f / glm::tan(glm::radians(fovy) / 2.0f);
	float A = (zFar + zNear) / (zFar - zNear);
	float B = (2 * zFar * zNear) / (zFar - zNear);
	ret[0][0] = d / aspect;
	ret[1][1] = d;
	ret[2][2] = A;
	ret[2][3] = -1.0f;
	ret[3][2] = B;
	*/
	float d = 1.0f / glm::tan(glm::radians(fovy) / 2.0f);
	float A = (zFar + zNear) / (zNear - zFar);
	float B = (2 * zFar * zNear) / (zNear - zFar);
	ret[0][0] = d / aspect;
	ret[1][1] = d;
	ret[2][2] = A;
	ret[2][3] = -1.0f;
	ret[3][2] = B;

	return ret;
}

// TODO
mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	mat4 ret;
	// YOUR CODE FOR HW2 HERE
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
	return ret;
}

// TODO
mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	mat4 ret;
	// YOUR CODE FOR HW2 HERE
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;
	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
