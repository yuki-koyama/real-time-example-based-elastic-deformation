#pragma once

#include <Y/Vector.h>
#include <string>

struct Config {
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    double WALL_SIZE;
    float SPHERE_RADIUS;
    Y::Vector3d BGCOLOR_TOP;
    Y::Vector3d BGCOLOR_BOTTOM;
    
    bool load();
    bool save();
    void set_default();
    
    Config();
};
