#pragma once

#include "Constraint.h"
#include "Vector.h"

struct EventHandler {
    enum DragMode {
        DRAGMODE_NONE = 0,
        DRAGMODE_SELECT,
        DRAGMODE_MANIPULATE,
        DRAGMODE_TEMP,
    } m_dragMode;
    Y::Vector2d m_select_pos[2];
    Y::Vector2d m_manipulate_prevPos;
    int m_currentConstraintID;
    EventHandler();
    void glut_mouse(int button, int state, int x, int y);
    void glut_motion(int x, int y);
    void glut_keyboard (unsigned char key, int x, int y);
    void glut_special (int key, int x, int y);
};
