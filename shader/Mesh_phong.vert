#version 330 core

// Input vertex data
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;

layout(location = 4) in vec3 vTangent;

out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 worldPos;
out vec2 texCoord;

out mat3 tbnMatrix;

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


// matrices that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform bool ifVertexColor;
uniform vec3 objectColor;

void main(){
	// Output position of the vertex, in clip space : MVP * vPosition
	texCoord = vTexCoords;
	gl_Position = mvpMatrix * vec4(vPosition, 1);
	worldPos = vec3(modelMatrix * vec4(vPosition, 1));

	vec3 bitangent = cross(vTangent, vNormal);
	vec3 T = normalize(vec3(modelMatrix * vec4(vTangent, 0.0)));
	vec3 B = normalize(vec3(modelMatrix * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(vNormal, 0.0)));
	tbnMatrix = mat3(T, B, N);
	vertexNormal = N;
	if (mat.hasObjectColor){
		vertexColor = mat.color;
	}else if (ifVertexColor){
		vertexColor = vColor;
	}else{
		vertexColor = vec3(0.5,0.5,0.5);
	}

}
