#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec3 norm_ws;
in vec3 pos_ws;

out vec3 color;

vec3 base_color = vec3(1,0,0);
float roughness = 0.5;
float specular = 0.3;

vec3 light_direction = vec3(0, -1, -1);
uniform vec3 uniform_eye;

vec3 base_diffuse(vec3 bc, float r) {
	return bc;
}

float microfacet_distribution(float NdotH) {
	return 1.0;
}

float fresnel_approximation(float HdotV) {
	return 1.0;
}

float geometric_attenuation(float NdotL, float NdotV) {
	return NdotL * NdotV;
}

void main()
{
	vec3 n = normalize(norm_ws);
	vec3 v = normalize(uniform_eye - pos_ws);
	vec3 l = normalize(-1 * light_direction);
	vec3 h = normalize(v + l);

	float NdotL = dot(n,l);
	float NdotV = dot(n,v);
	float HdotV = dot(h,v); // HdotL would work too
	float NdotH = dot(n,h);

	vec3 bc = base_diffuse(base_color, roughness);
	float d = microfacet_distribution(NdotH);
	float f = fresnel_approximation(HdotV);
	float g = geometric_attenuation(NdotL, NdotV);

	color = bc * (1 - specular) + specular * d * f * g / (4 * NdotL * NdotV);
}
