#pragma once

#include "tinyxml.h"
#include <string>

struct PhysParam {
    int m_frameFrequency;
    float m_dt;
    int m_stepPerFrame;
    float m_airResistance;
    float m_gravity;
    float m_springCoef;
    float m_mass;
    bool loadXML(const std::string& fname);
    bool saveXML(const std::string& fname) const;
    PhysParam();
};
