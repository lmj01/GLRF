#version 460 core
#define MAX_POINT_LIGHTS 10
#define PI 3.14159265359
struct Light {
	vec3 F0;
	vec3 L;
	vec3 V;
	vec3 N;
	vec3 radiance;
	vec3 albedo;
	float roughness;
	float metallic;
	float ao;
};
struct Material {
	vec3 albedo;
	float roughness;
	float metallic;
  float ao;
	bool useTextureAlbedo;
	bool useTextureNormal;
	bool useTextureRoughness;
	bool useTextureMetallic;
	bool useTextureAo;
	sampler2D textureAlbedo;
	sampler2D textureNormal;
	sampler2D textureRoughness;
	sampler2D textureMetallic;
	sampler2D textureAo;
};
uniform Material material;
uniform uint pointLight_count;
uniform vec3 pointLight_color[MAX_POINT_LIGHTS];
uniform float pointLight_power[MAX_POINT_LIGHTS];
uniform float directionalLight_power;
uniform vec3 pointLight_position[MAX_POINT_LIGHTS];
uniform vec3 directionalLight_direction;
uniform bool useDirectionalLight;
uniform vec3 camera_position;
uniform sampler2D heightMap;

struct VS_OUT {
  	vec2 texcoord;
	vec3 P;
	vec3 N;
	mat3 TBN;
};
in VS_OUT VS;

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 renderLightInfluence(Light light);
vec2 ParallaxMapping(vec2 uv, vec3 V, int minSamples, int maxSamples);

void main() {
	vec3 pL[MAX_POINT_LIGHTS];
	for (uint i = 0; i < pointLight_count; i++) {
	  pL[i] = VS.TBN * (pointLight_position[i] - VS.P);
	}
	vec3 dL = normalize(directionalLight_direction);
	vec3 N = VS.TBN * normalize(VS.N);
	vec3 V = VS.TBN * normalize(- VS.P);

	vec2 uv_parallax = ParallaxMapping(VS.texcoord, V, 8, 32);
	//vec2 uv_parallax = VS.texcoord;

	vec3 albedo = material.albedo;
	float roughness = material.roughness;
	float metallic = material.metallic;
	float ao = material.ao;
	if (material.useTextureAlbedo) albedo = texture(material.textureAlbedo, uv_parallax).rgb;
	if (material.useTextureRoughness) roughness = texture(material.textureRoughness, uv_parallax).r;
	if (material.useTextureMetallic) metallic = texture(material.textureMetallic, uv_parallax).r;
	if (material.useTextureAo) ao = texture(material.textureAo, uv_parallax).r;

	if (material.useTextureNormal) {
		//N = normalize(texture(material.textureNormal, uv_parallax).rgb);
	}

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

  vec3 Lo = vec3(0.0);

	for (uint i = 0; i < pointLight_count; i++) {
		float L_dist = length(pL[i]);
		vec3 L = normalize(pL[i]);
		float attenuation = 1.0 / (L_dist * L_dist);
		vec3 radiance = pointLight_color[i] * attenuation * pointLight_power[i];
		Light light;
		light.F0 = F0;
		light.L = L;
		light.V = V;
		light.N = N;
		light.radiance = radiance;
		light.albedo = albedo;
		light.roughness = roughness;
		light.metallic = metallic;
		light.ao = ao;
		Lo += renderLightInfluence(light);
  }

	if (useDirectionalLight) {
		Light light;
		light.F0 = F0;
		light.L = -dL;
		light.V = V;
		light.N = N;
		light.radiance = vec3(1.0) * directionalLight_power;
		light.albedo = albedo;
		light.roughness = roughness;
		light.metallic = metallic;
		light.ao = ao;
		Lo += renderLightInfluence(light);
	}

  vec3 ambient = vec3(0.03) * albedo * ao;
  vec3 color = ambient + Lo;
	//color = vec3(1.0);
  frag_color = vec4(color, 1.0);

	float brightness = dot(frag_color.rgb, vec3(0.2126, 0.7152, 0.0722));
  if (brightness > 1.0) {
		bright_color = vec4(frag_color.rgb, 1.0);
	} else {
		bright_color = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

vec2 ParallaxMapping(vec2 uv, vec3 V_tangent, int minSamples, int maxSamples) {
	float limit = - length(V_tangent.xy) / V_tangent.z;
	limit *= 1.0;

	vec2 offsetDir = normalize(V_tangent.xy);
	vec2 maxOffset = offsetDir * limit;

	int numSamples = int(mix(maxSamples, minSamples, dot(V_tangent, vec3(0.0, 0.0, 1.0))));

	float stepSize = 1.0 / float(numSamples);

	vec2 dx = dFdx(uv);
	vec2 dy = dFdy(uv);

	float currentRayHeight = 1.0;
	vec2 currentOffset = vec2(0.0);
	vec2 lastOffset = vec2(0.0);
	float lastSampledHeight = 1.0;
	float currentSampledHeight = 1.0;
	int currentSample = 0;

	while (currentSample < numSamples) {
		currentSampledHeight = textureGrad(heightMap, uv + currentOffset, dx, dy).a;
		if (currentSampledHeight > currentRayHeight) {
			float delta1 = currentSampledHeight - currentRayHeight;
			float delta2 = (currentRayHeight + stepSize) - lastSampledHeight;
			float ratio = delta1 / (delta1 + delta2);
			currentOffset = ratio * lastOffset + (1.0 - ratio) * currentOffset;
			currentSample = numSamples + 1;
		} else {
			currentSample++;
			currentRayHeight -= stepSize;
			lastOffset = currentOffset;
			currentOffset += stepSize * maxOffset;
			lastSampledHeight = currentSampledHeight;
		}
	}
	return uv + currentOffset;
}

vec3 renderLightInfluence(Light light) {
	vec3 H = normalize(light.V + light.L);

	vec3 F = fresnelSchlick(max(dot(H, light.V), 0.0), light.F0);
	float NDF = DistributionGGX(light.N, H, light.roughness);
	float G   = GeometrySmith(light.N, light.V, light.L, light.roughness);
	float denominator = 4.0 * max(dot(light.N, light.V), 0.0) * max(dot(light.N, light.L), 0.0);
	vec3 specular     = NDF * G * F / max(denominator, 0.001);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;

	kD *= 1.0 - light.metallic;
	float NdotL = max(dot(light.N, light.L), 0.0);
	return (kD * light.albedo / PI + specular) * light.radiance * NdotL;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
