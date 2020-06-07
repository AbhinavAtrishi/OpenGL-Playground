#include "Window.h"

Window::Window() {
	// Initialize with default values
	width = 800;
	height = 600;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	// Initialize with user input values
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
}

int Window::Initialize() {

	// Initialize GLFW
	if (!glfwInit()) {
		std::cout << "GLFW Init Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setting up GLFW Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Check Backward Compatibility & raise error
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Rendered Output", NULL, NULL);

	if (!mainWindow) {
		std::cout << "GLFW Window Creation Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer sizes
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set main window as current
	glfwMakeContextCurrent(mainWindow);

	// Allow extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	GLenum errorGLEW = glewInit();
	if (errorGLEW != GLEW_OK) {
		std::cout << "GLEW Init failed : " << glewGetErrorString(errorGLEW) << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable Depth testing
	glEnable(GL_DEPTH_TEST);

	// Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	std::cout << "Successfull in creating window" << std::endl;
}

bool Window::getShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
