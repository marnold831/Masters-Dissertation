#version 330 core

uniform float particleSize = 2.5;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in Vertex{
	vec4 position;
	vec4 color;
} IN[];

out Vertex{
	vec4 position;
	vec4 color;
	vec2 texCoord;
} OUT;

void main() {

	for (int i = 0; i < gl_in.length(); ++i) {
		OUT.color = IN[i].color;
		OUT.position = IN[i].position;
		//OUT.position = vec4(gl_in[i].gl_Position.x, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z, gl_in[i].gl_Position.w);
		// top right
		gl_Position = gl_in[i].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(1, 0);

		EmitVertex();

		// Top Left
		gl_Position = gl_in[i].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(0, 0);
		EmitVertex();

		// bottom right
		gl_Position = gl_in[i].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(1, 1);
		EmitVertex();

		// bottom Left
		gl_Position = gl_in[i].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(0, 1);
		EmitVertex();

		EndPrimitive();

	}
}