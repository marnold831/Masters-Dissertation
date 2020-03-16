#version 450 core

uniform float number;

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uint gid = gl_GlobalInvocationID.x;
void main() {
	vec4 temp = Positions[gid];
	Positions[gid] = vec4(temp.x + 0.01, temp.y + 0.01, temp.z + 0.01, temp.w + 0.01);
}
	
