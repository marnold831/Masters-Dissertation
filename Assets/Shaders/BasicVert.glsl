#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

out Vertex
{
	vec3 position;
	vec4 color;
} OUT; 

void main(void)
{
	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);
	gl_Position = mvp * Positions[gl_VertexID];
	OUT.position = vec3(mvp * Positions[gl_VertexID]).xyz;
	OUT.color = vec4(0.0, 1.0, 1.0, 1.0);
}