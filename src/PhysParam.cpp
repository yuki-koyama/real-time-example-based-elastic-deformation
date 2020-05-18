#include "PhysParam.h"
#include "tinyxml.h"

bool PhysParam::loadXML(const std::string& fname)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(fname.c_str())) return false;

    TiXmlElement* txe_param = doc.FirstChildElement("param");
    if (!txe_param) return false;

    txe_param->QueryIntAttribute("frameFrequency", &m_frameFrequency);
    txe_param->QueryFloatAttribute("dt", &m_dt);
    txe_param->QueryIntAttribute("stepPerFrame", &m_stepPerFrame);
    txe_param->QueryFloatAttribute("airResistance", &m_airResistance);
    txe_param->QueryFloatAttribute("gravity", &m_gravity);
    txe_param->QueryFloatAttribute("springCoef", &m_springCoef);

    return true;
}
bool PhysParam::saveXML(const std::string& fname) const
{
    TiXmlDocument doc;

    TiXmlElement* txe_param = new TiXmlElement("param");
    doc.LinkEndChild(txe_param);

    txe_param->SetAttribute("frameFrequency", m_frameFrequency);
    txe_param->SetDoubleAttribute("dt", m_dt);
    txe_param->SetAttribute("stepPerFrame", m_stepPerFrame);
    txe_param->SetDoubleAttribute("airResistance", m_airResistance);
    txe_param->SetDoubleAttribute("gravity", m_gravity);
    txe_param->SetDoubleAttribute("springCoef", m_springCoef);

    if (!doc.SaveFile(fname.c_str())) return false;

    return true;
}
PhysParam::PhysParam()
    : m_frameFrequency(1000 / 30),
      m_dt(1.0f / 300.0f),
      m_stepPerFrame(5),
      m_airResistance(0.010f),
      m_gravity(9.8f),
      m_springCoef(300.0f),
      m_mass(1.0f)
{
}
