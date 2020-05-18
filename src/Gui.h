#pragma once

#include <AntTweakBar.h>
#include <string>

struct Gui
{
    //-----------------+
    // general setting |
    //-----------------+
    struct General
    {
        TwBar* m_bar;
        enum DemoMode
        {
            DEMOMODE_AUTHOR = 0,
            DEMOMODE_PLAY
        } m_demoMode;
        enum RenderMode
        {
            RENDERMODE_BOTH = 0,
            RENDERMODE_PARTICLE,
            RENDERMODE_TRIMESH,
        } m_renderMode;
        std::string m_wall_fname_load;
        std::string m_wall_fname_save;
        bool        m_showWorldAxis;
        void        init();
        General() : m_bar(0), m_demoMode(DEMOMODE_AUTHOR), m_renderMode(RENDERMODE_TRIMESH), m_showWorldAxis(false) {}
        static void TW_CALL CBSet_render_lightPos(const void* value, void* clientData);
        static void TW_CALL CBGet_render_lightPos(void* value, void* clientData);
        static void TW_CALL CB_simulation_step(void*);
        static void TW_CALL CB_simulation_reset(void*);
        static void TW_CALL CB_wall_add(void*);
        static void TW_CALL CBSet_wall_normal(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_normal(void* value, void* clientData);
        static void TW_CALL CBSet_wall_distance(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_distance(void* value, void* clientData);
        static void TW_CALL CBSet_wall_color(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_color(void* value, void* clientData);
        static void TW_CALL CBSet_wall_reflection(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_reflection(void* value, void* clientData);
        static void TW_CALL CBSet_wall_friction(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_friction(void* value, void* clientData);
        static void TW_CALL CBSet_wall_repulsion(const void* value, void* clientData);
        static void TW_CALL CBGet_wall_repulsion(void* value, void* clientData);
        static void TW_CALL CBSet_wall_load(const void* value, void* clientData);
        static void TW_CALL CBSet_wall_save(const void* value, void* clientData);
        static void TW_CALL CB_wall_delete(void*);
    } m_general;

    //-----------------+
    // for author mode |
    //-----------------+
    struct Author
    {
        TwBar* m_bar;
        enum ManipulateMode
        {
            MANIPULATEMODE_TRANSLATE = 0,
            MANIPULATEMODE_ROTATE,
            MANIPULATEMODE_SCALE,
        } m_manipulateMode;
        std::string m_xml_load_fname;
        std::string m_xml_save_fname;
        std::string m_tetmesh_fname;
        std::string m_trimesh_fname;
        bool        m_example_show;
        bool        m_example_use;
        std::string m_example_tetnode_load_fname;
        std::string m_example_tetnode_save_fname;
        void        init();
        Author() : m_bar(0), m_manipulateMode(MANIPULATEMODE_TRANSLATE) {}
        static void TW_CALL CBSet_xml_load(const void* value, void* clientData);
        static void TW_CALL CBSet_xml_save(const void* value, void* clientData);
        static void TW_CALL CBSet_tetmesh(const void* value, void* clientData);
        static void TW_CALL CBSet_trimesh(const void* value, void* clientData);
        static void TW_CALL CB_constraint_delete(void*);
        static void TW_CALL CB_example_add(void*);
        static void TW_CALL CB_example_delete(void*);
        static void TW_CALL CBSet_example_tetnode_load(const void* value, void* clientData);
        static void TW_CALL CBSet_example_tetnode_save(const void* value, void* clientData);
    } m_author;

    //---------------+
    // for play mode |
    //---------------+
    struct Play
    {
        TwBar*      m_bar;
        std::string m_scene_load_fname;
        std::string m_scene_save_fname;
        std::string m_object_fname;
        int         m_lifetime;
        void        init();
        Play() : m_bar(0), m_lifetime(-1) {}
        static void TW_CALL CBSet_scene_load(const void* value, void* clientData);
        static void TW_CALL CBSet_scene_save(const void* value, void* clientData);
        static void TW_CALL CBSet_object_load(const void* value, void* clientData);
        static void TW_CALL CB_object_delete(void*);
        static void TW_CALL CB_instantiate(void*);
        static void TW_CALL CB_instantiates(void*);
    } m_play;

    // param
    struct Param
    {
        TwBar*              m_bar;
        std::string         m_load_fname;
        std::string         m_save_fname;
        void                init();
        static void TW_CALL CBSet_load(const void* value, void* clientData);
        static void TW_CALL CBSet_save(const void* value, void* clientData);
        static void TW_CALL CBSet_mass(const void* value, void* clientData);
        static void TW_CALL CBGet_mass(void* value, void* clientData);
        Param() : m_bar(0) {}
    } m_param;

    // tmp
    struct Tmp
    {
        TwBar* m_bar;
        bool   m_flag;
        float  m_real0;
        bool   m_weightClamp;
        void   init();
        Tmp() : m_flag(false), m_real0(0), m_weightClamp(true) {}
        static void TW_CALL CB_test(void*);
    } m_tmp;

    struct Effect
    {
        TwBar* m_bar;
        float  m_sm_contrast;
        float  m_sm_lightness;
        float  m_fixed_contrast;
        float  m_fixed_lightness;
        void   init();
        Effect() : m_sm_contrast(1.2f), m_sm_lightness(0.40f), m_fixed_contrast(1.2f), m_fixed_lightness(0.55f) {}
    } m_effect;

    struct Camera
    {
        TwBar* m_bar;
        void   init();
        Camera() {}
    } m_camera;

    void init();
    void deinit();
};
