#pragma once

#include "gl.h"
#include <string>
#include <vector>

namespace Y {
    int loadShader(std::string vtxShdName, std::string frgShdName, GLuint *lpProg);
    int loadShader(std::string vtxShdName, std::string frgShdName, GLuint *lpProg, std::vector<std::string> macros);
    void printShaderLog(GLuint shader);
    void printProgramInfoLog(GLuint program);
}

