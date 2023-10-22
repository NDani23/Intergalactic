#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec3 vs_out_col;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_col = vs_in_col;
}