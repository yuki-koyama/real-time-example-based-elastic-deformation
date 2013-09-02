#include "Core.h"

#include <Y/GLUtil.h>
#include <Y/ShaderUtil.h>

#include "Particle.h"
#include "Wall.h"
#include "tinyxml.h"

#include "params.h"

#define VERTEX_SHADER ("Wall.vs")
#define FRAGMENT_SHADER ("Wall.fs")
#define SHADOWMAP_VERTEX_SHADER ("WallShadow.vs")
#define SHADOWMAP_FRAGMENT_SHADER ("WallShadow.fs")
#define UNIFORM_SHADOWMAP ("shadowMap")
#define DEFAULT_SHADER (0)

#define WALL_QUALITY 0.05

using namespace std;

namespace {
    Core& core = Core::getInstance();
}

Wall::Coef::Coef()
: m_reflection(0.6f)
, m_friction(1.0f)
, m_repulsion(2.0f)
{}

void Wall::processCollision(Particle& p) const {
    // simple collision processing
    Y::Vector3f position = m_position.convert<float>();
    Y::Vector3f normal = m_normal.convert<float>();
    float dx = normal.dot_product(p.x - position);
    if (0 <= dx)
        return;
    p.x -= dx * normal;
    
    float dv = normal.dot_product(p.v);
    Y::Vector3f v_normal = dv * normal;
    p.v -= m_coef.m_reflection * v_normal;
    p.v *= (1.0f - m_coef.m_friction);
    p.f += m_coef.m_repulsion * normal;
    
}
bool Wall::loadXML(const string& fname) {
    TiXmlDocument doc;
    if (!doc.LoadFile(fname.c_str()))
        return false;
    
    TiXmlElement* txe_wall = doc.FirstChildElement("wall");
    
    if (!loadXML(txe_wall))
        return false;

    return true;
}
bool Wall::loadXML(const TiXmlElement* txe_wall) {
    if (!txe_wall)
        return false;
    
    txe_wall->QueryDoubleAttribute("normal_x", &m_normal.x_);
    txe_wall->QueryDoubleAttribute("normal_y", &m_normal.y_);
    txe_wall->QueryDoubleAttribute("normal_z", &m_normal.z_);
    if (m_normal.length() == 0)
        return false;
    m_normal.normalize();
    txe_wall->QueryDoubleAttribute("distance", &m_distance);
    txe_wall->QueryDoubleAttribute("color_r", &m_color.x_);
    txe_wall->QueryDoubleAttribute("color_g", &m_color.y_);
    txe_wall->QueryDoubleAttribute("color_b", &m_color.z_);
    txe_wall->QueryFloatAttribute("reflection", &m_coef.m_reflection);
    txe_wall->QueryFloatAttribute("friction", &m_coef.m_friction);
    txe_wall->QueryFloatAttribute("repulsion", &m_coef.m_repulsion);
    
    update();
    
    return true;
}
bool Wall::saveXML(const string& fname) const {
    TiXmlDocument doc;
    
    TiXmlElement* txe_wall = new TiXmlElement("wall");
    doc.LinkEndChild(txe_wall);
    
    if (!saveXML(txe_wall))
        return false;
    
    if (!doc.SaveFile(fname.c_str()))
        return false;
    
    return true;
}
bool Wall::saveXML(TiXmlElement* txe_wall) const {
    txe_wall->SetDoubleAttribute("normal_x", m_normal.x_);
    txe_wall->SetDoubleAttribute("normal_y", m_normal.y_);
    txe_wall->SetDoubleAttribute("normal_z", m_normal.z_);
    txe_wall->SetDoubleAttribute("distance", m_distance);
    txe_wall->SetDoubleAttribute("color_r", m_color.x_);
    txe_wall->SetDoubleAttribute("color_g", m_color.y_);
    txe_wall->SetDoubleAttribute("color_b", m_color.z_);
    txe_wall->SetDoubleAttribute("reflection", m_coef.m_reflection);
    txe_wall->SetDoubleAttribute("friction", m_coef.m_friction);
    txe_wall->SetDoubleAttribute("repulsion", m_coef.m_repulsion);
    return true;
}
void Wall::setFrame() {
    double maxCoord = 0;
    int    maxCoord_axis = -1;
    for (int i = 0; i < 3; ++i) {
        double coord = abs(m_normal[i]);
        if (maxCoord < coord) {
            maxCoord = coord;
            maxCoord_axis = i;
        }
    }
    m_frame[0].set(0, 0, 0);
    m_frame[0][(maxCoord_axis + 1) % 3] = 1;
    m_frame[0] -= (m_normal | m_frame[0]) * m_normal;
    m_frame[0].normalize();
    m_frame[1] = m_normal % m_frame[0];
}

// temp
extern void glVertex3(Y::Vector<double, 3> vec);
void glVertex3(Y::Vector<double, 3> vec) {
    glVertex3d(vec[0], vec[1], vec[2]);
}

void Wall::render(bool shadowMapping) const {
    const double R = core.m_config.WALL_SIZE;
    const double d = R * WALL_QUALITY;
        
    if (shadowMapping) {
        glUseProgram(shadowMappingShaderProgram);
        glBegin(GL_QUADS);
        for (double dx0 = -R; dx0 < R; dx0 += d) {
            for (double dy0 = -R; dy0 < R; dy0 += d) {
                double dx1 = dx0 + d, dy1 = dy0 + d;
                glVertex3(m_position + dx0 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy1 * m_frame[1]);
                glVertex3(m_position + dx0 * m_frame[0] + dy1 * m_frame[1]);
            }
        }
        glEnd();
    } else if (core.m_drawer.showGrid) {
        glUseProgram(shaderProgram);
        glUniform1f(contrastLocation, core.m_gui.m_effect.m_fixed_contrast);
        glUniform1f(lightnessLocation, core.m_gui.m_effect.m_fixed_lightness);
        float color[2][3] = {{1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 1.0f}};
        glBegin(GL_QUADS);
        glNormal3d(m_normal[0], m_normal[1], m_normal[2]);
        int i = 0;
        int j = 0;

        for (double dx0 = -R; dx0 < R; dx0 += d) {
            i = j;
            for (double dy0 = -R; dy0 < R; dy0 += d) {
                double dx1 = dx0 + d, dy1 = dy0 + d;
                glColor3fv(color[i]);
                glVertex3(m_position + dx0 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy1 * m_frame[1]);
                glVertex3(m_position + dx0 * m_frame[0] + dy1 * m_frame[1]);
                ++ i;
                i %= 2;
            }
            ++ j;
            j %= 2;
        }
                    
        glEnd();
    } else {
        glUseProgram(shaderProgram);
        glUniform1f(contrastLocation, core.m_gui.m_effect.m_fixed_contrast);
        glUniform1f(lightnessLocation, core.m_gui.m_effect.m_fixed_lightness);
        glColor3d(m_color[0], m_color[1], m_color[2]);
        glBegin(GL_QUADS);
        glNormal3d(m_normal[0], m_normal[1], m_normal[2]);
        for (double dx0 = -R; dx0 < R; dx0 += d) {
            for (double dy0 = -R; dy0 < R; dy0 += d) {
                double dx1 = dx0 + d, dy1 = dy0 + d;
                glVertex3(m_position + dx0 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy0 * m_frame[1]);
                glVertex3(m_position + dx1 * m_frame[0] + dy1 * m_frame[1]);
                glVertex3(m_position + dx0 * m_frame[0] + dy1 * m_frame[1]);
            }
        }
        glEnd();
    }
    glUseProgram(DEFAULT_SHADER);
}

void Wall::initShaders() {
    std::vector<std::string> macros;
    macros.push_back(std::string("COLOR_ADJUSTMENT"));
    std::string vtxShdName(VERTEX_SHADER);
    std::string frgShdName(FRAGMENT_SHADER);
    Y::loadShader(vtxShdName, frgShdName, &shaderProgram, macros);
    
    std::string shadowVtxShdName(SHADOWMAP_VERTEX_SHADER);
    std::string shadowFrgShdName(SHADOWMAP_FRAGMENT_SHADER);
    Y::loadShader(shadowVtxShdName, shadowFrgShdName, &shadowMappingShaderProgram);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, UNIFORM_SHADOWMAP), 1);
    contrastLocation = glGetUniformLocation(shaderProgram, "contrast");
    lightnessLocation = glGetUniformLocation(shaderProgram, "lightness");
    glUseProgram(DEFAULT_SHADER);
}

Wall::Wall() {
    m_normal.y_ = 1;
    m_distance = 0;
    m_color.set(1.0, 1.0, 1.0);
    initShaders();
    update();
}

Wall::Wall(const Y::Vector3d& normal, double distance, const Y::Vector3d& color)
: m_normal(normal)
, m_distance(0)
, m_color(color)
{
    initShaders();
    update();
}
