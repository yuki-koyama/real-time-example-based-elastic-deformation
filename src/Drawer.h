#pragma once

#include "Trackball.h"

struct Drawer
{
    Y::Trackball m_trackball;
    int          m_width;
    int          m_height;
    double       m_fovy;
    float        m_nearClip;
    float        m_farClip;
    bool         showGrid;

    // for shadowmapping sampling
    Y::Vector3d offset;

    Drawer();
    ~Drawer();
    void init();
    void reshape(int width, int height);
    void draw();
    void deinit() {}

private:
    void scene(bool shadowMapping);
};
