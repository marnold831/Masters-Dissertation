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
	vec4 position;
	vec4 color;
} OUT; 

void main(void)
{
	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);
	vec4 pos = vec4(Positions[gl_VertexID].xyz, 1.0);
	pos = mvp * pos;
	gl_Position = pos;
	OUT.position = vec4(pos.xyz, Positions[gl_VertexID].w);
	OUT.color = vec4(0.0, 1.0, 1.0, 1.0);
}