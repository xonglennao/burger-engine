#ifndef __SHADERS_HPP__
#define __SHADERS_HPP__

//#define USE_GEOMETRY_SHADER

#include "CommonGraphics.h"

// filename : name of the file contening a shader
// return : a program object
GLhandleARB loadShader(const char * filename);

// object : program object
// return : true if compilation is a success
bool compileShader(GLhandleARB object);

// object : a list of program objects
// nb : the number of program objects
// return : shader object
GLhandleARB linkShaders(GLhandleARB* object, const unsigned int& nb);

// ext : the name of the extension
// return : true if the extension is supoerted, false otherwise
bool isExtensionSupported(char * ext);
#endif
