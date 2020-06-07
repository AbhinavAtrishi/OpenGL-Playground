#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = startMovSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

void Camera::Update() {
	// Front
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	// Right
	right = glm::normalize(glm::cross(front, worldUp));

	// Up
	up = glm::normalize(glm::cross(right, front));
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime) {
	if (keys[GLFW_KEY_W]) {
		position += front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * movementSpeed * deltaTime;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	Update();
}

glm::mat4 Camera::CalcViewMatrix() {
	return glm::lookAt(position, position + front, up);
}