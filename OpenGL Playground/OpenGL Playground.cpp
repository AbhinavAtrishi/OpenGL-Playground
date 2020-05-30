#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "										\n\
#version 330														\n\
layout (location = 0) in vec3 pos;									\n\
void main()															\n\
{																	\n\
	gl_Position = vec4(0.5 * pos.x, 0.5 * pos.y, 0.5 * pos.z, 1.0);}\n\
";

// Fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
out vec4 colour;													\n\
void main()															\n\
{																	\n\
	colour = vec4(0.5, 0.2, 1.0, 1.0);}								\n\
";


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];

	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling " << shaderType << " Shader : " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		std::cout << "Error creating shader" << std::endl;
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program : " << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program : " << eLog << std::endl;
		return;
	}
}

void CreateTriangle() {
	// Vertices of the triangle
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Generate & Bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate & Bind VBO for the above VAO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Connect buffer to actual data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);

}

int main()
{
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

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "First Window", NULL, NULL);

	if (!mainWindow) {
		std::cout << "GLFW Window Creation Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer sizes
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set main window as current
	glfwMakeContextCurrent(mainWindow);

	// Allow extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Init failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Load & Compile Shaders & triangle
	CreateTriangle();
	CompileShaders();

	// Loop till the window is closed
	while (!glfwWindowShouldClose(mainWindow)) {

		// Get events
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.4f, 0.6f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Assign shader
		glUseProgram(shader);

		// Bind VAO
		glBindVertexArray(VAO);

		// Draw 
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unbind VAO
		glBindVertexArray(0);

		// Remove Shader
		glUseProgram(0);

		// Swap Buffers
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}