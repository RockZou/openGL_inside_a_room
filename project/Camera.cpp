#include "Camera.h"


// Create two matrices, seCamera, setAttriblocation,
Camera::Camera(){
	camera_matrix_attrib_location = projection_matrix_attrib_location  = -1;
}


void Camera::setAttribLocations(GLuint camera_matrix_attrib_location, GLuint projection_matrix_attrib_location){
 this->camera_matrix_attrib_location = camera_matrix_attrib_location;
 this->projection_matrix_attrib_location = projection_matrix_attrib_location;
}


void Camera::setOrthographicProjection(float l, float r, float b, float t, float n, float f){
	float projection_mat[16];
	for(int i = 0; i<12; projection_mat[i++]=0); 
	projection_mat[0] = 2/(r-l);
	projection_mat[5] = 2/(t-b);
	projection_mat[10] = 2/(n-f);
	projection_mat[12] = -(r+l)/(r-l);
	projection_mat[13] = -(t+b)/(t-b);
	projection_mat[14] = -(n+f)/(n-f);
	projection_mat[15] = 1;
	glUniformMatrix4fv(projection_matrix_attrib_location, 1, GL_FALSE, projection_mat);
}

void Camera::setPerspectiveProjection(float l, float r, float b, float t, float n, float f){
	float projection_mat[16];
	for(int i = 0; i<16; projection_mat[i++]=0);
	projection_mat[0] = -2*n/(r-l);
	projection_mat[5] = -2*n/(t-b);
	projection_mat[8] = -(l+r)/(l-r);
	projection_mat[9] = -(b+t)/(b-t);
	projection_mat[10] = -(f+n)/(n-f);
	projection_mat[11] = -1;
	projection_mat[14] = -2*f*n/(f-n);
	glUniformMatrix4fv(projection_matrix_attrib_location, 1, GL_FALSE, projection_mat);
}


void Camera::setPerspectiveProjection(float fov, float aspect, float n, float f){ /*aspect = width/height */
	float t = abs(n)*tan(fov* 3.14159265358979323846/360); 
	float b = -t;
	float r = aspect*t;
	float l = -r;
	setPerspectiveProjection(l,r,b,t,n,f);
}

void Camera::setCameraMatrix(glm::vec3 camPosition, glm::vec3 gaze, glm::vec3 up){
	float camera_mat[16];
	this->gaze = gaze;
	this->position = camPosition;
	this->up = up;
	setCameraMatrix();	
}

void Camera::setCameraMatrix(){
	float camera_mat[16];
	gaze = glm::normalize(gaze);
	up = glm::normalize(up);
	glm::vec3 w = -gaze;
	glm::vec3 v = up;
	glm::vec3 u = glm::cross(v,w);
	glm::mat4 Mcam = glm::inverse(glm::mat4(glm::vec4(u,0.0), glm::vec4(v,0.0), glm::vec4(w,0.0) , glm::vec4(position,1.0)));
	for(int i = 0; i< 16; ++i){
		camera_mat[i] = Mcam[i/4][i%4];
	}
	glUniformMatrix4fv(camera_matrix_attrib_location, 1, GL_FALSE, camera_mat);
}
	
