#pragma once

#include "Particle.h"
#include "Vector.h"
#include <KLIB/MatrixVL.h>

class ShapeMatchingObject;

class Cluster {
public:
    float sumOfMass;
    Y::Vector3f xc;
    Y::Vector3f xc0;
    std::vector<int> particle_ids;
    std::vector<Y::Vector3f> particle_gxs;
    Y::Matrix3x3f Eigen;
    
    Y::Matrix3x3f A;
    
    // for our method
    Y::Vector6f U;
    Y::Matrix3x3f R;
    std::vector<Y::Vector6f> Umode;
    Y::Matrix3x3f Ustatic;
    int id;
    KLIB::MatrixVLd AtA_inv;        // projection matrix for local examples
    
    Cluster();
    ~Cluster();
    void calculateSumOfMass(ShapeMatchingObject* object);
    void calculateRestCenterPoint(ShapeMatchingObject* object);
    void updateCenterPoint(ShapeMatchingObject* object);
    Y::Matrix3x3f getMomentMatrix(ShapeMatchingObject* object);
};
