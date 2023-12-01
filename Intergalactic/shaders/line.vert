#version 130

uniform mat4 mvp;
uniform vec3 points[500];

out vec4 vs_out_color;

void main()
{
	gl_Position = mvp * vec4(points[gl_VertexID], 1) ;
	vs_out_color = vec4(1, 0, 0, 1);
}