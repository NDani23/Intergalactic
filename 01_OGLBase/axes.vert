#version 130

uniform mat4 mvp;
uniform vec3 points[10];

/*uniform vec3 forward;
uniform vec3 up;
uniform vec3 cross_vec;

vec4 positions[6] = vec4[6](
	// 1. szakasz
	vec4( 0,  0, 0, 1),
	vec4(cross_vec, 1),
	// 2. szakasz
	vec4( 0,  0, 0, 1),
	vec4(up, 1),
	// 3. szakasz
	vec4( 0,  0, 0, 1),
	vec4(forward, 1)
);

vec4 colors[3] = vec4[3](
	vec4(1, 0, 0, 1),
	vec4(0, 1, 0, 1),
	vec4(0, 0, 1, 1)
);*/

out vec4 vs_out_color;

void main()
{
	gl_Position = mvp * vec4(points[gl_VertexID], 1) ;
	vs_out_color = vec4(1, 0, 0, 1);
}

// 1. feladat: hogyan lehetne a positions tömb méretét lecsökkenteni 4-re? 