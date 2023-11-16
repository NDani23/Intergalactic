#version 330 core

out vec4 fs_out_col;

uniform vec4 Color;

void main()
{
	fs_out_col = Color;
}