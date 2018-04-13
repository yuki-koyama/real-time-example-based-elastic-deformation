#include "Core.h"
#include "Scene.h"
using namespace std;

namespace {
    Core& core = Core::getInstance();
}

bool Scene::loadXML(const string& fname) {
    TiXmlDocument doc;
    if (!doc.LoadFile(fname.c_str()))
        return false;
    
    TiXmlElement* txe_scene = doc.FirstChildElement("scene");
    if (!txe_scene)
        return false;
    
    {   // objects
        int n_objects;
        txe_scene->QueryIntAttribute("n_objects", &n_objects);
        m_smObjects_play_template.resize(n_objects);
        TiXmlElement* txe_object = txe_scene->FirstChildElement("object");
        for (int i = 0; i < n_objects; ++i, txe_object = txe_object->NextSiblingElement())
            if (!m_smObjects_play_template[i].loadXML(txe_object))
                return false;
    }
    
    {   // walls
        int n_walls;
        txe_scene->QueryIntAttribute("n_walls", &n_walls);
        m_walls.resize(n_walls);
        TiXmlElement* txe_wall = txe_scene->FirstChildElement("wall");
        for (int i = 0; i < n_walls; ++i, txe_wall = txe_wall->NextSiblingElement())
            if (!m_walls[i].loadXML(txe_wall))
                return false;
    }
    TiXmlElement* txe_lightPos = txe_scene->FirstChildElement("lightPos");
    if (txe_lightPos) {
        double x, y, z;
        txe_lightPos->QueryDoubleAttribute("x", &x);
        txe_lightPos->QueryDoubleAttribute("y", &y);
        txe_lightPos->QueryDoubleAttribute("z", &z);
        m_lightPos.set(x, y, z);
    }
    
    return true;
}
bool Scene::saveXML(const string& fname) const {
    TiXmlDocument doc;
    
    TiXmlElement* txe_scene = new TiXmlElement("scene");
    doc.LinkEndChild(txe_scene);
    
    {   // objects
        int n_objects = (int)m_smObjects_play_template.size();
        txe_scene->SetAttribute("n_objects", n_objects);
        for (int i = 0; i < n_objects; ++i) {
            TiXmlElement* txe_object = new TiXmlElement("object");
            txe_scene->LinkEndChild(txe_object);
            if (!m_smObjects_play_template[i].saveXML(txe_object))
                return false;
        }
    }
    {   // walls
        int n_walls = (int)m_walls.size();
        txe_scene->SetAttribute("n_walls", n_walls);
        for (int i = 0; i < n_walls; ++i) {
            TiXmlElement* txe_wall = new TiXmlElement("wall");
            txe_scene->LinkEndChild(txe_wall);
            if (!m_walls[i].saveXML(txe_wall))
                return false;
        }
    }
    TiXmlElement* txe_lightPos = new TiXmlElement("lightPos");
    txe_scene->LinkEndChild(txe_lightPos);
    txe_lightPos->SetDoubleAttribute("x", m_lightPos.x_);
    txe_lightPos->SetDoubleAttribute("y", m_lightPos.y_);
    txe_lightPos->SetDoubleAttribute("z", m_lightPos.z_);
    
    if (!doc.SaveFile(fname.c_str()))
        return false;
    
    return true;
}
void Scene::simulateOneStep(float dt) {
    if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR) {
        m_smObject_author.simulateOneStep(dt);
    } else if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_PLAY) {
        for (list<ShapeMatchingObject>::iterator smObject = m_smObjects_play.begin(); smObject != m_smObjects_play.end();) {
            if (smObject->m_lifetime == 0) {
                smObject = m_smObjects_play.erase(smObject);
            } else {
                smObject->simulateOneStep(dt);
                ++smObject;
            }
        }
    }
}
void Scene::reset() {
    if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR)
        m_smObject_author.reset();
    else if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_PLAY)
        m_smObjects_play.clear();
}
void Scene::play_instantiate() {
    printf("play_instantiate\n");
    static int index = 0;
    m_smObjects_play.push_back(m_smObjects_play_template[index]);
    ShapeMatchingObject& object = m_smObjects_play.back();
    object.reset();
    object.embedTriMesh();
    object.trimesh.setBufferObject();
    object.m_lifetime = core.m_gui.m_play.m_lifetime;
    index = (index + 1) % m_smObjects_play_template.size();
}
Scene::Scene()
: m_isPause(false)
, m_useGravity(true)
, m_lightPos(-3.0, 4.0, 6.0)
{
    m_smObject_author.m_scene = this;
}
