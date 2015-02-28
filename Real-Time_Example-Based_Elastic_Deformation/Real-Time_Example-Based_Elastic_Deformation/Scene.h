#pragma once

#include "Wall.h"
#include "ShapeMatchingObject.h"
#include "Vector.h"
#include <vector>

struct Scene {
    std::vector<Wall> m_walls;
    ShapeMatchingObject m_smObject_author;
    std::list<ShapeMatchingObject> m_smObjects_play;
    std::vector<ShapeMatchingObject> m_smObjects_play_template;
    Y::Vector3f m_lightPos;
    bool m_isPause;
    bool m_useGravity;
    void simulateOneStep(float dt);
    void reset();
    void play_instantiate();
    bool loadXML(const std::string& fname);
    bool saveXML(const std::string& fname) const;
    Scene();
};
