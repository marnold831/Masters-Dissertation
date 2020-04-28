#version 450 core

uniform float time;

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

layout(std140, binding = 2) buffer dir {
	vec4 Directions[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uint gid = gl_GlobalInvocationID.x;
vec3 centrePos = vec3(0.0, 0.0, 0.0);
float RadConvert = 3.14159265358979323846 / 180;

float OffsetCalc(float time){
	float offset = sin(time * RadConvert) + cos(time * RadConvert);
	return offset;
}


void main() {

	vec3 dir = centrePos - vec3(Directions[gid].xyz);
	vec3 dirNormalize = normalize(dir);

	vec3 distanceToMove = dirNormalize * -0.1;
	vec3  newPos = Positions[gid].xyz + distanceToMove;

	Directions[gid] = vec4(newPos.xyz, 0.0);

	vec3 perpDir = vec3(-dirNormalize.y, dirNormalize.x, dirNormalize.z);
	
	Positions[gid].w = Positions[gid].w - time;

	vec3 offset = perpDir * OffsetCalc(Positions[gid].w);

	newPos = newPos + offset;

	Positions[gid] = vec4(newPos.xyz, Positions[gid].w);
	//Positions[gid] = Positions[gid];
}
	
