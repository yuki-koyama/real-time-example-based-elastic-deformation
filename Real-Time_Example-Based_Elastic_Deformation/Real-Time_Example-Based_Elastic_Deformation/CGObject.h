#pragma once

#include "Vertex.h"

namespace cg {
    
    class CGObject {
    public:
        CGObject();
        ~CGObject();
        virtual void renderObject() = 0;
        void render();
    protected:
        GLuint vbo;
        GLuint ibo;
        Vertex *vertices;
        GLushort *indices;
        
        virtual void setBufferObject() = 0;
        virtual void deleteBufferObject() = 0;
    private:
    };

}

