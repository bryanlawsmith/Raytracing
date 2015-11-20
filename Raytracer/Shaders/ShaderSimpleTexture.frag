#version 430

layout(location = 0) out vec4 o_FragColor;

layout(binding = 0) uniform sampler2D u_Texture;

varying vec2 v_TexCoord0;

void main()
{
	o_FragColor = texture(u_Texture, v_TexCoord0);
}