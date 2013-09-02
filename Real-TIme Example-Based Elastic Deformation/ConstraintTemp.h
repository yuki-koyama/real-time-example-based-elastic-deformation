#pragma once

#include <Y/Vector.h>

class Particle;

struct ConstraintTemp {
    Particle* m_particle;
    std::vector<Particle*> m_neighborParticles;
    Y::Vector3f m_constraintPos;
    ConstraintTemp()
        : m_particle(0)
    {}
};
