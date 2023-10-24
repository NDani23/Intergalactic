#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;
out float visibility;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform vec3 playerPos;

const float density = 50000;
const float gradient = 500;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	

	vec3 pos = (world * vec4(vs_in_pos, 1)).xyz;
	vec3 distance_vec = playerPos - pos;
	float dist = length(distance_vec.xyz);

	vs_out_pos = pos;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;

	visibility = clamp(dist / 4000.f, 0, 1);
}