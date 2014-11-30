#version 330 core

//The rotation matrix -- to be deleted
uniform mat4 Mrot_1;
uniform mat4 Mrot;

in vec4 s_vPosition;
in vec2 s_vTexcoord;
in vec3 vnewEyePosition;

out vec2 s_fTexcoord;
out vec3 fnewEyePosition;
//need to send out the vertex position for fragment shader to do lighting
out vec3 s_fPosition;

uniform mat4 Mcam;
uniform mat4 Mproj;


void main () {
	//gl_Position = Mrot_1*s_vPosition;// to be deleted
	gl_Position = Mproj*Mcam*s_vPosition;
	
	s_fPosition=s_vPosition.xyz;//send the vertex position
	
	s_fTexcoord = s_vTexcoord; 

	fnewEyePosition = vnewEyePosition;

}
