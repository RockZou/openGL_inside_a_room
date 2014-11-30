#version 330 core

//The rotation matrix
uniform mat4 Mrot;
uniform mat4 Mrot_1;

uniform mat4 Mcam;
uniform mat4 Mproj;

in vec4 s_vPosition;
in vec2 s_vTexcoord;

out vec2 s_fTexcoord;

void main () {
	s_fTexcoord = s_vTexcoord; 
	 
	//gl_Position = Mrot*s_vPosition;
	gl_Position = Mproj*Mcam*s_vPosition;
}
