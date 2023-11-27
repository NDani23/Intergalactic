#version 330 core

in vec3 vs_in_pos;

uniform mat4 ViewProj;
uniform mat4 Transform;

void main()
{
	gl_Position = ViewProj * Transform * vec4(vs_in_pos, 1.0);
}
