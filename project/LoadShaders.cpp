#include "LoadShaders.h"
	
GLuint LoadShaders( ShaderInfo shaderInfo )
{
	GLuint program = glCreateProgram(); // create program
	
	GLuint vertexShader   = glCreateShader( GL_VERTEX_SHADER );	    // create a vertex shader object
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );	// create a fragment shader object
	
	// load and compile vertex shader
	string vstext;
	const char* text = getShaderProgram( shaderInfo.vShaderFile, vstext);
	//GLint length = vstext.size();
	glShaderSource( vertexShader, 1, &text, NULL );
	glCompileShader( vertexShader );
		
	// load and compile fragment shader
	string fstext;
	text = getShaderProgram( shaderInfo.fShaderFile, fstext );
	glShaderSource(fragmentShader, 1, &text, NULL );
	glCompileShader( fragmentShader );
	
    // attach the vertex and fragment shaders to the program
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );

	// link the objects for an executable program
	glLinkProgram( program );

	// return the program
	return program;
}

const char* getShaderProgram( const char *filePath, string &shadertext )
{
	fstream shaderFile( filePath, ios::in );

	if ( shaderFile.is_open() )
	{
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shadertext = buffer.str();
		buffer.clear();
	}
	shaderFile.close();

	return shadertext.c_str();
}