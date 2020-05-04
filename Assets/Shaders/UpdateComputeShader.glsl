#version 450 core

uniform float time;

layout(std140, binding = 0) buffer posA {
	vec4 PositionsA[];
};

layout(std140, binding = 1) buffer posB {
	vec4 PositionsB[];
};

layout(std140, binding = 2) buffer dirA {
	vec4 DirectionsA[];
};

layout(std140, binding = 3) buffer dirB {
	vec4 DirectionsB[];
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uint gid = gl_GlobalInvocationID.x;
vec3 centrePos = vec3(0.0, 0.0, 0.0);
float RadConvert = 3.14159265358979323846 / 180;

float OffsetCalcSin(float time){
	float offset = sin(0.5 * time *RadConvert) + pow(cos(time * 2 * RadConvert), 0.3333333333);
	return offset;
}

float OffsetCalcCos(float time) {
	float offset = cos(time * RadConvert);
	return offset;
}



void main() {

	vec3 dir = centrePos - vec3(DirectionsA[gid].xyz);
	vec3 dirNormalize = normalize(dir);

	vec3 distanceToMove = dirNormalize * -0.1;
	vec3  newPos = DirectionsA[gid].xyz + distanceToMove;

	DirectionsB[gid] = vec4(newPos.xyz, 0.0);

	vec3 perpDirX = cross(dirNormalize, vec3(0,1,0));
	if(perpDirX.y > 0.9999){
		perpDirX = cross(dirNormalize, vec3(1,0,0));
	}
	
	PositionsB[gid].w = PositionsA[gid].w - time;

	//vec3 offsetX = perpDirX * OffsetCalcSin(PositionsB[gid].w*6);

	vec3 offsetX = perpDirX * OffsetCalcSin(PositionsB[gid].w);

	vec3 perpDirY = cross(perpDirX, vec3(0,0,1));
	vec3 offsetY = perpDirY * OffsetCalcCos(PositionsB[gid].w*6);

	vec3 perpDirZ = cross(perpDirY, vec3(1,0,0));
	vec3 offsetZ = perpDirZ * (OffsetCalcSin(0.5*PositionsB[gid].w) + (pow(OffsetCalcCos(2*PositionsB[gid].w), 1/3)));


	newPos = newPos + offsetX*10; //+ offsetY*10 + offsetZ * 10;

	PositionsB[gid] = vec4(newPos.xyz, PositionsB[gid].w);
	//PositionsA[gid] = PositionsA[gid];
}
	
