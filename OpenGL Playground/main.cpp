#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Custom Libraries
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const GLfloat PI = 3.1415926535f;
const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";


void CreateDrawableObjects() {
	// Vertices of the triangle
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	GLuint indices[] = {
		0, 3, 1,
		1, 3, 2,
		0, 3, 2,
		1, 0, 2
	};

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(mesh1);

	Mesh* mesh2 = new Mesh();
	mesh2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(mesh2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	// Create Main Window
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	//Load & Compile Shaders & Objects (Mesh's)
	CreateShaders();
	CreateDrawableObjects();

	// Add projection Matrix 
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Add uniform variable locations
	GLuint projectionLoc = 0, modelLoc = 0;

	// Uncomment below line to View Wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Loop till the window is closed
	while (!mainWindow.getShouldClose()) {

		// Get events
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.4f, 0.6f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Assign shader
		shaderList[0].UseShader();
		projectionLoc = shaderList[0].GetProjectionLoc();
		modelLoc = shaderList[0].GetModelLoc();

		// Get time
		float timeSeconds = glfwGetTime();
		float sineWave = sin(timeSeconds) / 4.0f;
		float cosWave = cos(timeSeconds) / 4.0f;

		glm::mat4 model = glm::mat4(1.0);

		// std::cout << glm::to_string(model) << std::endl;
		model = glm::translate(model, glm::vec3(sineWave * 4.0f, -0.5f, -2.5f));
		model = glm::rotate(model, 4.0f * PI * sineWave, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));


		// Set model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		meshList[0]->RenderMesh();

		glm::mat4 model2 = glm::mat4(1.0);

		// std::cout << glm::to_string(model) << std::endl;
		model2 = glm::translate(model2, glm::vec3(cosWave * 4.0f, 0.5f, -2.5f));
		model2 = glm::rotate(model2, 4.0f * PI * cosWave, glm::vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		meshList[1]->RenderMesh();


		// Remove Shader
		glUseProgram(0);

		// Swap Buffers
		mainWindow.swapBuffers();
	}
	return 0;
}