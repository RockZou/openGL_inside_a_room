#version 330 core

//The rotation matrix
uniform mat4 Mrot;

in vec4 s_vPosition;
in vec2 s_vTexcoord;
out vec2 s_fTexcoord;

//need to send out the vertex position for fragment shader to do lighting
out vec3 s_fPosition;

void main () {
	s_fTexcoord = s_vTexcoord; 

	s_fPosition=s_vPosition.xyz;//send the vertex position
	 
	gl_Position = Mrot*s_vPosition;
}
