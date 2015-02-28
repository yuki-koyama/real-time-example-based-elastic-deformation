#pragma once

#include "CGObject.h"

class Sphere : public cg::CGObject {
public:
    static Sphere& getInstance() {
        static Sphere p;
        return p;
    }
    Sphere();
    ~Sphere();
    void renderObject();

    GLuint vbo;
    GLuint ibo;
    cg::Vertex *vertices;
    GLushort *indices;
    
    void setBufferObject();
    void deleteBufferObject();
};


