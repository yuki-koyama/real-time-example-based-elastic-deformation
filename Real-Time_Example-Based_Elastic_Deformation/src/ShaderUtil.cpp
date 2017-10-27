#include "GLUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include "ShaderUtil.h"

static int _loadShader(GLuint shader, std::string shdName, std::vector<std::string> macros);

int Y::loadShader(std::string vtxShdName, std::string frgShdName, GLuint *lpProg) {
    std::vector<std::string> empty;
    return Y::loadShader(vtxShdName, frgShdName, lpProg, empty);
}

int Y::loadShader(std::string vtxShdName, std::string frgShdName, GLuint *lpProg, std::vector<std::string> macros) {
    GLuint vtxShader;
	GLuint frgShader;
	GLuint prog;
	GLint linked;
    
	vtxShader = glCreateShader(GL_VERTEX_SHADER);
	frgShader = glCreateShader(GL_FRAGMENT_SHADER);
    
	if (_loadShader(vtxShader, vtxShdName, macros) < 0) {
		return -1;
	}
    
	if (_loadShader(frgShader, frgShdName, macros) < 0) {
		return -1;
	}
    
	prog = glCreateProgram();
    
	glAttachShader(prog, vtxShader);
	glAttachShader(prog, frgShader);
    
	glDeleteShader(vtxShader);
	glDeleteShader(frgShader);
    
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    
	if (linked == GL_FALSE) {
		fprintf(stderr, "Link error of %s & %s!!\n", vtxShdName.c_str(), frgShdName.c_str());
        printProgramInfoLog(prog);
		return -1;
	}
    
	*lpProg = prog;
    
	return 0;
}

void Y::printShaderLog(GLuint shader) {
    int logSize;
    int length;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);
    
    GLchar *log = (GLchar *)malloc(sizeof(GLchar) * logSize);
    
    if (logSize > 1) {
        glGetShaderInfoLog(shader, logSize, &length, log);
        fprintf(stderr, "Shader Info Log\n%s\n", log);
    }
}

void Y::printProgramInfoLog(GLuint program) {
    GLsizei bufSize;
    
    glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
    
    if (bufSize > 1) {
        GLchar *infoLog;
        
        infoLog = (GLchar *)malloc(bufSize);
        if (infoLog != NULL) {
            GLsizei length;
            
            glGetProgramInfoLog(program, bufSize, &length, infoLog);
            fprintf(stderr, "Program Info Log:\n%s\n", infoLog);
            free(infoLog);
        }
        else
            fprintf(stderr, "Could not allocate InfoLog buffer.\n");
    }
}


// static functions

int _loadShader(GLuint shader, std::string shdName, std::vector<std::string> macros) {
	FILE *fp;
	void *buf;
	int size;
	GLint compiled;
    
	if ((fp = fopen(shdName.c_str(), "rb")) == NULL) {
		fprintf(stderr, "%s is not found!!\n", shdName.c_str());
		return -1;
	}
    
	fseek(fp, 0, SEEK_END);
	size = (int)ftell(fp);
    
	if ((buf = (void *)malloc(size + 1)) == NULL) {
		fprintf(stderr, "Memory is not enough for %s\n", shdName.c_str());
		fclose(fp);
		return -1;
	}
    
	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, size, fp);
    ((char *)buf)[size] = '\0';
   
    std::string header;
    for (std::vector<std::string>::iterator macro = macros.begin(); macro != macros.end(); ++ macro) {
        header += std::string("#define ") + *macro + std::string("\n");
    }
   
    std::string wholeSource = header + std::string((char *)buf);
    
    int wholeSize = (int)wholeSource.size();
    GLchar *source = (GLchar *)malloc(wholeSize + 1);
    memcpy(source, wholeSource.c_str(), wholeSize + 1);
    source[wholeSize] = '\0';
  
//	glShaderSource(shader, 1, (const GLchar **)&buf, &size);
    glShaderSource(shader, 1, (const GLchar **)&source, &wholeSize);
    
	free(buf);
    free(source);
	fclose(fp);
    
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", shdName.c_str());
        Y::printShaderLog(shader);
		return -1;
	}
    
	return 0;

}

