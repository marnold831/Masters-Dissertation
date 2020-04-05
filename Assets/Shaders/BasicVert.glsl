#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

layout(location = 3) in vec4 position;

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

out Vertex
{
	vec4 position;
	vec4 color;
} OUT; 

void main(void)
{
	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);
	//gl_Position = mvp * Positions[gl_VertexID];
	//OUT.position = vec3(mvp * Positions[gl_VertexID]).xyz;

	gl_Position = mvp * position;
	OUT.position = mvp * position;
	OUT.color = vec4(0.0, 1.0, 1.0, 1.0);
}