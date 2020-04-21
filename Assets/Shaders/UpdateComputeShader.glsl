#version 450 core

uniform float time;

layout(std140, binding = 0) buffer pos {
	vec4 Positions[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


uint gid = gl_GlobalInvocationID.x;
vec3 centrePos = vec3(0.0, 0.0, 0.0);
void main() {

	vec3 dir = centrePos - vec3(Positions[gid].xyz);
	vec3 dirNormalize = normalize(dir);

	vec3 distanceToMove = dirNormalize * -0.1;

	vec3  newPos = Positions[gid].xyz + distanceToMove;
	Positions[gid].w = Positions[gid].w - time;

	Positions[gid] = vec4(newPos.xyz, Positions[gid].w);
	//Positions[gid] = Positions[gid];
}
	
