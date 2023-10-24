#version 330 core

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skyboxTexture;

void main()
{
	//fog - sandstorm shading
	fs_out_col = mix(glm::vec4(150/255.f,105/255.f,50/255.f, 1), texture( skyboxTexture, (vs_out_pos)), (vs_out_pos.y)/2);
}