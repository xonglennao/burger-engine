
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "shaderTool.h"

const int MAX_LOG_STRING = 1024;
char logstring[MAX_LOG_STRING];

GLhandleARB loadShader(const char * filename)
{

  if(filename==0)
    return 0;

  std::string filestr(filename);

  bool vertexshader = false;
  bool fragmentshader = false;
#ifdef USE_GEOMETRY_SHADER
  bool geometryshader = false;
#endif

  if(filestr.find(".vert") != std::string::npos){
    vertexshader=true;
  }else{
    if(filestr.find(".frag") != std::string::npos){
      fragmentshader=true;
    }
#ifdef USE_GEOMETRY_SHADER
    else{
      if(filestr.find(".geo") != std::string::npos){
	geometryshader=true;
      }
#endif
  }

  if(!vertexshader 
     && !fragmentshader
#ifdef USE_GEOMETRY_SHADER 
     && !geometryshader 
#endif
     )
    return 0;

  std::ifstream file(filename); 
  if(!file)
    return 0;
  
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  std::string s = buffer.str();

  GLcharARB * source = new GLcharARB[4*(buffer.str().size()/4+1)];
  if(source == 0)
    return 0;

  unsigned int i;
  for(i =0; i < buffer.str().size(); ++i){
   source[i] = s[i];
  }
  source[i] = '\0';

  GLhandleARB so = 0; 
  if(vertexshader)
    so = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  if(fragmentshader)
    so = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);    
#ifdef USE_GEOMETRY_SHADER 
  if(geometryshader)
    so = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);  
#endif

  glShaderSourceARB(so, 1,(const GLcharARB**) &source, 0);

  return so;

}

bool compileShader(GLhandleARB object)
{

  if(object==0)
    return false;

  glCompileShaderARB(object);
  
  int status;
  glGetObjectParameterivARB(object, GL_OBJECT_COMPILE_STATUS_ARB, &status);
  if(status==0){
    int length = 0;
    glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
    if(length>0){
      GLsizei minlength = std::min(MAX_LOG_STRING,length);
      glGetInfoLogARB(object, minlength, 0, logstring);
      std::cerr << logstring << std::endl;
    }
    return false;
  }
  return true;
}


GLhandleARB linkShaders(GLhandleARB* object, const unsigned int& nb)
{
  if(object==0)
    return 0;

  GLhandleARB po = 0;
  po = glCreateProgramObjectARB();

  for(unsigned int i = 0; i < nb; ++i){
    if(object[i]>0)
      glAttachObjectARB(po,object[i]);
  }
  
  glLinkProgramARB(po);

  int status;
  glGetObjectParameterivARB(po, GL_OBJECT_LINK_STATUS_ARB, &status);
  if(status==0){
    int length = 0;
    glGetObjectParameterivARB(po, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
    if(length>0){
      GLsizei minlength = std::min(MAX_LOG_STRING,length);
      glGetInfoLogARB(po, minlength, 0, logstring);
      std::cerr << logstring << std::endl;
    }
    return 0;
  }
  return po;
}

bool isExtensionSupported(char * ext)
{  
  std::string extensions = std::string((char*)(glGetString(GL_EXTENSIONS)));
  if(extensions.find(ext) != std::string::npos)
    return true;
  return false;
}
