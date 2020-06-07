#version 330

layout (location = 0) in vec3 pos;

out vec4 vertexColour;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
	vertexColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}