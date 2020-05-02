#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec3 norm_ws;
in vec3 pos_ws;

out vec3 color;

vec3 _base_color = vec3(1,0,0);
float _roughness = 0.5;
float _metallic = 0.5;
float _specular = 0.5;

vec3 light_direction = vec3(0, -1, -1);
uniform vec3 uniform_eye;

vec3 base_diffuse(
	vec3 base_color,
	float roughness,
	float theta_l,
	float theta_v,
	float theta_d
) {
	float fd90 = 0.5 + 2.0 * pow(cos(theta_d),2) * roughness;
	vec3 bd =
		base_color / M_PI *
		(1 + (fd90 - 1) * (1 - pow(cos(theta_l),5))) *
		(1 + (fd90 - 1) * (1 - pow(cos(theta_v),5)));

	return bd;
}

void main()
{
	vec3 n = normalize(norm_ws);
	vec3 v = normalize(uniform_eye - pos_ws);
	vec3 l = normalize(-1 * light_direction);
	vec3 h = normalize(v + l);

	float theta_l = dot(n,l);
	float theta_v = dot(n,v);
	float theta_d = dot(h,v);

	vec3 bc = base_diffuse(_base_color, _roughness, theta_l, theta_v, theta_d);

	color = bc;
}
