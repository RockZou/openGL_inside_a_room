#include <iostream>
#include <fstream>
#include <sstream>
#include "texture.h"
#include "LoadShaders.h"

using namespace std;

#define BUFFER_OFFSET(offset) ( (void *) offset )
#define EPS 0.000000001


/**************KNOWN UNFIXED BUG*************/
/*When performing big range mouse movements, especially with diagonal movements, the cute
disappears and if let go of the mouse during that period, the cube never comes back.
Observation of the rotation matrix tells that once those movements are performed, 
some of the terms in the rotation matrix approaches -1.
*/


//GLuint shaderProgramID; //replaced with GLuint program


//the rotate matrix passed to the vertex shader each time
GLfloat rotate_mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

//matrix to store the rotation from earlier
GLfloat old_rotate_mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

int g_height = 512;
int g_width = 512;



GLfloat p1_x, p1_y, p2_x, p2_y;


GLuint Mrot_unif;//location of the Mrot in shader for top
GLuint Mrot_unif_1;//location of the Mrot in shader for bottom


GLuint program_0;
GLuint program_1;


Texture tex0,tex1,tex2,tex3;

/**********COPY CODE************/

/*END COPY CODE*/


/****************COPY CODE*****************************/

enum VAO_IDs { TopFaces,BottomFace, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

//dimensions for the cube
GLfloat s = 0.5;


//coordinates for the bottom surface, rendered separtately from the rest
const GLuint NumVerticesBot = 6;

GLfloat verticesBot[] = {
	s, -s, s, -s, -s, s, -s, -s, -s,
	s, -s, s, -s, -s, -s, s, -s, -s
};
GLfloat texcoordsBot[] = { 
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};



//NumVertices = number of faces * 3;
const GLuint NumVertices = 30;

GLfloat vertices[] = {
	//front -- 123, 134
	s,s,s,        -s,s,s,        -s,-s,s,
	s,s,s,        -s,-s,s,        s,-s,s,

//back -- 658,687
	-s,s,-s,       s,s,-s,        s,-s,-s,
	-s,s,-s,       s,-s,-s,      -s,-s,-s,

//left -- 267,273
	-s,s,s,        -s,s,-s,       -s,-s,-s,
	-s,s,s,        -s,-s,-s,      -s,-s,s,

//rgiht -- 514,548
	s,s,-s,         s,s,s,        s,-s,s,
	s,s,-s,         s,-s,s,       s,-s,-s,
//top -- 562,521
	s,s,-s,         -s,s,-s,      -s,s,s,
	s,s,-s,         -s,s,s,       s,s,s,
//bottom -- 437,478
	//s,-s,s,         -s,-s,s,      -s,-s,-s,
	//s,-s,s,         -s,-s,-s,      s,-s,-s
};

GLfloat texcoords[] = {
	//front -- 123, 134
	1/2.0f,1/3.0f,        1/4.0f,1/3.0f,       1/4.0f,2/3.0f,
	1/2.0f,1/3.0f,        1/4.0f,2/3.0f,       1/2.0f,2/3.0f,
	//back -- 658,687
	1.0f,1/3.0f,          3/4.0f,1/3.0f,       3/4.0f,2/3.0f,
	1.0f,1/3.0f,          3/4.0f,2/3.0f,       1.0f,2/3.0f,

	//left -- 267,273
	1/4.0f,1/3.0f,         0.0f,1/3.0f,        0.0f,2/3.0f,
	1/4.0f,1/3.0f,         0.0f,2/3.0f,        1/4.0f,2/3.0f,
	//rgiht -- 514,548
	3/4.0f,1/3.0f,         1/2.0f,1/3.0f,      1/2.0f,2/3.0f,
	3/4.0f,1/3.0f,         1/2.0f,2/3.0f,      3/4.0f,2/3.0f,
	//top -- 562,521
	1/2.0f, 0.0f,          1/4.0f, 0.0f,       1/4.0f, 1/3.0f,
	1/2.0f, 0.0f,          1/4.0f, 1/3.0f,     1/2.0f, 1/3.0f,
	//bottom -- 437,478
	//1/2.0f, 2/3.0f,        1/4.0f, 2/3.0f,     1/4.0f, 1.0f,
	//1/2.0f, 2/3.0f,        1/4.0f, 1.0f,       1/2.0f, 1.0f
};


/************Coordinates fo points********************/
/*
Front
1.+s,+s,+s||(1/2,1/3)
2.-s,+s,+s||(1/4,1/3)
3.-s,-s,+s||(1/4,2/3)
4.+s,-s,+s||(1/2,2/3)

Back
5.+s,+s,-s||(3/4,1/3)
6.-s,+s,-s||(1,1/3)
7.-s,-s,-s||(1,2/3)
8.+s,-s,-s||(3/4,2/3)

Left
2.-s,+s,+s||(1/4,1/3)
3.-s,-s,+s||(1/4,2/3)
6.-s,+s,-s||(0,1/3)
7.-s,-s,-s||(0,2/3)

Right
1.+s,+s,+s||(1/2,1/3)
4.+s,-s,+s||(1/2,2/3)
5.+s,+s,-s||(3/4,1/3)
8.+s,-s,-s||(3/4,2/3)

Top
1.+s,+s,+s||(1/2,1/3)
2.-s,+s,+s||(1/4,1/3)
5.+s,+s,-s||(1/2,0)
6.-s,+s,-s||(1/4,0)

Bottom
3.-s,-s,+s||(1/4,2/3)
4.+s,-s,+s||(1/2,2/3)
7.-s,-s,-s||(1/4,1)
8.+s,-s,-s||(1/2,1)


*/



//get the rotation Matrix, adapted from the rotation function
void getMatrix(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	// Calculate z1 and z2 for following calculations	
	GLfloat z1_sq = 3 - x1*x1 - y1*y1;
	GLfloat z1 = z1_sq>0.000001 ? sqrt(z1_sq) : EPS;
	GLfloat z2_sq = 3 - x2*x2 - y2*y2;
	GLfloat z2 = z2_sq > 0.000001 ? sqrt(z2_sq) : EPS;
	printf("z1 is %f", z1);
	printf("z2 is %f", z2);
	// Find the rotation vector
	GLfloat rotateX = y1*z2 - z1*y2;
	GLfloat rotateY = z1*x2 - x1*z2;
	GLfloat rotateZ = x1*y2 - y1*x2;
	// Normalize rotation vector
	GLfloat rotateLength = sqrt(rotateX*rotateX + rotateY*rotateY + rotateZ*rotateZ)+EPS;//0.00001 to fix the rotateLength==0 bug
	printf("rotateLength is %f\n", rotateLength);
	rotateX = -rotateX / rotateLength;
	rotateY = -rotateY / rotateLength;
	rotateZ = -rotateZ / rotateLength;
	// Determine the angle to rotate around the rotation vector


	GLfloat cosTheta = (x1*x2 + y1*y2 + z1*z2) / 3;
	GLfloat sinTheta = sqrt(1 - cosTheta*cosTheta) + EPS;
	GLfloat t_rotate_mat[16];//temperary rotation matrix
	//define the rotation matrix
	for (int i = 0; i<16; t_rotate_mat[i++] = 0);

	t_rotate_mat[0] = cosTheta + (1 - cosTheta)*rotateX*rotateX;
	t_rotate_mat[4] = rotateX*rotateY*(1 - cosTheta) - rotateZ*sinTheta;
	t_rotate_mat[8] = rotateX*rotateZ*(1 - cosTheta) + rotateY*sinTheta;
	t_rotate_mat[1] = rotateX*rotateY*(1 - cosTheta) + rotateZ*sinTheta;
	t_rotate_mat[5] = cosTheta + (1 - cosTheta)*rotateY*rotateY;
	t_rotate_mat[9] = rotateZ*rotateY*(1 - cosTheta) - rotateX*sinTheta;
	t_rotate_mat[2] = rotateX*rotateZ*(1 - cosTheta) - rotateY*sinTheta;
	t_rotate_mat[6] = rotateZ*rotateY*(1 - cosTheta) + rotateX*sinTheta;
	t_rotate_mat[10] = cosTheta + (1 - cosTheta)*rotateZ*rotateZ;
	t_rotate_mat[15] = 1;


	//matrix multiplication to store the new rotation matrix into the uniform rotation matrix
	for (int i = 0; i < 16; i++)
	{
		int col = i / 4;
		int row = i % 4;

		int t_index = row;
		int old_index = col * 4;

		rotate_mat[i] = 0;

		for (int j = 0; j < 4; j++)
		{
			rotate_mat[i] += old_rotate_mat[old_index] * t_rotate_mat[t_index];

			old_index++;
			t_index += 4;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		if (i % 4 == 0) printf("\n");
		printf("%f ", rotate_mat[i]);
	}
	
	glUseProgram(program_0);
	//Mrot_unif = glGetUniformLocation(program_0, "Mrot");
	glUniformMatrix4fv(Mrot_unif, 1, GL_FALSE, rotate_mat);

	glUseProgram(program_1);
	//get the rotation matrix location in the shader
	//Mrot_unif_1 = glGetUniformLocation(program_1, "Mrot_1");
	glUniformMatrix4fv(Mrot_unif_1, 1, GL_FALSE, rotate_mat);
}


void mouse(int button, int state, int xx, int yy){
	int i, j;
	// normalize xx and yy to x and y
	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			p1_x = x; p1_y = y;
		}
		else if (state == GLUT_UP){
			//printf("Mouse Up Function");
			p1_x = 0.000001; p1_y = 0.000001;

			//when mouse is released update the base rotation matrix
			for (int i = 0; i < 16; i++)
			{
				if (i % 4 == 0) printf("\n");
				old_rotate_mat[i] = rotate_mat[i];
				printf("%f ", old_rotate_mat[i]);
			}
		}
	}
}

void mouseMotion(int xx, int yy){

	printf("X: %d", xx);
	printf("Y: %d\n", yy);
	GLfloat x = 2 * (GLfloat)xx / g_width - 1;
	GLfloat y = 1 - 2 * (GLfloat)yy / g_width;
	p2_x = x; p2_y = y;

	getMatrix(p1_x, p1_y, p2_x, p2_y);

	glutPostRedisplay();

}

void loadTextures(void){

	glActiveTexture(GL_TEXTURE2);
	tex2.Load("cobblestone.jpg");
	tex2.Bind();

	glActiveTexture(GL_TEXTURE0);
	tex0.Load("cubemaplayout.png");
	//tex0.Load("cobblestone.jpg");
	tex0.Bind();

	glActiveTexture(GL_TEXTURE1);			// Make texture1 active
	tex3.Load("cobblestone_normal.jpg");	// Load texture from file
	tex3.Bind();

}


void init_topFaces(void){


	ShaderInfo shader_0 = { GL_VERTEX_SHADER, "vertex_shader.vsh", GL_FRAGMENT_SHADER, "fragment_shader.fsh" };
	program_0 = LoadShaders(shader_0);

	glUseProgram(program_0);



	glGenVertexArrays(1, &VAOs[TopFaces]);
	glBindVertexArray(VAOs[TopFaces]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices)+sizeof(texcoords),
		NULL,
		GL_STATIC_DRAW);

	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(vertices),
		vertices);

	//Load the colors data right after that
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		sizeof(texcoords),
		texcoords);

	GLuint vPosition = glGetAttribLocation(program_0, "s_vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vTexcoord = glGetAttribLocation(program_0, "s_vTexcoord");
	glEnableVertexAttribArray(vTexcoord);
	glVertexAttribPointer(vTexcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices))
		/*BUFFER_OFFSET(3*3*sizeof(GLfloat))*/);


	/**************************BUG REPORT********************************/
	/*
	 THE glUniform1i sets the uniform variable for all instances of Shader,
	 and therefore cannot be dynamically changed. Thus, each time a diffusemap/normalmap
	 is loaded into the glUniform1i, every object will be using the same maps.
	*/
	/*ATTEMPTED SOLUTIONS*/
	/*SOLUTION 1:
		Use sepatate shaders -- not working. only one shader seem to be active
	*/


	glUniform1i(glGetUniformLocation(program_0, "diffuseMap"), 0); // set the variable diffuseMap to 0 so that it uses texture0


	//get the rotation matrix location in the shader
	Mrot_unif = glGetUniformLocation(program_0, "Mrot");
	glUniformMatrix4fv(Mrot_unif, 1, GL_FALSE, rotate_mat);

	//glUniform1i(glGetUniformLocation(program, "normalMap"), 1); // set the variable normalMap to 1 so that it uses texture1
	//glActiveTexture(GL_TEXTURE1);			// Make texture1 active
	//tex1.Load("cobblestone_normal.jpg");	// Load texture from file
	//tex1.Bind();							// bind the texture to the active texture 

	//****************END RENDER TOP 5 FACES




}

void init_bottomFaces()
{

	//*****************RENDER BOTTOM FACE
	//Use a new set of shaders for different uniform textures --- DOESNT WORK
	ShaderInfo shader_1 = { GL_VERTEX_SHADER, "vertex_shader_1.vsh", GL_FRAGMENT_SHADER, "fragment_shader_1.fsh" };
	program_1 = LoadShaders(shader_1);

	glUseProgram(program_1);


	glGenVertexArrays(1, &VAOs[BottomFace]);
	glBindVertexArray(VAOs[BottomFace]);

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);


	//Create the buffer but don't load anything
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(verticesBot) + sizeof(texcoordsBot),
		NULL,
		GL_STATIC_DRAW);

	//Load the vertex data
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		sizeof(verticesBot),
		verticesBot);

	//Load the colors data right after that
	glBufferSubData(GL_ARRAY_BUFFER,
		sizeof(verticesBot),
		sizeof(texcoordsBot),
		texcoordsBot);

	GLuint  vPosition = glGetAttribLocation(program_1, "s_vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint	vTexcoord = glGetAttribLocation(program_1, "s_vTexcoord");
	glEnableVertexAttribArray(vTexcoord);
	glVertexAttribPointer(vTexcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticesBot)));

	glUniform1i(glGetUniformLocation(program_1, "diffuseMap"), 2); // set the variable diffuseMap to 0 so that it uses texture0
	glUniform1i(glGetUniformLocation(program_1, "normalMap"), 1); // set the variable normalMap to 1 so that it uses texture1
	// bind the texture to the active texture 

	glBindVertexArray(0);
	//*******************END Render Bottom Face****************



	//get the rotation matrix location in the shader
	Mrot_unif_1 = glGetUniformLocation(program_1, "Mrot_1");
	glUniformMatrix4fv(Mrot_unif_1, 1, GL_FALSE, rotate_mat);



	//****************NEEDED*********************
	//ADD USING MULTIPLE TEXTURE


}

//Any time the windows is resized, this function is called.
//The parameters passed to it are the new dimensions of the window.
void changeViewport(int w, int h){
	glViewport(0,0,w,h);
}

//This function is called each time the window is redrawn.
void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	//Draw Topfaces
	glUseProgram(program_0);
	glBindVertexArray(VAOs[TopFaces]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
		
	
	
	//Draw Bottomface
	glUseProgram(program_1);
	glBindVertexArray(VAOs[BottomFace]);
	glDrawArrays(GL_TRIANGLES, 0, NumVerticesBot);
	







	glFlush();
	glutSwapBuffers();

}

int main(int argc, char **argv){
	
	glutInit(&argc,argv);			 //Initialize GLUT

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_height, g_width);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Rock is the Daady");


	glewExperimental = GL_TRUE;
	if (glewInit()) {
		cerr << "GLEW Init Error" << endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	loadTextures();

	init_topFaces();
	init_bottomFaces();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	GLenum err = glewInit();		 //Very important! This initializes the entry points 
	if(GLEW_OK!=err){				 //in the OpenGL driver so we can call all the functions 
		cerr<<"GLEW error";			 //in the API.
		return 1;
	}

	/*Original Shader Program*/
	/*Shader s("myshader");
	GLuint shaderProgramID = s.Bind();*/
	/*End Shader Program*/

	glutMainLoop();
	
	return 0;
} 