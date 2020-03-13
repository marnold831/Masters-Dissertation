#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

layout(std140, binding = 0) buffer pos {
	float number;
};
in vec3 position;
layout(location = 2) in vec2 texCoord;



out Vertex
{
	vec2 texCoord;
	float number;
} OUT; 

void main(void)
{
	vec3 temp = position;
	mat4 mvp 		  = (projMatrix * viewMatrix * modelMatrix);
	gl_Position		= mvp * vec4(position, 1.0);
	OUT.texCoord	= (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.number = number;
}