#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, EBO, shader;


// Vertex Shader
static const char* vShader = "										\n\
#version 330														\n\
layout (location = 0) in vec3 pos;									\n\
void main()															\n\
{																	\n\
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);}\n\
";

// Fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
out vec4 colour;													\n\
void main()															\n\
{																	\n\
	colour = vec4(1.0, 0.5, 0.0, 1.0);}								\n\
";


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	// Create the shader
	GLuint theShader = glCreateShader(shaderType);

	// Add the code to an array
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// Store the length of the code in an array 
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// Set Source & Compile
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// Check & Log for errors
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(theShader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error Compiling " << shaderType << " Shader : " << errorLog << std::endl;
		return;
	}

	// Attach the compiled shader to the existing program
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	// Create a program for storing all the shaders
	shader = glCreateProgram();

	// Check successful creation
	if (!shader) {
		std::cout << "Error creating shader" << std::endl;
		return;
	}

	// Add Vertex & Fragment Shaders
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	// Check & Log for errors
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	// Linking
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error Linking Program : " << errorLog << std::endl;
		return;
	}

	// Validating
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error Validating Program : " << errorLog << std::endl;
		return;
	}
}

void CreateTriangles() {
	// Vertices of the triangle
	GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,  // top left
		 0.0f,  0.5f, 0.0f   // top middle
	};

	GLuint indices[] = {
		4, 1, 2
	};

	// Generate & Bind VAO
	glGenVertexArrays(1, &VAO);

	// Generate VBO, EBO 
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
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
	CreateTriangles();
	CompileShaders();

	// Uncomment below line to View Wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	
		// Unbind VAO
		glBindVertexArray(0);

		// Remove Shader
		glUseProgram(0);

		// Swap Buffers
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}