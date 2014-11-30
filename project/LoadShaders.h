#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "GL/glew.h"
#include "GL/freeglut.h"

struct ShaderInfo {
	GLenum vTarget;
	const char *vShaderFile;
	GLenum fTarget;
	const char *fShaderFile;
};

GLuint LoadShaders( ShaderInfo shaderInfo );
const char* getShaderProgram( const char *filePath, string &shaderProgramText );