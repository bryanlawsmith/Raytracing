#version 430

uniform mat4 u_Transform;

layout (location = 0) in vec2 i_Position;
layout (location = 1) in vec2 i_TexCoord0;

varying vec2 v_TexCoord0;

void main()
{
	v_TexCoord0 = i_TexCoord0;

	gl_Position = u_Transform * vec4(i_Position, 0.0f, 1.0f);
}