#include<GL/glew.h>
#include<GL/glut.h>
#include<iostream>

GLhandleARB linkToProgram(GLhandleARB vertexShader,  GLhandleARB fragShader){
	GLhandleARB shaderPro = glCreateProgramObjectARB();
	glAttachObjectARB(shaderPro, vertexShader);
	glAttachObjectARB(shaderPro, fragShader);
	glLinkProgramARB(shaderPro);
	GLint linked;
	glGetProgramivARB(shaderPro, GL_LINK_STATUS, &linked);
	//最初用来显示链接是否成功的。刘志华 2014.9.22
	if(!linked){
		printf("链接不成功\n");
	}else{
		printf("链接成功\n");
	}
	return shaderPro;
}
GLhandleARB loadShader(char* filename, unsigned int type)
{
	FILE *pfile;
	GLhandleARB handle;
	const GLcharARB* files[1];
	
	// shader Compilation variable
	GLint result;				// Compilation code result
	GLint errorLoglength ;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[400000];
	memset(buffer,0,400000);
	
	// This will raise a warning on MS compiler
	fopen_s(&pfile, filename, "rb");
	if(!pfile)
	{
		printf("Sorry, can't open file: '%s'.\n", filename);
		exit(0);
	}
	
	fread(buffer,sizeof(char),400000,pfile);
	//printf("%s\n",buffer);
	fclose(pfile);
	handle = glCreateShaderObjectARB(type);
	if (!handle){
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
		exit(0);
	}	
	files[0] = (const GLcharARB*)buffer;
	glShaderSourceARB(
					  handle, //The handle to our shader
					  1, //The number of files.
					  files, //An array of const char * data, which represents the source code of theshaders
					  NULL);	
	glCompileShaderARB(handle);
	//Compilation checking.
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	// If an error was detected.
	if (!result)	{
		//We failed to compile.
		printf("Shader '%s' failed compilation.\n",filename);
		//Attempt to get the length of our error log.
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
		//Create a buffer to read compilation error message
		errorLogText = (char *)malloc(sizeof(char) * errorLoglength);
		//Used to get the final length of the log.
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
		// Display errors.
		printf("%s\n",errorLogText);
		// Free the buffer malloced earlier
		free(errorLogText);
	}
	return handle;
}