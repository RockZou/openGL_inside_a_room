#version 330 core

in vec2 s_fTexcoord;
in vec3 s_fPosition;
in vec3 fnewEyePosition;

uniform vec3 camPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

out vec4 fColor;

vec4 computeColor();

vec3 position;
void main () {
	position=s_fPosition;

	fColor = computeColor();
}


vec4 computeColor(){
	
	vec4 light = vec4(0.8,0.8,0.8,1.0);
	vec3 eyePosition = camPos;
	vec3 lightPosition = vec3(0,0,-10); 
	  
	// set new color
	vec4 ambient = vec4(0.1,0.1, 0.1, 1);

	vec4 diffuse = texture(diffuseMap, s_fTexcoord);       // Get the diffuse color from texture map 0
	vec4 normalMapColor = texture(normalMap, s_fTexcoord); // Get the color from texture map 1  
	vec3 normal = 2*normalMapColor.xyz - vec3(1,1,1);      // convert the color to normal vector   
	normal.z = -normal.z;                                  // invert z direction since we are using Normalized Device Coordinates 
	vec4 specular = vec4(1,1,1,1);
	float shininess = 15;

	vec3 dirToLight = normalize(lightPosition - position);
	vec3 dirToEye = normalize(eyePosition - position);
	vec3 halfvector = normalize(dirToEye+dirToLight);
	float cosSigma = dot(halfvector, normal);
	float cosTheta = clamp(dot(normal, dirToLight),0,1);
	return ambient + diffuse*cosTheta + specular*pow(cosSigma,shininess);

}