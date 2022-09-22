#version 410 core

const int NumLights = 4;
const float gamma     = 2.2;
const float exposure  = 1.0;
const float pureWhite = 1.0;
struct AnalyticalLight {
	vec3 position;
	vec3 radiance;
};

uniform AnalyticalLight lights[NumLights];

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 worldPos;
in vec2 texCoord;


layout(location=0) out vec4 color;
uniform vec3 camPos;
in mat3 tbnMatrix;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D roughnessTexture;

void main()
{		
	vec3 albedo = texture(diffuseTexture,texCoord).rgb;
	float metalness = texture(metalnessTexture,texCoord).r;
	float roughness = texture(roughnessTexture,texCoord).r;


	// TODO: Calculate the diffuse and specular light reflected from the fragment
	// using the properties of the lights (position and radiance), properties of the
	// fragment (position, normal, color), camera position and properties of the
	// material(metalness, roughness).
	// Use the aggregated reflected light instead of albedo in the rest of this code.

	// Sampling from normal map and transforming it into world space
	// ------------------------------------------------------------------------------------------------
	
	vec3 normal = texture(normalTexture, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(tbnMatrix * normal);


	// Specular term
	// ------------------------------------------------------------------------------------------------

	// Modelling Fresnel reflectance F(h,l) using Schlick's approximation
	vec3 fZero, term1;
	vec3 baseValue = {0.04, 0.04, 0.04};
	vec3 viewDir = normalize(camPos - worldPos);
	vec3 Schlick[NumLights], lightDir[NumLights], halfway[NumLights];
	float term2[NumLights];

	if(metalness == 1.0) {fZero = albedo;}
	else if(metalness == 0.0) {fZero = baseValue;}
	else {fZero = (metalness * (albedo - baseValue)) + baseValue;}

	for(int i=0; i<NumLights; i++) {
		lightDir[i] = normalize(lights[i].position - worldPos);
		halfway[i] = normalize(lightDir[i] + viewDir);
		term1 = vec3(1.0) - fZero;
		if(dot(halfway[i], lightDir[i]) <= 0.0) {
			Schlick[i] = fZero + term1;} else {
			term2[i] = pow((1.0 - dot(halfway[i], lightDir[i])), 5.0);
			Schlick[i] = fZero + (term1 * term2[i]);}}


	// Modelling joint masking-shadowing function based on Smith function. Note: Following the tutorial
	// "Lighting" on learnopengl.com, I followed the observations of Disney/Epic Games and squared the
	// roughness parameter. The same holds for the NDF that follows.

	float smith_masking[NumLights], smith_shadowing[NumLights], separable_jointFunc[NumLights], chi_smithMask[NumLights], chi_smithShad[NumLights];
	float a_mask[NumLights], a_shad[NumLights], lambda_ggxMask[NumLights], lambda_ggxShad[NumLights];

	for(int i=0; i<NumLights; i++) {
		a_mask[i] = dot(normal, viewDir) / (roughness*roughness*(sqrt(1.0 - pow(dot(normal, viewDir),2.0))));
		a_shad[i] = dot(normal, lightDir[i]) / (roughness*roughness*(sqrt(1.0 - pow(dot(normal, lightDir[i]),2.0))));
		lambda_ggxMask[i] = (-1.0 + (sqrt(1.0 + (1.0 / (pow(a_mask[i], 2.0)))))) / 2.0;
		lambda_ggxShad[i] = (-1.0 + (sqrt(1.0 + (1.0 / (pow(a_shad[i], 2.0)))))) / 2.0;
		chi_smithMask[i] = dot(halfway[i], viewDir);
		chi_smithShad[i] = dot(halfway[i], lightDir[i]);
		if(chi_smithMask[i] <= 0) {smith_masking[i] = 0.0;} else
		{smith_masking[i] = 1.0 / (1.0 + lambda_ggxMask[i]);}
		if(chi_smithShad[i] <= 0) {smith_shadowing[i] = 0.0;} else
		{smith_shadowing[i] = 1.0 / (1.0 + lambda_ggxShad[i]);}
		separable_jointFunc[i] = smith_masking[i] * smith_shadowing[i];
	}


	// Modelling NDF using GGX/Trowbridge-Reitz NDF
	float NDF[NumLights], chi[NumLights], chi_squared[NumLights];
	const float pi = 3.141592653589;
	float roughness_squared = pow(roughness, 4.0); // squaring of the initial roughness parameter as mentioned above

	for(int i=0; i<NumLights; i++) {
			chi[i] = dot(normal, halfway[i]);
			chi_squared[i] = pow(chi[i], 2.0);
			if(chi[i] <= 0) {NDF[i] = 0.0;} else
			{NDF[i] = roughness_squared / (pi*(pow((1.0 + (chi_squared[i] * (roughness_squared - 1.0))), 2.0)));}
	}


	// Modelling specular term using the components calculated before
	vec3 specularTerm[NumLights], numerator[NumLights];
	float denominator[NumLights];

	for(int i=0; i<NumLights; i++) {
		numerator[i] = Schlick[i] * separable_jointFunc[i] * NDF[i];
		denominator[i] = 4.0 * abs(dot(normal, lightDir[i])) * abs(dot(normal, viewDir));
		specularTerm[i] = numerator[i] / denominator[i];
	}

	
	// Diffuse term
	// ------------------------------------------------------------------------------------------------
	
	vec3 diffuseTerm[NumLights];
	vec3 rho;

	if(metalness == 1.0) {rho = vec3(0.0,0.0,0.0);}
	else if(metalness == 0.0) {rho = albedo;}
	else {rho = (1.0 - metalness) * albedo;}
	
	// Considering conservation of energy rule
	for(int i=0; i<NumLights; i++) {
		diffuseTerm[i] = (vec3(1.0) - Schlick[i])*(rho / pi);
	}


	// Aggregation over the different light sources
	// ------------------------------------------------------------------------------------------------
	
	vec3 total;
	
	for(int i=0; i<NumLights; i++) {
		total += (specularTerm[i] + diffuseTerm[i]);
	}


    float luminance = dot(total, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance/(pureWhite*pureWhite))) / (1.0 + luminance);

	// Scale color by ratio of average luminances.
	vec3 mappedColor = (mappedLuminance / luminance) * total;

	// Gamma correction.
	color = vec4(pow(mappedColor, vec3(1.0/gamma)), 1.0);
}

