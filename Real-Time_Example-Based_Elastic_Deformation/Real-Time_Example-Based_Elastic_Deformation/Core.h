#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glfw.h>
#include "Gui.h"
#include "Drawer.h"
#include "EventHandler.h"
#include "Config.h"
#include "PhysParam.h"
#include "Scene.h"
#include "ShapeMatchingObject.h"
#include "Constraint.h"
#include "ConstraintTemp.h"
#include <string>
#include "Clock.h"

struct Core {
    static Core& getInstance() {
        static Core p;
        return p;
    }
    Gui m_gui;
    Drawer m_drawer;
    EventHandler m_eventHandler;
    Config m_config;
    PhysParam m_physParam;
    std::vector<Constraint> m_constraints;
    ConstraintTemp m_constraintTemp;
    Y::ClockData m_clkData;
    
    Scene m_scene;
    
    Core();
};
