#pragma once

#include "gl.h"

namespace cg
{

    typedef struct _Vertex
    {
        GLfloat x, y, z, dammy0;
        GLfloat nx, ny, nz, dammy1;
        //    GLfloat u, v;
    } Vertex;

} // namespace cg
