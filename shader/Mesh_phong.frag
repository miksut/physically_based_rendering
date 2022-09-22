#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 worldPos;
in vec2 texCoord;

uniform struct Material{
        vec3 ka;
        vec3 kd;
        vec3 ks;
        float ns;
        bool ifTextureColor;
        bool hasNormalTexture;
        bool hasSpecTexture;
        bool hasObjectColor;
        vec3 color;
        float illumination;
    }mat;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specTexture;

uniform sampler2D cubetex;
uniform sampler2D cubetexSpec;

uniform sampler2D cubeNormalTex;
in mat3 tbnMatrix;

uniform vec3 camPos;

layout(location = 0) out vec4 color;

const int NumLights = 4;
struct AnalyticalLight {
	vec3 position;
	vec3 radiance;
};

uniform AnalyticalLight lights[NumLights];
const float gamma     = 2.2;
const float exposure  = 1.0;
const float pureWhite = 1.0;
uniform struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

void main()
{
	vec3 specValue = mat.ks;
	if (mat.hasSpecTexture)
		specValue = texture(specTexture, texCoord.xy).rgb;
	vec3 normal;
	if (mat.hasNormalTexture){
	vec3 normalTangetSpace = texture(normalTexture, texCoord.xy).rgb;
	normalTangetSpace = normalize(normalTangetSpace * 2.0 - 1.0);
    normal = normalize(tbnMatrix * normalTangetSpace);
    color = vec4(normal,1);
	}else{
		normal = vertexNormal;
	}
	vec3 diffuseColor =vec3(0);
	vec3 ambientColor =vec3(0);
	vec3 specularColor =vec3(0);
	vec3 colorMap = vec3 (0.7,0.5,0.5);
	if (mat.ifTextureColor){
		colorMap = vec3(texture(diffuseTexture,texCoord));
	}else{
		colorMap = vertexColor;
	}
	for(int i=0; i<NumLights; ++i)
	{
	vec3 lightDir = normalize(lights[i].position - worldPos);
	float diffDot = max(dot(normal, lightDir), 0.0);
	diffuseColor += diffDot * lights[i].radiance;
	float ambientFactor = 0.1f;
	ambientColor += (light.ambient.xyz * mat.ka);


	float spec;
	if (diffDot>0){
	vec3 viewDir = vec3(normalize(camPos - worldPos));
	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = max(dot(viewDir, reflectDir), 0.0);
	spec = pow(specDot, 32);
	}else{
		spec = 0;
	}
	float specStrength = 1.0;
	specularColor += specStrength * spec * lights[i].radiance;
	}

	color = vec4((ambientColor + diffuseColor) * colorMap.rgb + (specularColor * specValue) + mat.illumination * mat.color, 1.0);

	float luminance = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance/(pureWhite*pureWhite))) / (1.0 + luminance);

	// Scale color by ratio of average luminances.
	vec3 mappedColor = (mappedLuminance / luminance) * color.rgb;
	color = vec4(pow(mappedColor, vec3(1.0/gamma)), 1.0);
	//color = vec4(1,0,0,1);
}
