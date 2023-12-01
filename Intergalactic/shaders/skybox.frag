#version 330 core

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;

uniform vec3 playerPos;

void main()
{
	fs_out_col = texture( skyboxTexture, (vs_out_pos));

}