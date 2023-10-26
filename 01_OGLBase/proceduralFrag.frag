#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec3 vs_out_col;
in float visibility;

out vec4 fs_out_col;

uniform vec3 light_dir = vec3(0.5,-1,0);


// fénytulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 La = vec3(0.1, 0.1, 0.1);
uniform vec3 Ld = vec3(0.9, 0.9, 0.9);
uniform vec3 Ls = vec3(0.2, 0.2, 0.2);

// anyagtulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 Ka = vec3(0.1, 0.1, 0.1);
uniform vec3 Kd = vec3(0.8, 0.8, 0.8);
uniform vec3 Ks = vec3(0.1, 0.1, 0.1);

uniform vec3 eye_pos = vec3(0,7,-30);

void main()
{

	//ambient light
	vec3 ambient = La;

	//diffuse light
	vec3 to_light = normalize(-light_dir);
	vec3 norm = normalize(vs_out_norm);
	float di = clamp(dot(to_light, norm),0,1);

	vec3 diffuse = Ld * di;

	//specular light
	vec3 ref = normalize(reflect(light_dir, vs_out_norm));
	vec3 to_eye = normalize(eye_pos-vs_out_pos);	
	float si = pow(clamp(dot(ref,to_eye),0,1),1);

	vec3 specular = si * Ls;

	//out color
	fs_out_col = vec4(ambient + diffuse, 1) * vec4(vs_out_col, 1);
	fs_out_col = mix(fs_out_col, vec4(150/255.f,105/255.f,50/255.f, 1), visibility);
}