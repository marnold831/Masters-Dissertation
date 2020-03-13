#version 450 core

uniform float number;

layout(std140, binding = 0) buffer pos {
	float numbers;
};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main() {
	numbers = 0;
}
	
