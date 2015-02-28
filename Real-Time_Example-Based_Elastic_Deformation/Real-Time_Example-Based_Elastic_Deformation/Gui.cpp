#include "Core.h"
#include <iostream>
#include <iterator>

#define SIMPLE_MODE 1
#define DEMO_MODE   0

using namespace std;

namespace {
    Core& core = Core::getInstance();
    Gui& gui = core.m_gui;
    ShapeMatchingObject& smObject_author = core.m_scene.m_smObject_author;
}

extern void TW_CALL CBGet_stdstring(void *value, void *clientData);

// [begin] AntTweakBar callback functions -------------------------------------------------------------------
void TW_CALL CBGet_stdstring(void *value, void *clientData) {
    TwCopyStdStringToLibrary(*reinterpret_cast<string*>(value), *reinterpret_cast<string*>(clientData));
}
// general setting
void TW_CALL Gui::General::CBSet_render_lightPos(const void *value, void *clientData) {
    core.m_scene.m_lightPos = *reinterpret_cast<const Y::Vector3f*>(value);
    for (vector<Wall>::iterator wall = core.m_scene.m_walls.begin(); wall != core.m_scene.m_walls.end(); ++wall)
        wall->update();
}
void TW_CALL Gui::General::CBGet_render_lightPos(void *value, void *clientData) {
    *reinterpret_cast<Y::Vector3f*>(value) = core.m_scene.m_lightPos;
}
void TW_CALL Gui::General::CB_simulation_step(void *) {
    if (!core.m_scene.m_isPause)
        return;
    core.m_scene.simulateOneStep(core.m_physParam.m_dt);
}
void TW_CALL Gui::General::CB_simulation_reset(void *) {
    core.m_scene.reset();
}
void TW_CALL Gui::General::CB_wall_add(void *) {
    core.m_scene.m_walls.push_back(Wall());
}
void TW_CALL Gui::General::CBSet_wall_normal(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    wall.m_normal = *reinterpret_cast<const Y::Vector3d*>(value);
    wall.m_normal.normalize();
    wall.update();
}
void TW_CALL Gui::General::CBGet_wall_normal(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    *reinterpret_cast<Y::Vector3d*>(value) = core.m_scene.m_walls.back().m_normal;
}
void TW_CALL Gui::General::CBSet_wall_distance(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    wall.m_distance = *reinterpret_cast<const double*>(value);
    wall.update();
}
void TW_CALL Gui::General::CBGet_wall_distance(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    *reinterpret_cast<double*>(value) = core.m_scene.m_walls.back().m_distance;
}
void TW_CALL Gui::General::CBSet_wall_color(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    Y::Vector3f color = *reinterpret_cast<const Y::Vector3f*>(value);
    wall.m_color = color.convert<double>();
}
void TW_CALL Gui::General::CBGet_wall_color(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Y::Vector3f color =  core.m_scene.m_walls.back().m_color.convert<float>();
    *reinterpret_cast<Y::Vector3f*>(value) = color;
}
void TW_CALL Gui::General::CBSet_wall_reflection(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    wall.m_coef.m_reflection = *reinterpret_cast<const float*>(value);
}
void TW_CALL Gui::General::CBGet_wall_reflection(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    *reinterpret_cast<float*>(value) = core.m_scene.m_walls.back().m_coef.m_reflection;
}
void TW_CALL Gui::General::CBSet_wall_friction(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    wall.m_coef.m_friction = *reinterpret_cast<const float*>(value);
}
void TW_CALL Gui::General::CBGet_wall_friction(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    *reinterpret_cast<float*>(value) = core.m_scene.m_walls.back().m_coef.m_friction;
}
void TW_CALL Gui::General::CBSet_wall_repulsion(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    wall.m_coef.m_repulsion = *reinterpret_cast<const float*>(value);
}
void TW_CALL Gui::General::CBGet_wall_repulsion(void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    *reinterpret_cast<float*>(value) = core.m_scene.m_walls.back().m_coef.m_repulsion;
}
void TW_CALL Gui::General::CBSet_wall_load(const void *value, void *clientData) {
    gui.m_general.m_wall_fname_load = *reinterpret_cast<const string*>(value);
    if (gui.m_general.m_wall_fname_load.empty())
        return;
    Wall wall;
    if (!wall.loadXML(gui.m_general.m_wall_fname_load + ".xml"))
        return;
    core.m_scene.m_walls.push_back(wall);
    gui.m_general.m_wall_fname_load.clear();
}
void TW_CALL Gui::General::CBSet_wall_save(const void *value, void *clientData) {
    if (core.m_scene.m_walls.empty())
        return;
    gui.m_general.m_wall_fname_save = *reinterpret_cast<const string*>(value);
    if (gui.m_general.m_wall_fname_save.empty())
        return;
    Wall& wall = core.m_scene.m_walls.back();
    if (!wall.saveXML(gui.m_general.m_wall_fname_save + ".xml"))
        return;
    gui.m_general.m_wall_fname_save.clear();
}
void TW_CALL Gui::General::CB_wall_delete(void *) {
    if (!core.m_scene.m_walls.empty())
        core.m_scene.m_walls.pop_back();
}
// authoring mode
void TW_CALL Gui::Author::CBSet_xml_load(const void *value, void *clientData) {
    gui.m_author.m_xml_load_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_author.m_xml_load_fname.empty())
        return;
    if (!smObject_author.loadXML(gui.m_author.m_xml_load_fname + ".xml"))
        return;
    gui.m_author.m_xml_load_fname.clear();
}
void TW_CALL Gui::Author::CBSet_xml_save(const void *value, void *clientData) {
    gui.m_author.m_xml_save_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_author.m_xml_save_fname.empty())
        return;
    smObject_author.saveXML(gui.m_author.m_xml_save_fname + ".xml");
    gui.m_author.m_xml_save_fname.clear();
}
void TW_CALL Gui::Author::CBSet_tetmesh(const void *value, void *clientData) {
    gui.m_author.m_tetmesh_fname = *reinterpret_cast<const string*>(value);
    if (!smObject_author.tetmesh.load(gui.m_author.m_tetmesh_fname))
        return;
    smObject_author.buildFromTetMesh();
    gui.m_author.m_tetmesh_fname.clear();
}
void TW_CALL Gui::Author::CBSet_trimesh(const void *value, void *clientData) {
    if (smObject_author.tetmesh.empty()) {
        return;
    }
    gui.m_author.m_trimesh_fname = *reinterpret_cast<const string*>(value);
    if (!smObject_author.trimesh.load(gui.m_author.m_trimesh_fname)) {
        return;
    }
    smObject_author.embedTriMesh();
    gui.m_author.m_trimesh_fname.clear();
}
void TW_CALL Gui::Author::CB_constraint_delete(void *) {
    int& currentConstraintID = core.m_eventHandler.m_currentConstraintID;
    if (currentConstraintID == -1)
        return;
    vector<Constraint>::iterator iter = core.m_constraints.begin();
    advance(iter, currentConstraintID);
    Constraint& c = *iter;
    for (int i = 0; i < c.m_particles.size(); ++i) {
        Particle* p = c.m_particles[i];
        p->v = p->f = Y::Vector3f(0.0, 0.0, 0.0);
        p->constraintID = -1;
    }
    core.m_constraints.erase(iter);
    for (int i = 0; i < smObject_author.particles.size(); ++i) {
        Particle& p = smObject_author.particles[i];
        if (currentConstraintID < p.constraintID)
            --p.constraintID;
    }
    currentConstraintID = -1;
}
void TW_CALL Gui::Author::CB_example_add(void *) {
    for (int i = 0; i < smObject_author.particles.size(); ++i) {
        Particle& p = smObject_author.particles[i];
        p.x_example.push_back(p.x);
    }
    smObject_author.precomputeExamples();
}
void TW_CALL Gui::Author::CB_example_delete(void *) {
    if (smObject_author.particles.front().x_example.size() == 1)
        return;
    for (int i = 0; i < smObject_author.particles.size(); ++i)
        smObject_author.particles[i].x_example.pop_back();
    smObject_author.precomputeExamples();
}
void TW_CALL Gui::Author::CBSet_example_tetnode_load(const void *value, void *clientData) {
    if (smObject_author.tetmesh.empty())
        return;
    gui.m_author.m_example_tetnode_load_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_author.m_example_tetnode_load_fname.empty())
        return;
    if (!smObject_author.loadNode(gui.m_author.m_example_tetnode_load_fname))
        return;
    gui.m_author.m_example_tetnode_load_fname.clear();
}
void TW_CALL Gui::Author::CBSet_example_tetnode_save(const void *value, void *clientData) {
    if (smObject_author.tetmesh.empty())
        return;
    gui.m_author.m_example_tetnode_save_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_author.m_example_tetnode_save_fname.empty())
        return;
    if (!smObject_author.saveNode(gui.m_author.m_example_tetnode_save_fname))
        return;
    gui.m_author.m_example_tetnode_save_fname.clear();
}
// playing mode
void TW_CALL Gui::Play::CBSet_scene_load(const void *value, void *clientData) {
    gui.m_play.m_scene_load_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_general.m_demoMode != Gui::General::DEMOMODE_PLAY || gui.m_play.m_scene_load_fname.empty())
        return;
    if (!core.m_scene.loadXML(gui.m_play.m_scene_load_fname + ".xml"))
        return;
    gui.m_play.m_scene_load_fname.clear();
}
void TW_CALL Gui::Play::CBSet_scene_save(const void *value, void *clientData) {
    gui.m_play.m_scene_save_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_general.m_demoMode != Gui::General::DEMOMODE_PLAY || gui.m_play.m_scene_save_fname.empty())
        return;
    if (!core.m_scene.saveXML(gui.m_play.m_scene_save_fname + ".xml"))
        return;
    gui.m_play.m_scene_save_fname.clear();
}
void TW_CALL Gui::Play::CBSet_object_load(const void *value, void *clientData) {
    gui.m_play.m_object_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_general.m_demoMode != Gui::General::DEMOMODE_PLAY || gui.m_play.m_object_fname.empty())
        return;
    ShapeMatchingObject object;
    if (!object.loadXML(gui.m_play.m_object_fname + ".xml"))
        return;
    object.m_scene = &core.m_scene;
    core.m_scene.m_smObjects_play_template.push_back(object);
    object.m_lifetime = core.m_gui.m_play.m_lifetime;
    core.m_scene.m_smObjects_play.push_back(object);
    gui.m_play.m_object_fname.clear();
}
void TW_CALL Gui::Play::CB_object_delete(void *) {
    if (!core.m_scene.m_smObjects_play_template.empty()) {
        core.m_scene.m_smObjects_play_template.pop_back();
    }
}
void TW_CALL Gui::Play::CB_instantiate(void *) {
    if (!core.m_scene.m_smObjects_play_template.empty()) {
        core.m_scene.play_instantiate();
    }
}
void TW_CALL Gui::Play::CB_instantiates(void *) {
    for (int i = 0; i < core.m_scene.m_smObjects_play_template.size(); ++ i) {
        core.m_scene.play_instantiate();
    }
}
// param
void TW_CALL Gui::Param::CBSet_load(const void *value, void *clientData) {
    gui.m_param.m_load_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_param.m_load_fname .empty())
        return;
    if (!core.m_physParam.loadXML(gui.m_param.m_load_fname + ".xml"))
        return;
    gui.m_param.m_load_fname.clear();
}
void TW_CALL Gui::Param::CBSet_save(const void *value, void *clientData) {
    gui.m_param.m_save_fname = *reinterpret_cast<const string*>(value);
    if (gui.m_param.m_save_fname .empty())
        return;
    if (!core.m_physParam.saveXML(gui.m_param.m_save_fname + ".xml"))
        return;
    gui.m_param.m_save_fname.clear();
}
void TW_CALL Gui::Param::CBSet_mass(const void *value, void *clientData) {
    core.m_physParam.m_mass = *reinterpret_cast<const float*>(value);
    
    if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR) {
        ShapeMatchingObject& sm = core.m_scene.m_smObject_author;
        for (int i = 0; i < sm.tetmesh.vertices_.size(); i ++) {
            TetMesh::Vertex& vertex = sm.tetmesh.vertices_[i];
            Particle& p = sm.particles[i];
            p.m = core.m_physParam.m_mass / (vertex.neighbor_vid_.size() + 1.0f);
            printf("p.m = %f\n", p.m);
        }
        sm.calculateStaticVariables();
        sm.precomputeUstatic();
        sm.precomputeExamples();
        
    } else if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_PLAY) {
        for (list<ShapeMatchingObject>::iterator smObject = core.m_scene.m_smObjects_play.begin(); smObject != core.m_scene.m_smObjects_play.end();) {
            for (int i = 0; i < smObject->tetmesh.vertices_.size(); i ++) {
                TetMesh::Vertex& vertex = smObject->tetmesh.vertices_[i];
                Particle& p = smObject->particles[i];
                p.m = core.m_physParam.m_mass / (vertex.neighbor_vid_.size() + 1.0f);
            }
            smObject->calculateStaticVariables();
            smObject->precomputeUstatic();
            smObject->precomputeExamples();
        }
    }
}
void TW_CALL Gui::Param::CBGet_mass(void *value, void *clientData) {
    *reinterpret_cast<float*>(value) = core.m_physParam.m_mass;
}


// tmp
void TW_CALL Gui::Tmp::CB_test(void *) {
    for (int i = 0; i < smObject_author.particles.size(); ++i) {
        Particle& p = smObject_author.particles[i];
        p.x = p.x_example.back();
        p.v = Y::Vector3f(0.0, 0.0, 0.0);
        p.f = Y::Vector3f(0.0, 0.0, 0.0);
    }
    smObject_author.updateTriMesh();
}
// [end] AntTweakBar callback functions -------------------------------------------------------------------


void Gui::init() {
    TwDefine("GLOBAL iconpos=bottomright"); 
    m_general.init();
    m_author.init();
    m_play.init();
    m_param.init();
    m_tmp.init();
    m_effect.init();
    m_camera.init();
}
void Gui::General::init() {
    m_bar = TwNewBar("General");
#if SIMPLE_MODE
    TwDefine("General label='general setting' size='250 220' position='0 0'");
#else
#if DEMO_MODE
    TwDefine("General label='general setting' size='250 320' position='0 0' iconified=true");
#else
    TwDefine("General label='general setting' size='250 320' position='0 0'");
#endif
#endif
    
    TwAddVarRW(m_bar, "demomode", TwDefineEnum("DemoModeType", 0, 0), &m_demoMode, "label='demo mode' enum='0 {authoring mode}, 1 {playing mode}'");
    TwAddVarRW(m_bar, "render_mode", TwDefineEnum("RenderModeType", 0, 0), &m_renderMode, "group='render' label='mode' enum='0 {both}, 1 {particle}, 2 {trimesh}' key=v");
#if !SIMPLE_MODE
    TwAddVarRW(m_bar, "render_showWorldAxis", TW_TYPE_BOOLCPP, &m_showWorldAxis, "group='render' label='world axis'");
#endif
    TwAddVarCB(m_bar, "render_lightPos", TW_TYPE_DIR3F, CBSet_render_lightPos, CBGet_render_lightPos, 0, "group='render' label='light pos'");
    TwAddVarRW(m_bar, "simulation_pause", TW_TYPE_BOOLCPP, &core.m_scene.m_isPause, "group='simulation' label='run/pause' true='paused' false='running' key=p");
    TwAddButton(m_bar, "simulation_step", CB_simulation_step, 0, "group='simulation' label='>>> step'   key=s");
    TwAddButton(m_bar, "simulation_reset", CB_simulation_reset, 0, "group='simulation' label='>>> reset'  key=r");
    TwAddVarRW(m_bar, "simulation_useGravity", TW_TYPE_BOOLCPP, &core.m_scene.m_useGravity, "group='simulation' label='gravity' key=g");
#if !SIMPLE_MODE
    TwAddVarRW(m_bar, "wall_showGrid", TW_TYPE_BOOLCPP, &core.m_drawer.showGrid, "group='wall' label='show grid'");
    TwAddButton(m_bar, "wall_add", CB_wall_add, 0, "group='wall' label='>>> add'");
    TwAddVarCB(m_bar, "wall_normal", TW_TYPE_DIR3D, CBSet_wall_normal, CBGet_wall_normal, 0, "group='wall' label='normal'");
    TwAddVarCB(m_bar, "wall_distance", TW_TYPE_DOUBLE, CBSet_wall_distance, CBGet_wall_distance, 0, "group='wall' label='distance' step=0.01");
    TwAddVarCB(m_bar, "wall_color", TW_TYPE_COLOR3F, CBSet_wall_color, CBGet_wall_color, 0, "group='wall' label='color'");
    TwAddVarCB(m_bar, "wall_reflection", TW_TYPE_FLOAT, CBSet_wall_reflection, CBGet_wall_reflection, 0, "group='wall' label='reflection' min=0.0 max=1.0 step=0.01");
    TwAddVarCB(m_bar, "wall_friction", TW_TYPE_FLOAT, CBSet_wall_friction, CBGet_wall_friction, 0, "group='wall' label='friction' min=0.0 max=2.0 step=0.01");
    TwAddVarCB(m_bar, "wall_repulsion", TW_TYPE_FLOAT, CBSet_wall_repulsion, CBGet_wall_repulsion, 0, "group='wall' label='repulsion' min=0.0 max=5.0 step=0.01");
    TwAddVarCB(m_bar, "wall_load", TW_TYPE_STDSTRING, CBSet_wall_load, CBGet_stdstring, &m_wall_fname_load, "group='wall' label='load'");
    TwAddVarCB(m_bar, "wall_save", TW_TYPE_STDSTRING, CBSet_wall_save, CBGet_stdstring, &m_wall_fname_save, "group='wall' label='save'");
    TwAddButton(m_bar, "wall_delete", CB_wall_delete, 0, "group='wall' label='>>> delete_back'");
    TwAddVarRW(m_bar, "offset_x", TW_TYPE_DOUBLE, &core.m_drawer.offset.x_, "group='shadow area' label='x' step=0.1");
    TwAddVarRW(m_bar, "offset_y", TW_TYPE_DOUBLE, &core.m_drawer.offset.y_, "group='shadow area' label='y' step=0.1");
    TwAddVarRW(m_bar, "offset_z", TW_TYPE_DOUBLE, &core.m_drawer.offset.z_, "group='shadow area' label='z' step=0.1");
#endif
}
void Gui::Author::init() {
    m_bar = TwNewBar("Author");
#if SIMPLE_MODE
    TwDefine("Author label='authoring mode' size='250 500' position='0 220'");
#else
#if DEMO_MODE
    TwDefine("Author label='authoring mode' size='250 500' position='0 320' iconified=true");
#else
    TwDefine("Author label='authoring mode' size='250 500' position='0 320'");
#endif
#endif
    
    TwAddVarCB(m_bar, "xml_load", TW_TYPE_STDSTRING, CBSet_xml_load, CBGet_stdstring, &m_xml_load_fname, "label='loadXML'");
    TwAddVarCB(m_bar, "xml_save", TW_TYPE_STDSTRING, CBSet_xml_save, CBGet_stdstring, &m_xml_save_fname, "label='saveXML'");
    TwAddVarCB(m_bar, "tetmesh", TW_TYPE_STDSTRING, CBSet_tetmesh, CBGet_stdstring, &m_tetmesh_fname, "label='tetmesh'");
    TwAddVarCB(m_bar, "trimesh", TW_TYPE_STDSTRING, CBSet_trimesh, CBGet_stdstring, &m_trimesh_fname, "label='trimesh'");
    // constraint
    TwAddVarRW(m_bar, "constraint_mode", TwDefineEnum("ManipulateModeType", 0, 0), &m_manipulateMode, "group='constraint' label='manip mode' enum='0 {translate}, 1 {rotate}, 2 {scale}' key=m");
    TwAddButton(m_bar, "constraint_delete" , CB_constraint_delete, 0, "group='constraint' label='>>> delete'  key=d");
    // example
    TwAddButton(m_bar, "example_add" , CB_example_add, 0, "group='example' label='>>> add'");
    TwAddButton(m_bar, "example_delete", CB_example_delete, 0, "group='example' label='>>> delete_back'");
    TwAddVarRW(m_bar, "example_show", TW_TYPE_BOOLCPP, &m_example_show, "group='example' label='show'");
    TwAddVarRW(m_bar, "example_use", TW_TYPE_BOOLCPP, &m_example_use, "group='example' label='use'");
    TwAddVarCB(m_bar, "example_tetnode_load", TW_TYPE_STDSTRING, CBSet_example_tetnode_load, CBGet_stdstring, &m_example_tetnode_load_fname, "group='example' label='load pose'");
    TwAddVarCB(m_bar, "example_tetnode_save", TW_TYPE_STDSTRING, CBSet_example_tetnode_save, CBGet_stdstring, &m_example_tetnode_save_fname, "group='example' label='save pose'");
    // config
    TwAddVarRW(m_bar, "config_damping", TW_TYPE_FLOAT, &smObject_author.m_config.m_damping, "group='config' label='damping' min=0 max=1 step=0.01");
    TwAddVarRW(m_bar, "config_stiff_alpha", TW_TYPE_FLOAT, &smObject_author.m_config.m_stiffness_alpha, "group='config' label='stiff_alpha' min=0.00 max=2.00 step=0.01");
    TwAddVarRW(m_bar, "config_stiff_iter", TW_TYPE_INT32, &smObject_author.m_config.m_stiffness_iter, "group='config' label='stiff_iter' min=1 max=50");
    TwAddVarRW(m_bar, "config_useLocal", TW_TYPE_BOOLCPP, &smObject_author.m_config.m_useLocalExample, "group='config' label='local example'");
    TwAddVarRW(m_bar, "config_beta", TW_TYPE_FLOAT, &smObject_author.m_config.m_beta, "group='config' label='beta' min=0 max=2.0 step=0.005");
    // initial
    TwAddVarRW(m_bar, "initial_offXmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset.x_, "group='initial' label='offset x' step=0.1");
    TwAddVarRW(m_bar, "initial_offYmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset.y_, "group='initial' label='offset y' step=0.1");
    TwAddVarRW(m_bar, "initial_offZmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset.z_, "group='initial' label='offset z' step=0.1");
//    TwAddVarRW(m_bar, "initial_offXmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_min.x_, "group='initial' label='offset x min' step=0.1");
//    TwAddVarRW(m_bar, "initial_offYmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_min.y_, "group='initial' label='offset y min' step=0.1");
//    TwAddVarRW(m_bar, "initial_offZmin", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_min.z_, "group='initial' label='offset z min' step=0.1");
//    TwAddVarRW(m_bar, "initial_offXmax", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_max.x_, "group='initial' label='offset x max' step=0.1");
//    TwAddVarRW(m_bar, "initial_offYmax", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_max.y_, "group='initial' label='offset y max' step=0.1");
//    TwAddVarRW(m_bar, "initial_offZmax", TW_TYPE_FLOAT, &smObject_author.m_initial.m_offset_max.z_, "group='initial' label='offset z max' step=0.1");
    TwAddVarRW(m_bar, "initial_velX", TW_TYPE_FLOAT, &smObject_author.m_initial.m_velocity.x_, "group='initial' label='velocity x' step=0.1");
    TwAddVarRW(m_bar, "initial_velY", TW_TYPE_FLOAT, &smObject_author.m_initial.m_velocity.y_, "group='initial' label='velocity y' step=0.1");
    TwAddVarRW(m_bar, "initial_velZ", TW_TYPE_FLOAT, &smObject_author.m_initial.m_velocity.z_, "group='initial' label='velocity z' step=0.1");
    // color
    TwAddVarRW(m_bar, "color", TW_TYPE_COLOR3F, smObject_author.m_color.ptr(), "label='color'");
#if !SIMPLE_MODE
    // performance
    TwAddVarRO(m_bar, "performance_timeMin", TW_TYPE_DOUBLE, &smObject_author.m_performance.m_timeMin, "group='performance' label='min'");
    TwAddVarRO(m_bar, "performance_timeMax", TW_TYPE_DOUBLE, &smObject_author.m_performance.m_timeMax, "group='performance' label='max'");
    TwAddVarRO(m_bar, "performance_timeAvg", TW_TYPE_DOUBLE, &smObject_author.m_performance.m_timeAvg, "group='performance' label='avg'");
#endif
}
void Gui::Play::init() {
    m_bar = TwNewBar("Play");
#if DEMO_MODE
    TwDefine("Play label='playing mode' size='250 140' position='250 0' iconified=true");
#else
    TwDefine("Play label='playing mode' size='250 140' position='250 0'");
#endif
    
    TwAddVarCB(m_bar, "scene_load", TW_TYPE_STDSTRING, CBSet_scene_load, CBGet_stdstring, &m_scene_load_fname, "label='load scene'");
    TwAddVarCB(m_bar, "scene_save", TW_TYPE_STDSTRING, CBSet_scene_save, CBGet_stdstring, &m_scene_save_fname, "label='save scene'");
    TwAddVarCB(m_bar, "object_load", TW_TYPE_STDSTRING, CBSet_object_load, CBGet_stdstring, &m_object_fname, "label='load object'");
    TwAddButton(m_bar, "object_delete" , CB_object_delete, 0, "label='>>> delete object'");
    TwAddButton(m_bar, "instantiate" , CB_instantiate, 0, "label='>>> instantiate'  key=SPACE");
#if !SIMPLE_MODE
    TwAddButton(m_bar, "instantiates" , CB_instantiates, 0, "label='>>> instantiates'  key=a");
    TwAddVarRW(m_bar, "lifetime", TW_TYPE_INT32, &m_lifetime, "label='lifetime' min=-1");
#endif
}
void Gui::Param::init() {
#if !SIMPLE_MODE
    m_bar = TwNewBar("Param");
#if DEMO_MODE
    TwDefine("Param label='parameters' size='250 140' position='500 0' iconified=true");
#else
    TwDefine("Param label='parameters' size='250 140' position='500 0'");
#endif
    
    //TwAddVarRW(m_bar, "frameFrequency", TW_TYPE_INT32, &core.m_physParam.m_frameFrequency, "label='frameFrequency' min=1, max=1000");
    TwAddVarRW(m_bar, "dt", TW_TYPE_FLOAT, &core.m_physParam.m_dt, "label='dt' step=0.0001");
    //TwAddVarRW(m_bar, "stepPerFrame", TW_TYPE_INT32, &core.m_physParam.m_stepPerFrame, "label='stepPerFrame' min=1 max=100");
    TwAddVarRW(m_bar, "airResistance", TW_TYPE_FLOAT, &core.m_physParam.m_airResistance, "label='airResistance' min=0.001 max=0.1 step=0.001");
    TwAddVarRW(m_bar, "gravity", TW_TYPE_FLOAT, &core.m_physParam.m_gravity, "label='gravity' step = 0.1");
    TwAddVarRW(m_bar, "springCoef", TW_TYPE_FLOAT, &core.m_physParam.m_springCoef, "label='springCoef' min=10 max=10000 step=10");
    //    TwAddVarCB(m_bar, "load", TW_TYPE_STDSTRING, CBSet_load, CBGet_stdstring, &m_load_fname, "label='load'");
    //    TwAddVarCB(m_bar, "save", TW_TYPE_STDSTRING, CBSet_save, CBGet_stdstring, &m_save_fname, "label='save'");
    //    TwAddVarCB(m_bar, "mass", TW_TYPE_FLOAT, CBSet_mass, CBGet_mass, 0, "label='mass' step=0.05 min=0.05 max=1000.0");
#endif
}
void Gui::Tmp::init() {
#if !SIMPLE_MODE
    m_bar = TwNewBar("Weight");
#if DEMO_MODE
    TwDefine("Weight label='weight' size='250 100' position='750 180' iconified=true");
#else
    TwDefine("Weight label='weight' size='250 100' position='750 180'");
#endif
    
    TwAddVarRW(m_bar, "flag", TW_TYPE_BOOLCPP, &m_flag, "label='weight test mode'");
    TwAddVarRW(m_bar, "real0", TW_TYPE_FLOAT, &m_real0, "label='example weight' step=0.01 min=-10 max=10");
    TwAddButton(m_bar, "test", CB_test, 0, "label='set to example pose'");
    TwAddVarRW(m_bar, "weightClamp", TW_TYPE_BOOLCPP, &m_weightClamp, "label='clamp'");
#endif
}
void Gui::Effect::init() {
#if !SIMPLE_MODE
    m_bar = TwNewBar("Effect");
#if DEMO_MODE
    TwDefine("Effect label='effect' size='250 100' position='750 280' iconified=true");
#else
    TwDefine("Effect label='effect' size='250 100' position='750 280'");
#endif
    
    TwAddVarRW(m_bar, "sm_contrast", TW_TYPE_FLOAT, &m_sm_contrast, "label='sm_contrast' step=0.05 min=0.0 max=5.0");
    TwAddVarRW(m_bar, "sm_lightness", TW_TYPE_FLOAT, &m_sm_lightness, "label='sm_lightness' step=0.05 min=0.0 max=1.0");
    TwAddVarRW(m_bar, "fixed_contrast", TW_TYPE_FLOAT, &m_fixed_contrast, "label='fixed_contrast' step=0.05 min=0.0 max=5.0");
    TwAddVarRW(m_bar, "fixed_lightness", TW_TYPE_FLOAT, &m_fixed_lightness, "label='fixed_lightness' step=0.05 min=0.0 max=1.0");
#endif
}
void Gui::Camera::init() {
    m_bar = TwNewBar("Camera");
#if DEMO_MODE
    TwDefine("Camera label='camera' size='250 180' position='750 0' iconified=true");
#else
    TwDefine("Camera label='camera' size='250 180' position='750 0'");
#endif
    
    TwAddVarRW(m_bar, "camera_x", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.eyePoint.x_, "group='camera' label='x' step=0.05");
    TwAddVarRW(m_bar, "camera_y", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.eyePoint.y_, "group='camera' label='y' step=0.05");
    TwAddVarRW(m_bar, "camera_z", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.eyePoint.z_, "group='camera' label='z' step=0.05");
    TwAddVarRW(m_bar, "focus_x", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.focusPoint.x_, "group='focus' label='x' step=0.05");
    TwAddVarRW(m_bar, "focus_y", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.focusPoint.y_, "group='focus' label='y' step=0.05");
    TwAddVarRW(m_bar, "focus_z", TW_TYPE_DOUBLE, &core.m_drawer.m_trackball.focusPoint.z_, "group='focus' label='z' step=0.05");
}

void Gui::deinit() {
    TwDeleteAllBars();
}
