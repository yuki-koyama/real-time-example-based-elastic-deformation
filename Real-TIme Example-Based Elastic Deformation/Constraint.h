#pragma once

#include <vector>
#include "Particle.h"

struct Constraint {
    std::vector<Particle*> m_particles;
    Y::Vector3f m_xc;
    void translate(const Y::Vector3f& d);
    void rotate(Y::Vector3f& axis, float angle);
    void scale(float s);
};

