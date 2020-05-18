#include "Config.h"
#include "tinyxml.h"

void Config::set_default()
{
    WINDOW_WIDTH  = 1024;
    WINDOW_HEIGHT = 768;
    SPHERE_RADIUS = 0.05f;
    WALL_SIZE     = 10.0;
    BGCOLOR_BOTTOM.set(0.0, 0.0, 0.0);
    BGCOLOR_TOP.set(0.0, 0.0, 0.0);
}
bool Config::load()
{
    TiXmlDocument doc;
    if (!doc.LoadFile("config.xml")) return false;
    TiXmlElement* elem = doc.FirstChildElement("config");
    if (!elem) return false;

    int flag = 1;
    flag *= elem->QueryIntAttribute("window_width", &WINDOW_WIDTH) == TIXML_SUCCESS;
    flag *= elem->QueryIntAttribute("window_height", &WINDOW_HEIGHT) == TIXML_SUCCESS;
    flag *= elem->QueryDoubleAttribute("wall_size", &WALL_SIZE) == TIXML_SUCCESS;

    TiXmlElement* txe_bgcolor = elem->FirstChildElement("bgcolor");
    if (txe_bgcolor)
    {
        txe_bgcolor->QueryDoubleAttribute("bottom_r", &BGCOLOR_BOTTOM.x_);
        txe_bgcolor->QueryDoubleAttribute("bottom_g", &BGCOLOR_BOTTOM.y_);
        txe_bgcolor->QueryDoubleAttribute("bottom_b", &BGCOLOR_BOTTOM.z_);
        txe_bgcolor->QueryDoubleAttribute("top_r", &BGCOLOR_TOP.x_);
        txe_bgcolor->QueryDoubleAttribute("top_g", &BGCOLOR_TOP.y_);
        txe_bgcolor->QueryDoubleAttribute("top_b", &BGCOLOR_TOP.z_);
    }

    return flag != 0;
}
bool Config::save()
{
    TiXmlDocument doc;
    TiXmlElement* elem = new TiXmlElement("config");
    doc.LinkEndChild(elem);

    elem->SetAttribute("window_width", WINDOW_WIDTH);
    elem->SetAttribute("window_height", WINDOW_HEIGHT);
    elem->SetDoubleAttribute("sphere_radius", SPHERE_RADIUS);
    elem->SetDoubleAttribute("wall_size", WALL_SIZE);

    TiXmlElement* txe_bgcolor = new TiXmlElement("bgcolor");
    elem->LinkEndChild(txe_bgcolor);
    txe_bgcolor->SetDoubleAttribute("bottom_r", BGCOLOR_BOTTOM.x_);
    txe_bgcolor->SetDoubleAttribute("bottom_g", BGCOLOR_BOTTOM.y_);
    txe_bgcolor->SetDoubleAttribute("bottom_b", BGCOLOR_BOTTOM.z_);
    txe_bgcolor->SetDoubleAttribute("top_r", BGCOLOR_TOP.x_);
    txe_bgcolor->SetDoubleAttribute("top_g", BGCOLOR_TOP.y_);
    txe_bgcolor->SetDoubleAttribute("top_b", BGCOLOR_TOP.z_);

    doc.SaveFile("config.xml");

    return true;
}
Config::Config()
{
    set_default();
    if (!load()) save();
}
