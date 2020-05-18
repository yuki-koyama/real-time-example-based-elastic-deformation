#include "TetMesh.h"
#include "Core.h"
#include <algorithm>
#include <fstream>
#include <tetgen.h>

using namespace std;

bool TetMesh::load(const string& filename_base)
{
    tetgenio in;
    if (!in.load_tetmesh(const_cast<char*>(filename_base.c_str()))) return false;
    init(in);
    return true;
}
void TetMesh::init(const tetgenio& in)
{
    vertices_.resize(in.numberofpoints);
    for (int i = 0; i < in.numberofpoints; ++i)
    {
        Vertex& vertex = vertices_[i];
        for (int j = 0; j < 3; ++j)
            vertex.position_.ptr()[j] = static_cast<float>(in.pointlist[3 * i + j]);
    }
    tetras_.resize(in.numberoftetrahedra);
    for (int i = 0; i < in.numberoftetrahedra; ++i)
    {
        Tetra& tetra = tetras_[i];
        for (int j = 0; j < 4; ++j)
            tetra.vid_[j] = in.tetrahedronlist[4 * i + j - in.firstnumber];
    }
    calc_neighbor();
    unused_vertices_.clear();
    unused_indices_.clear();
    remove_unused_vertices();
}
void TetMesh::remove_unused_vertices()
{
    size_t n         = vertices_.size();
    unused_vertices_ = vertices_;
    vertices_.clear();
    unused_indices_.clear();
    unused_indices_.resize(n, -1);
    for (int i = 0; i < n; ++i)
    {
        Vertex& v = unused_vertices_[i];
        if (v.neighbor_vid_.empty()) continue;
        unused_indices_[i] = static_cast<int>(vertices_.size());
        vertices_.push_back(v);
    }
    if (vertices_.size() == n)
    {
        unused_vertices_.clear();
        unused_indices_.clear();
        return;
    }
    for (vector<Tetra>::iterator tetra = tetras_.begin(); tetra != tetras_.end(); ++tetra)
    {
        for (int i = 0; i < 4; ++i)
            tetra->vid_[i] = unused_indices_[tetra->vid_[i]];
    }
    for (vector<Vertex>::iterator v = vertices_.begin(); v != vertices_.end(); ++v)
        for (list<int>::iterator neighbor_vid = v->neighbor_vid_.begin(); neighbor_vid != v->neighbor_vid_.end();
             ++neighbor_vid)
            *neighbor_vid = unused_indices_[*neighbor_vid];
}

void TetMesh::calc_neighbor()
{
    for (int i = 0; i < vertices_.size(); ++i)
        vertices_[i].neighbor_vid_.clear();

    for (int i = 0; i < tetras_.size(); ++i)
    {
        Tetra& t = tetras_[i];
        for (int j = 0; j < 4; ++j)
        {
            Vertex& v = vertices_[t.vid_[j]];
            for (int k = 0; k < 3; ++k)
                v.neighbor_vid_.push_back(t.vid_[(j + 1 + k) % 4]);
        }
    }

    for (int i = 0; i < vertices_.size(); ++i)
    {
        Vertex& v = vertices_[i];
        v.neighbor_vid_.sort();
        v.neighbor_vid_.unique();
    }
}
