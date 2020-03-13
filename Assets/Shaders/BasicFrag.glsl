#version 450 core

uniform sampler2D 	mainTex;

in Vertex
{
	vec2 texCoord;
	float number;
} IN;

out vec4 fragColor;

void main(void)
{
	fragColor = texture(mainTex, IN.texCoord);

	fragColor.xy = vec2(IN.number, IN.number).xy;
}