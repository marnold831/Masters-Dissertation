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
	
	if(IN.position.w < 0){
		discard;
	}
	else {
		fragColor = texture(mainTex, IN.texCoord);
	}
	
	//fragColor = texture(mainTex, IN.texCoord);
	
	//fragColor = IN.position;
	//fragColor = texture(mainTex, IN.texCoord);
	//fragColor = IN.color;
	/*fragColor.xy = vec2(IN.number, IN.number).xy;*/
}