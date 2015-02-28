#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <vector>

class Cluster;

class Particle {
public:
    float m;
    Y::Vector3f x;
    Y::Vector3f v;
    Y::Vector3f f;
    Y::Vector3f x0;
    Y::Matrix3x3f mxx0t;
    int main_cluster_id;
    std::vector<int> cluster_ids;
    std::vector<Y::Vector3f> gxs;
    Y::Vector3f gx;
    int id;
    int constraintID;
    Y::Vector3f constraintPos;
    std::vector<Y::Vector3f> x_example;
    Y::Vector3f x_tmp;
    
    Particle(Y::Vector3f x = Y::Vector3f(0.0f, 0.0f, 0.0f),
             Y::Vector3f v = Y::Vector3f(0.0f, 0.0f, 0.0f),
             Y::Vector3f f = Y::Vector3f(0.0f, 0.0f, 0.0f),
             float m = 1.0f);
    
    void updateMxx0t();

    void renderParticle(bool shadowMode = false);
};

