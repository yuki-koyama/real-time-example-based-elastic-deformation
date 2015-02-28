#include "Core.h"
#include "Sphere.h"
#include <stdlib.h>
#include <math.h>

using namespace cg;

Sphere::Sphere() {
}

Sphere::~Sphere() {
    deleteBufferObject();
}

void Sphere::renderObject() {
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    float r = Core::getInstance().m_config.SPHERE_RADIUS;
    glScalef(r, r, r);
    
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
	
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid*)(sizeof(GLfloat) * 4));
	
    glDrawElements(GL_TRIANGLES, 3 * 32 * 8, GL_UNSIGNED_SHORT, 0);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    glPopMatrix();
}

void Sphere::setBufferObject() {
    indices = (GLushort *)malloc(sizeof(GLushort) * 3 * 32 * 8);
    vertices = (Vertex *)malloc(sizeof(Vertex) * 17 * 9);
    
    Vertex* vertex = vertices;
    GLfloat dammy = 0.0f;
    for(int i = 0 ; i <= 8 ; ++i) {
        GLfloat v = i / 8.0f;
        GLfloat y = cosf(M_PI * v);
        GLfloat r = sinf(M_PI * v);
        for(int j = 0 ; j <= 16 ; ++j) {
            GLfloat u = j / 16.0f;
            Vertex data = {
                cosf(2 * M_PI * u) * r,  y, sinf(2 * M_PI * u) * r, dammy,
                cosf(2 * M_PI * u) * r,  y, sinf(2 * M_PI * u) * r, dammy
            };
            *vertex++ = data;
        }
    }
    
    GLushort* index = indices;
    for(int j = 0 ; j < 8 ; ++j) {
        int base = j * 17;
        for(int i = 0 ; i < 16 ; ++i) {
            *index++ = base + i;
            *index++ = base + i + 1;
            *index++ = base + i + 17;
            *index++ = base + i + 17;
            *index++ = base + i + 1;
            *index++ = base + i + 1 + 17;
        }
    }

    GLuint buffers[2];
    glGenBuffers(2, buffers);
    vbo = buffers[0];
    ibo = buffers[1];
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 17 * 9, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 3 * 32 * 8, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sphere::deleteBufferObject() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    free(vertices);
    free(indices);
}


