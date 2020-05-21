#pragma once

#include "Vector.h"
#include <list>
#include <string>
#include <vector>

class Particle;
class tetgenio;

struct TetMesh
{
    struct Vertex
    {
        Y::Vector3f    position_;
        std::list<int> neighbor_vid_;
        int            particle_id_;
        Vertex() : particle_id_(-1) {}
    };
    std::vector<Vertex> vertices_;
    std::vector<Vertex> unused_vertices_;
    std::vector<int>    unused_indices_;
    struct Tetra
    {
        //      v0--v2
        //     / \ .|
        //    /  .\ |    face & neighbor references: the same as that of opposite vertex
        //   / .   \|
        // v1.______v3
        int vid_[4]; // vertex id
        Tetra() { vid_[0] = vid_[1] = vid_[2] = vid_[3] = -1; }
    };
    std::vector<Tetra> tetras_;

    void clear()
    {
        vertices_.clear();
        tetras_.clear();
    }

    TetMesh() {}
    TetMesh(const std::string& filename_base) { load(filename_base); }

    bool load(const std::string& filename_base); // filename without file extension
    bool empty() const { return vertices_.empty(); }
    void init(const tetgenio& in);

private:
    void calc_neighbor();
    void remove_unused_vertices();
};
