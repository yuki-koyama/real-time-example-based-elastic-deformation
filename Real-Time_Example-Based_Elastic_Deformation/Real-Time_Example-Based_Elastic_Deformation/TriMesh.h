#pragma once

#include <string>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "Vector.h"

#include "CGObject.h"

struct TetMesh::Tetra;

struct TriMeshTraits : public OpenMesh::DefaultTraits {
    typedef Y::Vector3d Point;
    typedef Y::Vector3d Normal;
    typedef Y::Vector2d TexCoord2D;
    VertexTraits {
        int tetra_id_;     // closest tetrahedron
        Y::Vector4d baryCoord_;
        Y::Vector3d x0_; // original point
        VertexT()        // ctor
            : tetra_id_(-1)
        {}
    };
    FaceTraits {
        Y::Vector2d uv[3];
        FaceT() {
        }
    };
};

struct TriMesh : public OpenMesh::TriMesh_ArrayKernelT<TriMeshTraits>, public cg::CGObject {
    TriMesh();
    
    std::string fileName;
    bool load(const std::string& fname);
    bool empty() const { return n_vertices() == 0; }
    void updateVBO();
    void renderObject();
    void setBufferObject();
    void deleteBufferObject();
    
    // for shader program
    void initShaders();
    GLuint shaderProgram;
    GLuint contrastLocation;
    GLuint lightnessLocation;
};
