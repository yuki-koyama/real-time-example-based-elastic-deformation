#pragma once

#include <string>
#include "Matrix.h"
#include "Vector.h"
#include "tinyxml.h"

class Particle;

struct Wall {
    Y::Vector3d m_normal;
    double m_distance;          // distance from the origin
    Y::Vector3d m_color;
    // auxiliary
    Y::Vector3d m_position;
    Y::Vector3d m_frame[2];
    struct Coef {
        float m_reflection;
        float m_friction;
        float m_repulsion;
        Coef();
    } m_coef;
    void processCollision(Particle& p) const;
    void setPosition() {
        m_position = m_distance * m_normal;
    }
    void setFrame();
    void update() {
        setPosition();
        setFrame();
    }
    bool loadXML(const std::string& fname);
    bool loadXML(const TiXmlElement* txe_wall);
    bool saveXML(const std::string& fname) const;
    bool saveXML(TiXmlElement* txe_wall) const;
    void render(bool shadowMapping) const;
    
    Wall();
    Wall(const Y::Vector3d& normal, double distance, const Y::Vector3d& color);
    
private:
    // for shader program
    void initShaders();
    GLuint shaderProgram;
    GLuint shadowMappingShaderProgram;
    GLuint contrastLocation;
    GLuint lightnessLocation;
};
