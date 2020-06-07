#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	projectionMatrixLoc = 0;
	modelMatrixLoc = 0;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation){
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vCode = vertexString.c_str();
	const char* fCode = fragmentString.c_str();

	CompileShader(vCode, fCode);
}

std::string Shader::ReadFile(const char* fileLocation){
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "Failed to read " << fileLocation << "! File doesn't exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vCode, const char* fCode) {
	// Create a program for storing all the shaders
	shaderID = glCreateProgram();

	// Check successful creation
	if (!shaderID) {
		std::cout << "Error creating shader" << std::endl;
		return;
	}

	// Add Vertex & Fragment Shaders
	AddShader(shaderID, vCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fCode, GL_FRAGMENT_SHADER);

	// Check & Log for errors
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	// Linking
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error Linking Program : " << errorLog << std::endl;
		return;
	}

	// Validating
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error Validating Program : " << errorLog << std::endl;
		return;
	}

	// Get model location
	modelMatrixLoc = glGetUniformLocation(shaderID, "modelMatrix");
	projectionMatrixLoc = glGetUniformLocation(shaderID, "projectionMatrix");
	viewMatrixLoc = glGetUniformLocation(shaderID, "viewMatrix");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
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


void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	modelMatrixLoc = 0;
	projectionMatrixLoc = 0;
}

Shader::~Shader() {
	ClearShader();
}