#version 450 core

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

layout(std140, binding = 2) buffer dir {
	vec4 Directions[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

#define PI 3.14159265358979323846

#define NEWTON_ITER 2
#define HALLEY_ITER 0


float random(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

uint gid = gl_GlobalInvocationID.x;
void main() {

	float u = random(vec2(gid / 2, gid / 3));
	float v = random(vec2(gid / 5, gid / 7));
	float theta = u * 2.0 * PI;
	float phi = acos(2.0 * v - 1.0);
	float r = pow(random(vec2(gid / 9, gid / 11)), 1 / 3);
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	float sinPhi = sin(phi);
	float cosPhi = cos(phi);

	float x = r * sinPhi * cosTheta * 20;
	float y = r * sinPhi * sinTheta * 20;
	float z = r * cosPhi * 20;

	float v1 = abs(random(vec2(gid/11, gid/13)) * 50);
	float v2 = abs(random(vec2(gid/17, gid/19)) * 50);
	float w = abs(random(vec2(v1/23*23, v2/29*45)) * 300);

	Positions[gid] = vec4(x, y, z, w);
	Directions[gid] = vec4(x, y, z, w);
}
	
