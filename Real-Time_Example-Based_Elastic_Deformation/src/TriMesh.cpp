#include "Core.h"
#include "TetMesh.h"
#include "GLUtil.h"
#include "ShaderUtil.h"

#include "TriMesh.h"

#define VERTEX_SHADER ("TriMesh.vs")
#define FRAGMENT_SHADER ("TriMesh.fs")
#define DEFAULT_SHADER (0)

using namespace std;

namespace {
    Core& core = Core::getInstance();
}

typedef struct {
    GLfloat x, y, z, nx, ny, nz, u, v;
} TextureVertex;

bool TriMesh::load(const string& fname) {
    request_face_normals();
    request_vertex_normals();
    
    fileName = fname;
    std::string objFileName = fileName + ".obj";
    
    // load geometry
    if (!OpenMesh::IO::read_mesh(*this, objFileName)) {
        return false;
    }
    for (VIter v = vertices_begin(); v != vertices_end(); ++v) {
        data(v).x0_ = point(v);
    }
    
    printf("trimesh: #v = %d, #f = %d\n", n_vertices(), n_faces());
    setBufferObject();
    return true;
}

void TriMesh::updateVBO() {
    if (n_vertices() == 0)
        return;
    
    vector<TextureVertex> tmp_vertices;
    tmp_vertices.reserve(n_vertices());
    for (VIter v = vertices_begin(); v != vertices_end(); ++ v) {
        Y::Vector3d v_point = point(v);
        Y::Vector3d v_normal = normal(v);
        TextureVertex data = {
            v_point.x_, v_point.y_, v_point.z_,
            v_normal.x_, v_normal.y_, v_normal.z_,
            0.0, 0.0
        };
        tmp_vertices.push_back(data);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVertex) * n_vertices(), &tmp_vertices.front(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriMesh::renderObject() {
    
    glUseProgram(shaderProgram);
    glUniform1f(contrastLocation, core.m_gui.m_effect.m_sm_contrast);
    glUniform1f(lightnessLocation, core.m_gui.m_effect.m_sm_lightness);
    
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    glVertexPointer(3, GL_FLOAT, sizeof(TextureVertex), 0);
    glNormalPointer(GL_FLOAT, sizeof(TextureVertex), (GLvoid*)(sizeof(GLfloat) * 3));
    
    glDrawElements(GL_TRIANGLES, 3 * n_faces(), GL_UNSIGNED_SHORT, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    glUseProgram(DEFAULT_SHADER);
}

void TriMesh::setBufferObject() {
    vector<GLushort> tmp_indices;
    tmp_indices.reserve(3 * n_faces());
    for (FIter f = faces_begin(); f != faces_end(); ++f)
        for (FVIter v = fv_iter(f); v; ++v)
            tmp_indices.push_back(v.handle().idx());
    
    GLuint buffers[2];
    glGenBuffers(2, buffers);
    vbo = buffers[0];
    ibo = buffers[1];
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 3 * n_faces(), &tmp_indices.front(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    update_normals();
    updateVBO();
    initShaders();
}

void TriMesh::deleteBufferObject() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
}

void TriMesh::initShaders() {
    std::vector<std::string> macros;
    macros.push_back(std::string("COLOR_ADJUSTMENT"));
    std::string vtxShdName(VERTEX_SHADER);
    std::string frgShdName(FRAGMENT_SHADER);
    Y::loadShader(vtxShdName, frgShdName, &shaderProgram, macros);
        
    glUseProgram(shaderProgram);
    contrastLocation = glGetUniformLocation(shaderProgram, "contrast");
    lightnessLocation = glGetUniformLocation(shaderProgram, "lightness");
    glUseProgram(DEFAULT_SHADER);
}

TriMesh::TriMesh() {
}
