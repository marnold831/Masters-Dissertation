#version 450 core

uniform sampler2D 	mainTex;

in Vertex
{
	vec4 position;
	vec4 color;
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	if(IN.position.w <-1000)	{
		discard;
	}
	
	fragColor = texture(mainTex, IN.texCoord);
	
	
	//fragColor = texture(mainTex, IN.texCoord);
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	/*fragColor.xy = vec2(IN.number, IN.number).xy;*/
}