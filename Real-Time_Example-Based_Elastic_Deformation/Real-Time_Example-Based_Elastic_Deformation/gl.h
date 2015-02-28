#pragma once

#if defined (__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined _WIN64 | defined _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#endif

