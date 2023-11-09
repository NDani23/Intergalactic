#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;

uniform mat4 MVP;

void main()
{
	gl_Position = (MVP * vec4( vs_in_pos, 1 )).xyww;

	vs_out_pos = vs_in_pos;
}