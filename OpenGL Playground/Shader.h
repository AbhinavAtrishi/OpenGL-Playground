#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#pragma once

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vCode, const char* fCode) { CompileShader(vCode, fCode); }
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLoc() { return projectionMatrixLoc; }
	GLuint GetModelLoc() { return modelMatrixLoc; }
	
	void UseShader() { glUseProgram(shaderID); }
	void ClearShader();

	~Shader();
private:
	GLuint shaderID, projectionMatrixLoc, modelMatrixLoc;
	void CompileShader(const char* vCode, const char* fCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

