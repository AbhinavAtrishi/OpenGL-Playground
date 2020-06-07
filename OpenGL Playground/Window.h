#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma once

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose();
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();


	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	// Mouse
	GLfloat lastX, lastY, xChange, yChange;
	bool initialMovement = true;
	static void HandleMouse(GLFWwindow* window, GLdouble curX, GLdouble curY);


	//Keyboard
	bool keys[1024] = { 0 };
	static void HandleKeys(GLFWwindow* window, GLint key, GLint code, GLint action, GLint mode);
};

