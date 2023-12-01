#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec3 vs_out_col;
out float visibility;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform vec3 eye_pos;

const float density = 50000;
const float gradient = 500;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vec3 pos = (world * vec4(vs_in_pos, 1)).xyz;

	vec3 distance_vec = eye_pos - pos;

	float dist = length(distance_vec.xyz);

	
	vs_out_pos = pos;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_col = vs_in_col;
	//visibility = exp(-pow((dist*density),gradient));
	visibility = clamp(dist / 4000.f, 0, 1);
}