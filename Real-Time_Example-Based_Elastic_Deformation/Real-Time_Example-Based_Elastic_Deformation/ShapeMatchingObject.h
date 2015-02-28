#pragma once

#include "Particle.h"
#include "Cluster.h"
#include <vector>
#include <string>
#include <KLIB/MatrixVL.h>
#include "Clock.h"
#include "TetMesh.h"
#include "TriMesh.h"
#include "tinyxml.h"
#include <time.h>

struct Scene;

class ShapeMatchingObject {
public:
    Scene* m_scene;
    // object attribute
    struct Config {
        float m_damping;
        float m_stiffness_alpha;    // alpha value used in the original shape matching paper [Muller05]
        int   m_stiffness_iter;     // number of iterations in ProcDef [Ijiri09]
        bool m_useLocalExample;
        float m_beta;
        Config();
    } m_config;
    struct Initial {
//        Y::Vector3f m_offset_min;
//        Y::Vector3f m_offset_max;
        Y::Vector3f m_offset;
        Y::Vector3f m_velocity;
    } m_initial;
    int m_lifetime;
    Y::Vector3f m_color;
    
    // performance measure
    struct Performance {
        int m_count;
        double m_timeMax;
        double m_timeMin;
        double m_timeTotal;
        double m_timeAvg;
        void init();
        Y::ClockData m_clkData;
        Y::ClockStart* m_clkStart;
        void addTick(clock_t tick);
        Performance();
    } m_performance;
    
    ShapeMatchingObject();
    
    void reset();
    
    void simulateOneStep(float dt);
    void calculateForce(float dt);
    void calculateVelocity(float dt);
    void applyVelocity(float dt);
    void render(bool shadowMode);
    
    // setup methods
    void precomputeUstatic();
    void precomputeExamples_Umode();
    void precomputeExamples_AtA();
    void precomputeExamples();
    
    // rendering
    void setBufferObject() {}
    void deleteBufferObject() {}
    
    std::vector<Particle> particles;
    std::vector<Cluster> clusters;
    
    int numberOfModes;
    // given n example poses, A := (E_1 - E_n, ..., E_{n-1} - E_n), where E_i is i-th example pose's deformation descriptor (similar to [Martin11])
    // e.g., when numberOfModes is 3, AtA_inv is a 2 by 2 matrix
    
    KLIB::MatrixVLd AtA_inv;
    // note: in the paper, AtA_inv is described as (LtL)^(-1)
    
    bool saveNode(const std::string& node_fname) const;
    bool loadNode(const std::string& node_fname);
    
    // simulation methods
    void calculateStaticVariables();
    void updateParticles();
    void updateClusters();
    void shapeMatching();
    void calculateGoalPositions();
    void multiShapeMatching();
    void dampingGlobal(float dt);
    
    TetMesh tetmesh;
    void buildFromTetMesh();
    TriMesh trimesh;
    virtual void embedTriMesh();
    virtual void updateTriMesh();
    
    // XML including tetmesh, trimesh, and examples
    bool loadXML(const std::string& fname);
    bool loadXML(const TiXmlElement* txe_object);
    bool saveXML(const std::string& fname) const;
    bool saveXML(TiXmlElement* txe_object) const;
    
protected:
    void renderParticles(bool shadowMode);
    virtual void renderTrimesh(bool shadowMode);
};

