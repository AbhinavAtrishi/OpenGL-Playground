#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, GLuint* indices, GLuint numOfVertices, GLuint numOfIndices) {
	indexCount = numOfIndices;

	// Generate & Bind VAO
	glGenVertexArrays(1, &VAO);

	// Generate VBO, EBO 
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);
}

void Mesh::RenderMesh() {
	// Bind VAO
	glBindVertexArray(VAO);

	// Possibly EBO should be bound & unbound, but looks like this code works w/o
	// Draw
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// Unbind VAO
	glBindVertexArray(0);

}

void Mesh::ClearMesh() {
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}