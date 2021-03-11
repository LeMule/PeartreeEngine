#version 330 core
out vec4 FragColor;
in vec4 fragmentColor;

in vec2 TexCoord;

uniform sampler2D _texture1;
uniform sampler2D _texture2;

uniform float red;
uniform float green;
uniform float blue;
uniform float alpha;

void main()
{
	FragColor = mix(texture(_texture1, TexCoord),
					texture(_texture2, TexCoord), 0.2) * 
				vec4(fragmentColor.x + red, 
					 fragmentColor.y + green, 
					 fragmentColor.z + blue, 
					 fragmentColor.w + alpha);
}