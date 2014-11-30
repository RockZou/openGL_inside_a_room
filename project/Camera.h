#include "includes.h"
#include <cmath>

class Camera{
private:
	GLuint camera_matrix_attrib_location;
	GLuint projection_matrix_attrib_location;

public:
	glm::vec3 position, gaze, up;
	Camera();
	void setAttribLocations(GLuint camera_matrix_attrib_location, GLuint projection_matrix_attrib_location);
	void setOrthographicProjection(float l, float r, float b, float t, float n, float f);
	void setPerspectiveProjection(float l, float r, float b, float t, float n, float f);
	void setPerspectiveProjection(float fov, float aspect, float n, float f); /*aspect = width/height */
	void setCameraMatrix(glm::vec3 camPosition, glm::vec3 gaze, glm::vec3 up);
	void setCameraMatrix();
};