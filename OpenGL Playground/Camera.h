#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() {}
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovSpeed, GLfloat startTurnSpeed);
	
	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 CalcViewMatrix();

private:
	glm::vec3 position, front, right, up, worldUp;

	GLfloat yaw, pitch;

	GLfloat movementSpeed, turnSpeed;

	void Update();
};

