#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform vec3 light_dir = vec3(1,0,0);


// fénytulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 La = vec3(0.45, 0.45, 0.45);
uniform vec3 Ld = vec3(0.8, 0.8, 0.8);
uniform vec3 Ls = vec3(1, 1, 1);

// anyagtulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 Ka = vec3(0.2, 0.2, 0.2);
uniform vec3 Kd = vec3(0.4, 0.4, 0.4);
uniform vec3 Ks = vec3(0.6, 0.6, 0.6);

uniform vec3 eye_pos = vec3(1,1,1);

uniform sampler2D texImage;

void main()
{

	//ambient light
	vec3 ambient = Ka*La;

	//diffuse light
	vec3 to_light = normalize(-light_dir);
	vec3 norm = normalize(vs_out_norm);
	float di = clamp(dot(to_light, norm),0,1);

	vec3 diffuse = Ld * Kd * di;

	//specular light
	vec3 ref = normalize(reflect(light_dir, vs_out_norm));
	vec3 to_eye = normalize(eye_pos-vs_out_pos);	
	float si = pow(clamp(dot(ref,to_eye),0,1),500);

	vec3 specular = si * Ls * Ks ;

	//out color
	fs_out_col = vec4(ambient + diffuse + specular, 1);
}