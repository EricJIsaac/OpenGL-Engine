#version 330 core
#define PI 3.1415926535897932384626433832795

in vec3 norm_ws;
in vec3 pos_ws;

out vec3 color;

vec3 base_color = vec3(1,0,0);
float roughness = 0.1;
float specular = 0.2;

vec3 light_direction = vec3(0, -1, -1);
uniform vec3 uniform_eye;

vec3 base_diffuse(vec3 bc, float r, float HdotV, float NdotV, float NdotL) {
	float fd90 = 0.5 + 2 * r * HdotV * HdotV;

	float a = clamp(1 - NdotV, 0, 1);
	float a2 = a * a;
	float a5 = a2 * a2 * NdotV;

	float b = clamp(1 - NdotL, 0, 1);
	float b2 = b * b;
	float b5 = b2 * b2 * NdotL;

	return bc * (1 + fd90 * a) * (1 + fd90 * b) / PI;
}

float microfacet_distribution(float r, float NdotH) {
	float c = 1.0;
	float alpha = r * r;
	float alpha2 = alpha * alpha;

	float hc = clamp(NdotH, 0, 1);
	float hc2 = hc * hc;
	float hs2 = 1 - hc2;

	return c / (alpha2 * hc2 + hs2);
}

float fresnel_approximation(float s, float HdotV) {
	float a = clamp(1-HdotV, 0, 1);
	float a2 = a * a;
	float a5 = a2 * a2 * a;
	return s + (1 - s) * a5;
}

float geometric_attenuation(float r, float v) {
	float ar = 0.5 + r / 2;
	float a = ar * ar;
	float a2 = a * a;

	float u = clamp(v, 0, 1);
	float u2 = u * u;
	return 1 / (u + sqrt(a2 + (1 - a2) * u2));
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

	vec3 bc = base_diffuse(base_color, roughness, HdotV, NdotV, NdotL);
	float s_d = microfacet_distribution(roughness, NdotH);
	float s_f = fresnel_approximation(specular, HdotV);
	float s_g = geometric_attenuation(roughness, NdotL) *
	 						geometric_attenuation(roughness, NdotV);

	color = bc * (1 - specular) + specular * s_d * s_f * s_g / 4;
}
