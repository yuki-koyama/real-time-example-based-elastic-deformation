#pragma once
#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glext.h>
#include <iostream>
#include <string>
#include <vector>
//#include <typeinfo.h>
#include "Vector.h"
#include "Util.h"

namespace KLIB {

void glColor3d (const KLIB::Vector3d & c);
void glColor3f (const KLIB::Vector3f & c);
void glColor3b (const KLIB::Vector3c & c);
void glColor3i (const KLIB::Vector3i & c);
void glColor3ub(const KLIB::Vector3uc& c);
void glColor3ui(const KLIB::Vector3ui& c);
void glColor4d (const KLIB::Vector4d & c);
void glColor4f (const KLIB::Vector4f & c);
void glColor4b (const KLIB::Vector4c & c);
void glColor4i (const KLIB::Vector4i & c);
void glColor4ub(const KLIB::Vector4uc& c);
void glColor4ui(const KLIB::Vector4ui& c);

void glVertex2d(const KLIB::Vector2d& v);
void glVertex2f(const KLIB::Vector2f& v);
void glVertex2i(const KLIB::Vector2i& v);
void glVertex3d(const KLIB::Vector3d& v);
void glVertex3f(const KLIB::Vector3f& v);
void glVertex3i(const KLIB::Vector3i& v);
void glVertex4d(const KLIB::Vector4d& v);
void glVertex4f(const KLIB::Vector4f& v);
void glVertex4i(const KLIB::Vector4i& v);
void glTexCoord2d(const KLIB::Vector2d& t);
void glTexCoord2f(const KLIB::Vector2f& t);
void glTexCoord3d(const KLIB::Vector3d& t);
void glTexCoord3f(const KLIB::Vector3f& t);
void glTexCoord4d(const KLIB::Vector4d& t);
void glTexCoord4f(const KLIB::Vector4f& t);
void glNormal3d(const KLIB::Vector3d& n);
void glNormal3f(const KLIB::Vector3f& n);

void glTranslated(const KLIB::Vector3d& t);
void glTranslatef(const KLIB::Vector3f& t);

#define ERROR_CHECK

class GLUtil {
    GLUtil();
    ~GLUtil();
public:
    static bool checkError(const std::string& label) {
#ifndef ERROR_CHECK
        return true;
#else
        return checkError(label.c_str());
#endif
    }
    static bool checkError(const char *label = 0) {
#ifndef ERROR_CHECK
        return true;
#else
        GLenum errCode;
        if ((errCode = glGetError()) == GL_NO_ERROR)
            return true;
        std::cout <<  "OpenGL ERROR: ";
        switch(errCode) {
        case GL_INVALID_ENUM:
            std::cout << "invalid enumeration";
            break;
        case GL_INVALID_VALUE:
            std::cout << "invalid value";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "invalid operation";
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "stack overflow";
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "stack underflow";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "out of memory";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
            std::cout << "invalid framebuffer operation";
            break;
        default:
            std::cout << "unknown code";
        }
        if (label)
            std::cout << " (Label: " << label << ")";
        std::cout << std::endl;
        return false;
#endif
    }
    static bool checkFramebufferStatus(const std::string& label) {
#ifndef ERROR_CHECK
        return true;
#else
        return checkFramebufferStatus(label.c_str());
#endif
    }
    static bool checkFramebufferStatus(const char *label = 0) {
#ifndef ERROR_CHECK
        return true;
#else
        GLenum status;
        if ((status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) ==  GL_FRAMEBUFFER_COMPLETE_EXT)
            return true;
        std::cout << "Framebuffer ERROR: ";
        switch(status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            std::cout << "Framebuffer incomplete, incomplete attachment";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            std::cout << "Unsupported framebuffer format";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            std::cout << "Framebuffer incomplete, missing attachment";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            std::cout << "Framebuffer incomplete, attached images must have same dimensions";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            std::cout << "Framebuffer incomplete, attached images must have same format";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            std::cout << "Framebuffer incomplete, missing draw buffer";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            std::cout << "Framebuffer incomplete, missing read buffer";
            break;
        }
        if (label)
            std::cout << " (Label: " << label << ")";
        std::cout << std::endl;
        return false;
#endif
    }
    struct BoolState {
        GLboolean alpha_test_          ;
        GLboolean auto_normal_         ;
        GLboolean blend_               ;
        GLboolean clip_plane0_         ;
        GLboolean clip_plane1_         ;
        GLboolean clip_plane2_         ;
        GLboolean clip_plane3_         ;
        GLboolean clip_plane4_         ;
        GLboolean clip_plane5_         ;
        GLboolean color_material_      ;
        GLboolean cull_face_           ;
        GLboolean depth_test_          ;
        GLboolean dither_              ;
        GLboolean fog_                 ;
        GLboolean light0_              ;
        GLboolean light1_              ;
        GLboolean light2_              ;
        GLboolean light3_              ;
        GLboolean light4_              ;
        GLboolean light5_              ;
        GLboolean light6_              ;
        GLboolean light7_              ;
        GLboolean lighting_            ;
        GLboolean line_smooth_         ;
        GLboolean line_stipple_        ;
        GLboolean logic_op_            ;
        GLboolean map1_color_4_        ;
        GLboolean map1_index_          ;
        GLboolean map1_normal_         ;
        GLboolean map1_texture_coord_1_;
        GLboolean map1_texture_coord_2_;
        GLboolean map1_texture_coord_3_;
        GLboolean map1_texture_coord_4_;
        GLboolean map1_vertex_3_       ;
        GLboolean map1_vertex_4_       ;
        GLboolean map2_color_4_        ;
        GLboolean map2_index_          ;
        GLboolean map2_normal_         ;
        GLboolean map2_texture_coord_1_;
        GLboolean map2_texture_coord_2_;
        GLboolean map2_texture_coord_3_;
        GLboolean map2_texture_coord_4_;
        GLboolean map2_vertex_3_       ;
        GLboolean map2_vertex_4_       ;
        GLboolean normalize_           ;
        GLboolean point_smooth_        ;
        GLboolean polygon_smooth_      ;
        GLboolean polygon_stipple_     ;
        GLboolean scissor_test_        ;
        GLboolean stencil_test_        ;
        GLboolean texture_1d_          ;
        GLboolean texture_2d_          ;
        GLboolean texture_3d_          ;
        GLboolean texture_gen_q_       ;
        GLboolean texture_gen_r_       ;
        GLboolean texture_gen_s_       ;
        GLboolean texture_gen_t_       ;
        BoolState() :
            alpha_test_          (GL_FALSE),
            auto_normal_         (GL_FALSE),
            blend_               (GL_FALSE),
            clip_plane0_         (GL_FALSE),
            clip_plane1_         (GL_FALSE),
            clip_plane2_         (GL_FALSE),
            clip_plane3_         (GL_FALSE),
            clip_plane4_         (GL_FALSE),
            clip_plane5_         (GL_FALSE),
            color_material_      (GL_FALSE),
            cull_face_           (GL_FALSE),
            depth_test_          (GL_FALSE),
            dither_              (GL_FALSE),
            fog_                 (GL_FALSE),
            light0_              (GL_FALSE),
            light1_              (GL_FALSE),
            light2_              (GL_FALSE),
            light3_              (GL_FALSE),
            light4_              (GL_FALSE),
            light5_              (GL_FALSE),
            light6_              (GL_FALSE),
            light7_              (GL_FALSE),
            lighting_            (GL_FALSE),
            line_smooth_         (GL_FALSE),
            line_stipple_        (GL_FALSE),
            logic_op_            (GL_FALSE),
            map1_color_4_        (GL_FALSE),
            map1_index_          (GL_FALSE),
            map1_normal_         (GL_FALSE),
            map1_texture_coord_1_(GL_FALSE),
            map1_texture_coord_2_(GL_FALSE),
            map1_texture_coord_3_(GL_FALSE),
            map1_texture_coord_4_(GL_FALSE),
            map1_vertex_3_       (GL_FALSE),
            map1_vertex_4_       (GL_FALSE),
            map2_color_4_        (GL_FALSE),
            map2_index_          (GL_FALSE),
            map2_normal_         (GL_FALSE),
            map2_texture_coord_1_(GL_FALSE),
            map2_texture_coord_2_(GL_FALSE),
            map2_texture_coord_3_(GL_FALSE),
            map2_texture_coord_4_(GL_FALSE),
            map2_vertex_3_       (GL_FALSE),
            map2_vertex_4_       (GL_FALSE),
            normalize_           (GL_FALSE),
            point_smooth_        (GL_FALSE),
            polygon_smooth_      (GL_FALSE),
            polygon_stipple_     (GL_FALSE),
            scissor_test_        (GL_FALSE),
            stencil_test_        (GL_FALSE),
            texture_1d_          (GL_FALSE),
            texture_2d_          (GL_FALSE),
            texture_3d_          (GL_FALSE),
            texture_gen_q_       (GL_FALSE),
            texture_gen_r_       (GL_FALSE),
            texture_gen_s_       (GL_FALSE),
            texture_gen_t_       (GL_FALSE) {}
    };
    static BoolState boolState__;
//private:
//    static std::vector<BoolState> stackBoolState__;
//public:
//    static void pushBoolState();
//    static void popBoolState();
    static void printBoolState(std::ostream& ostr = std::cout) {
        BoolState temp = boolState__;
        storeBoolState();
        ostr << "GL_ALPHA_TEST          : " << (boolState__.alpha_test_           ? "true" : "false") << std::endl;
        ostr << "GL_AUTO_NORMAL         : " << (boolState__.auto_normal_          ? "true" : "false") << std::endl;
        ostr << "GL_BLEND               : " << (boolState__.blend_                ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE0         : " << (boolState__.clip_plane0_          ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE1         : " << (boolState__.clip_plane1_          ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE2         : " << (boolState__.clip_plane2_          ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE3         : " << (boolState__.clip_plane3_          ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE4         : " << (boolState__.clip_plane4_          ? "true" : "false") << std::endl;
        ostr << "GL_CLIP_PLANE5         : " << (boolState__.clip_plane5_          ? "true" : "false") << std::endl;
        ostr << "GL_COLOR_MATERIAL      : " << (boolState__.color_material_       ? "true" : "false") << std::endl;
        ostr << "GL_CULL_FACE           : " << (boolState__.cull_face_            ? "true" : "false") << std::endl;
        ostr << "GL_DEPTH_TEST          : " << (boolState__.depth_test_           ? "true" : "false") << std::endl;
        ostr << "GL_DITHER              : " << (boolState__.dither_               ? "true" : "false") << std::endl;
        ostr << "GL_FOG                 : " << (boolState__.fog_                  ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT0              : " << (boolState__.light0_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT1              : " << (boolState__.light1_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT2              : " << (boolState__.light2_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT3              : " << (boolState__.light3_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT4              : " << (boolState__.light4_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT5              : " << (boolState__.light5_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT6              : " << (boolState__.light6_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHT7              : " << (boolState__.light7_               ? "true" : "false") << std::endl;
        ostr << "GL_LIGHTING            : " << (boolState__.lighting_             ? "true" : "false") << std::endl;
        ostr << "GL_LINE_SMOOTH         : " << (boolState__.line_smooth_          ? "true" : "false") << std::endl;
        ostr << "GL_LINE_STIPPLE        : " << (boolState__.line_stipple_         ? "true" : "false") << std::endl;
        ostr << "GL_LOGIC_OP            : " << (boolState__.logic_op_             ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_COLOR_4        : " << (boolState__.map1_color_4_         ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_INDEX          : " << (boolState__.map1_index_           ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_NORMAL         : " << (boolState__.map1_normal_          ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_TEXTURE_COORD_1: " << (boolState__.map1_texture_coord_1_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_TEXTURE_COORD_2: " << (boolState__.map1_texture_coord_2_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_TEXTURE_COORD_3: " << (boolState__.map1_texture_coord_3_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_TEXTURE_COORD_4: " << (boolState__.map1_texture_coord_4_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_VERTEX_3       : " << (boolState__.map1_vertex_3_        ? "true" : "false") << std::endl;
        ostr << "GL_MAP1_VERTEX_4       : " << (boolState__.map1_vertex_4_        ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_COLOR_4        : " << (boolState__.map2_color_4_         ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_INDEX          : " << (boolState__.map2_index_           ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_NORMAL         : " << (boolState__.map2_normal_          ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_TEXTURE_COORD_1: " << (boolState__.map2_texture_coord_1_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_TEXTURE_COORD_2: " << (boolState__.map2_texture_coord_2_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_TEXTURE_COORD_3: " << (boolState__.map2_texture_coord_3_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_TEXTURE_COORD_4: " << (boolState__.map2_texture_coord_4_ ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_VERTEX_3       : " << (boolState__.map2_vertex_3_        ? "true" : "false") << std::endl;
        ostr << "GL_MAP2_VERTEX_4       : " << (boolState__.map2_vertex_4_        ? "true" : "false") << std::endl;
        ostr << "GL_NORMALIZE           : " << (boolState__.normalize_            ? "true" : "false") << std::endl;
        ostr << "GL_POINT_SMOOTH        : " << (boolState__.point_smooth_         ? "true" : "false") << std::endl;
        ostr << "GL_POLYGON_SMOOTH      : " << (boolState__.polygon_smooth_       ? "true" : "false") << std::endl;
        ostr << "GL_POLYGON_STIPPLE     : " << (boolState__.polygon_stipple_      ? "true" : "false") << std::endl;
        ostr << "GL_SCISSOR_TEST        : " << (boolState__.scissor_test_         ? "true" : "false") << std::endl;
        ostr << "GL_STENCIL_TEST        : " << (boolState__.stencil_test_         ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_1D          : " << (boolState__.texture_1d_           ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_2D          : " << (boolState__.texture_2d_           ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_3D          : " << (boolState__.texture_3d_           ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_GEN_Q       : " << (boolState__.texture_gen_q_        ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_GEN_R       : " << (boolState__.texture_gen_r_        ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_GEN_S       : " << (boolState__.texture_gen_s_        ? "true" : "false") << std::endl;
        ostr << "GL_TEXTURE_GEN_T       : " << (boolState__.texture_gen_t_        ? "true" : "false") << std::endl;
        boolState__ = temp;
    }
    static void storeBoolState() {
        //BoolState boolState;
        boolState__.alpha_test_           = glIsEnabled(GL_ALPHA_TEST          );
        boolState__.auto_normal_          = glIsEnabled(GL_AUTO_NORMAL         );
        boolState__.blend_                = glIsEnabled(GL_BLEND               );
        boolState__.clip_plane0_          = glIsEnabled(GL_CLIP_PLANE0         );
        boolState__.clip_plane1_          = glIsEnabled(GL_CLIP_PLANE1         );
        boolState__.clip_plane2_          = glIsEnabled(GL_CLIP_PLANE2         );
        boolState__.clip_plane3_          = glIsEnabled(GL_CLIP_PLANE3         );
        boolState__.clip_plane4_          = glIsEnabled(GL_CLIP_PLANE4         );
        boolState__.clip_plane5_          = glIsEnabled(GL_CLIP_PLANE5         );
        boolState__.color_material_       = glIsEnabled(GL_COLOR_MATERIAL      );
        boolState__.cull_face_            = glIsEnabled(GL_CULL_FACE           );
        boolState__.depth_test_           = glIsEnabled(GL_DEPTH_TEST          );
        boolState__.dither_               = glIsEnabled(GL_DITHER              );
        boolState__.fog_                  = glIsEnabled(GL_FOG                 );
        boolState__.light0_               = glIsEnabled(GL_LIGHT0              );
        boolState__.light1_               = glIsEnabled(GL_LIGHT1              );
        boolState__.light2_               = glIsEnabled(GL_LIGHT2              );
        boolState__.light3_               = glIsEnabled(GL_LIGHT3              );
        boolState__.light4_               = glIsEnabled(GL_LIGHT4              );
        boolState__.light5_               = glIsEnabled(GL_LIGHT5              );
        boolState__.light6_               = glIsEnabled(GL_LIGHT6              );
        boolState__.light7_               = glIsEnabled(GL_LIGHT7              );
        boolState__.lighting_             = glIsEnabled(GL_LIGHTING            );
        boolState__.line_smooth_          = glIsEnabled(GL_LINE_SMOOTH         );
        boolState__.line_stipple_         = glIsEnabled(GL_LINE_STIPPLE        );
        boolState__.logic_op_             = glIsEnabled(GL_LOGIC_OP            );
        boolState__.map1_color_4_         = glIsEnabled(GL_MAP1_COLOR_4        );
        boolState__.map1_index_           = glIsEnabled(GL_MAP1_INDEX          );
        boolState__.map1_normal_          = glIsEnabled(GL_MAP1_NORMAL         );
        boolState__.map1_texture_coord_1_ = glIsEnabled(GL_MAP1_TEXTURE_COORD_1);
        boolState__.map1_texture_coord_2_ = glIsEnabled(GL_MAP1_TEXTURE_COORD_2);
        boolState__.map1_texture_coord_3_ = glIsEnabled(GL_MAP1_TEXTURE_COORD_3);
        boolState__.map1_texture_coord_4_ = glIsEnabled(GL_MAP1_TEXTURE_COORD_4);
        boolState__.map1_vertex_3_        = glIsEnabled(GL_MAP1_VERTEX_3       );
        boolState__.map1_vertex_4_        = glIsEnabled(GL_MAP1_VERTEX_4       );
        boolState__.map2_color_4_         = glIsEnabled(GL_MAP2_COLOR_4        );
        boolState__.map2_index_           = glIsEnabled(GL_MAP2_INDEX          );
        boolState__.map2_normal_          = glIsEnabled(GL_MAP2_NORMAL         );
        boolState__.map2_texture_coord_1_ = glIsEnabled(GL_MAP2_TEXTURE_COORD_1);
        boolState__.map2_texture_coord_2_ = glIsEnabled(GL_MAP2_TEXTURE_COORD_2);
        boolState__.map2_texture_coord_3_ = glIsEnabled(GL_MAP2_TEXTURE_COORD_3);
        boolState__.map2_texture_coord_4_ = glIsEnabled(GL_MAP2_TEXTURE_COORD_4);
        boolState__.map2_vertex_3_        = glIsEnabled(GL_MAP2_VERTEX_3       );
        boolState__.map2_vertex_4_        = glIsEnabled(GL_MAP2_VERTEX_4       );
        boolState__.normalize_            = glIsEnabled(GL_NORMALIZE           );
        boolState__.point_smooth_         = glIsEnabled(GL_POINT_SMOOTH        );
        boolState__.polygon_smooth_       = glIsEnabled(GL_POLYGON_SMOOTH      );
        boolState__.polygon_stipple_      = glIsEnabled(GL_POLYGON_STIPPLE     );
        boolState__.scissor_test_         = glIsEnabled(GL_SCISSOR_TEST        );
        boolState__.stencil_test_         = glIsEnabled(GL_STENCIL_TEST        );
        boolState__.texture_1d_           = glIsEnabled(GL_TEXTURE_1D          );
        boolState__.texture_2d_           = glIsEnabled(GL_TEXTURE_2D          );
        boolState__.texture_3d_           = glIsEnabled(GL_TEXTURE_3D          );
        boolState__.texture_gen_q_        = glIsEnabled(GL_TEXTURE_GEN_Q       );
        boolState__.texture_gen_r_        = glIsEnabled(GL_TEXTURE_GEN_R       );
        boolState__.texture_gen_s_        = glIsEnabled(GL_TEXTURE_GEN_S       );
        boolState__.texture_gen_t_        = glIsEnabled(GL_TEXTURE_GEN_T       );
        //return boolState;
    }
    //static void restoreBoolState(const BoolState& boolState) {
    static void restoreBoolState() {
        if (boolState__.alpha_test_           == GL_TRUE) glEnable(GL_ALPHA_TEST          ); else glDisable(GL_ALPHA_TEST          );
        if (boolState__.auto_normal_          == GL_TRUE) glEnable(GL_AUTO_NORMAL         ); else glDisable(GL_AUTO_NORMAL         );
        if (boolState__.blend_                == GL_TRUE) glEnable(GL_BLEND               ); else glDisable(GL_BLEND               );
        if (boolState__.clip_plane0_          == GL_TRUE) glEnable(GL_CLIP_PLANE0         ); else glDisable(GL_CLIP_PLANE0         );
        if (boolState__.clip_plane1_          == GL_TRUE) glEnable(GL_CLIP_PLANE1         ); else glDisable(GL_CLIP_PLANE1         );
        if (boolState__.clip_plane2_          == GL_TRUE) glEnable(GL_CLIP_PLANE2         ); else glDisable(GL_CLIP_PLANE2         );
        if (boolState__.clip_plane3_          == GL_TRUE) glEnable(GL_CLIP_PLANE3         ); else glDisable(GL_CLIP_PLANE3         );
        if (boolState__.clip_plane4_          == GL_TRUE) glEnable(GL_CLIP_PLANE4         ); else glDisable(GL_CLIP_PLANE4         );
        if (boolState__.clip_plane5_          == GL_TRUE) glEnable(GL_CLIP_PLANE5         ); else glDisable(GL_CLIP_PLANE5         );
        if (boolState__.color_material_       == GL_TRUE) glEnable(GL_COLOR_MATERIAL      ); else glDisable(GL_COLOR_MATERIAL      );
        if (boolState__.cull_face_            == GL_TRUE) glEnable(GL_CULL_FACE           ); else glDisable(GL_CULL_FACE           );
        if (boolState__.depth_test_           == GL_TRUE) glEnable(GL_DEPTH_TEST          ); else glDisable(GL_DEPTH_TEST          );
        if (boolState__.dither_               == GL_TRUE) glEnable(GL_DITHER              ); else glDisable(GL_DITHER              );
        if (boolState__.fog_                  == GL_TRUE) glEnable(GL_FOG                 ); else glDisable(GL_FOG                 );
        if (boolState__.light0_               == GL_TRUE) glEnable(GL_LIGHT0              ); else glDisable(GL_LIGHT0              );
        if (boolState__.light1_               == GL_TRUE) glEnable(GL_LIGHT1              ); else glDisable(GL_LIGHT1              );
        if (boolState__.light2_               == GL_TRUE) glEnable(GL_LIGHT2              ); else glDisable(GL_LIGHT2              );
        if (boolState__.light3_               == GL_TRUE) glEnable(GL_LIGHT3              ); else glDisable(GL_LIGHT3              );
        if (boolState__.light4_               == GL_TRUE) glEnable(GL_LIGHT4              ); else glDisable(GL_LIGHT4              );
        if (boolState__.light5_               == GL_TRUE) glEnable(GL_LIGHT5              ); else glDisable(GL_LIGHT5              );
        if (boolState__.light6_               == GL_TRUE) glEnable(GL_LIGHT6              ); else glDisable(GL_LIGHT6              );
        if (boolState__.light7_               == GL_TRUE) glEnable(GL_LIGHT7              ); else glDisable(GL_LIGHT7              );
        if (boolState__.lighting_             == GL_TRUE) glEnable(GL_LIGHTING            ); else glDisable(GL_LIGHTING            );
        if (boolState__.line_smooth_          == GL_TRUE) glEnable(GL_LINE_SMOOTH         ); else glDisable(GL_LINE_SMOOTH         );
        if (boolState__.line_stipple_         == GL_TRUE) glEnable(GL_LINE_STIPPLE        ); else glDisable(GL_LINE_STIPPLE        );
        if (boolState__.logic_op_             == GL_TRUE) glEnable(GL_LOGIC_OP            ); else glDisable(GL_LOGIC_OP            );
        if (boolState__.map1_color_4_         == GL_TRUE) glEnable(GL_MAP1_COLOR_4        ); else glDisable(GL_MAP1_COLOR_4        );
        if (boolState__.map1_index_           == GL_TRUE) glEnable(GL_MAP1_INDEX          ); else glDisable(GL_MAP1_INDEX          );
        if (boolState__.map1_normal_          == GL_TRUE) glEnable(GL_MAP1_NORMAL         ); else glDisable(GL_MAP1_NORMAL         );
        if (boolState__.map1_texture_coord_1_ == GL_TRUE) glEnable(GL_MAP1_TEXTURE_COORD_1); else glDisable(GL_MAP1_TEXTURE_COORD_1);
        if (boolState__.map1_texture_coord_2_ == GL_TRUE) glEnable(GL_MAP1_TEXTURE_COORD_2); else glDisable(GL_MAP1_TEXTURE_COORD_2);
        if (boolState__.map1_texture_coord_3_ == GL_TRUE) glEnable(GL_MAP1_TEXTURE_COORD_3); else glDisable(GL_MAP1_TEXTURE_COORD_3);
        if (boolState__.map1_texture_coord_4_ == GL_TRUE) glEnable(GL_MAP1_TEXTURE_COORD_4); else glDisable(GL_MAP1_TEXTURE_COORD_4);
        if (boolState__.map1_vertex_3_        == GL_TRUE) glEnable(GL_MAP1_VERTEX_3       ); else glDisable(GL_MAP1_VERTEX_3       );
        if (boolState__.map1_vertex_4_        == GL_TRUE) glEnable(GL_MAP1_VERTEX_4       ); else glDisable(GL_MAP1_VERTEX_4       );
        if (boolState__.map2_color_4_         == GL_TRUE) glEnable(GL_MAP2_COLOR_4        ); else glDisable(GL_MAP2_COLOR_4        );
        if (boolState__.map2_index_           == GL_TRUE) glEnable(GL_MAP2_INDEX          ); else glDisable(GL_MAP2_INDEX          );
        if (boolState__.map2_normal_          == GL_TRUE) glEnable(GL_MAP2_NORMAL         ); else glDisable(GL_MAP2_NORMAL         );
        if (boolState__.map2_texture_coord_1_ == GL_TRUE) glEnable(GL_MAP2_TEXTURE_COORD_1); else glDisable(GL_MAP2_TEXTURE_COORD_1);
        if (boolState__.map2_texture_coord_2_ == GL_TRUE) glEnable(GL_MAP2_TEXTURE_COORD_2); else glDisable(GL_MAP2_TEXTURE_COORD_2);
        if (boolState__.map2_texture_coord_3_ == GL_TRUE) glEnable(GL_MAP2_TEXTURE_COORD_3); else glDisable(GL_MAP2_TEXTURE_COORD_3);
        if (boolState__.map2_texture_coord_4_ == GL_TRUE) glEnable(GL_MAP2_TEXTURE_COORD_4); else glDisable(GL_MAP2_TEXTURE_COORD_4);
        if (boolState__.map2_vertex_3_        == GL_TRUE) glEnable(GL_MAP2_VERTEX_3       ); else glDisable(GL_MAP2_VERTEX_3       );
        if (boolState__.map2_vertex_4_        == GL_TRUE) glEnable(GL_MAP2_VERTEX_4       ); else glDisable(GL_MAP2_VERTEX_4       );
        if (boolState__.normalize_            == GL_TRUE) glEnable(GL_NORMALIZE           ); else glDisable(GL_NORMALIZE           );
        if (boolState__.point_smooth_         == GL_TRUE) glEnable(GL_POINT_SMOOTH        ); else glDisable(GL_POINT_SMOOTH        );
        if (boolState__.polygon_smooth_       == GL_TRUE) glEnable(GL_POLYGON_SMOOTH      ); else glDisable(GL_POLYGON_SMOOTH      );
        if (boolState__.polygon_stipple_      == GL_TRUE) glEnable(GL_POLYGON_STIPPLE     ); else glDisable(GL_POLYGON_STIPPLE     );
        if (boolState__.scissor_test_         == GL_TRUE) glEnable(GL_SCISSOR_TEST        ); else glDisable(GL_SCISSOR_TEST        );
        if (boolState__.stencil_test_         == GL_TRUE) glEnable(GL_STENCIL_TEST        ); else glDisable(GL_STENCIL_TEST        );
        if (boolState__.texture_1d_           == GL_TRUE) glEnable(GL_TEXTURE_1D          ); else glDisable(GL_TEXTURE_1D          );
        if (boolState__.texture_2d_           == GL_TRUE) glEnable(GL_TEXTURE_2D          ); else glDisable(GL_TEXTURE_2D          );
        if (boolState__.texture_3d_           == GL_TRUE) glEnable(GL_TEXTURE_3D          ); else glDisable(GL_TEXTURE_3D          );
        if (boolState__.texture_gen_q_        == GL_TRUE) glEnable(GL_TEXTURE_GEN_Q       ); else glDisable(GL_TEXTURE_GEN_Q       );
        if (boolState__.texture_gen_r_        == GL_TRUE) glEnable(GL_TEXTURE_GEN_R       ); else glDisable(GL_TEXTURE_GEN_R       );
        if (boolState__.texture_gen_s_        == GL_TRUE) glEnable(GL_TEXTURE_GEN_S       ); else glDisable(GL_TEXTURE_GEN_S       );
        if (boolState__.texture_gen_t_        == GL_TRUE) glEnable(GL_TEXTURE_GEN_T       ); else glDisable(GL_TEXTURE_GEN_T       );
    }
    static void glVertex3f_triangleAutoNormal(const Vector3f& p) {
        static int count = 0;
        static Vector3f triangle[3];
        triangle[count] = p;
        if (count == 2) {
            Vector3f n = Util::calcNormal(triangle[0], triangle[1], triangle[2]);
            glNormal3fv(n.ptr());
            glVertex3fv(triangle[0].ptr());
            glVertex3fv(triangle[1].ptr());
            glVertex3fv(triangle[2].ptr());
        }
        count = (count + 1) % 3;
    }
    static void glVertex3f_quadAutoNormal(const Vector3f& p, Vector2f uv = Vector2f()) {
        static int count = 0;
        static Vector3f quad[4];
        static Vector2f quad_uv[4];
        quad[count] = p;
        quad_uv[count] = uv;
        if (count == 3) {
            Vector3f n0 = Util::calcNormal(quad[0], quad[1], quad[2]);
            Vector3f n1 = Util::calcNormal(quad[1], quad[2], quad[3]);
            Vector3f n2 = Util::calcNormal(quad[2], quad[3], quad[0]);
            Vector3f n3 = Util::calcNormal(quad[3], quad[0], quad[1]);
            Vector3f n = n0 + n1 + n2 + n3;
            n.normalize();
            glNormal3fv(n.ptr());
            glTexCoord2f(quad_uv[0]);    glVertex3f(quad[0]);
            glTexCoord2f(quad_uv[1]);    glVertex3f(quad[1]);
            glTexCoord2f(quad_uv[2]);    glVertex3f(quad[2]);
            glTexCoord2f(quad_uv[3]);    glVertex3f(quad[3]);
        }
        count = (count + 1) % 4;
    }
    static Vector3d project(const Vector3d& worldPos) {
        int vp[4] ;
        double model[16], proj[16] ;
        glGetIntegerv(GL_VIEWPORT, vp) ;
        glGetDoublev(GL_MODELVIEW_MATRIX, model) ;
        glGetDoublev(GL_PROJECTION_MATRIX, proj) ;
        Vector3d screenPos;
        gluProject(worldPos[0], worldPos[1], worldPos[2], model, proj, vp, &screenPos[0], &screenPos[1], &screenPos[2]);
        //screenPos[1] = vp[3] - 1 - screenPos[1];
        return screenPos;
    }
    static Vector3d unproject(const Vector2d& screenPos) {
        int vp[4] ;
        double model[16], proj[16] ;
        glGetIntegerv(GL_VIEWPORT, vp) ;
        glGetDoublev(GL_MODELVIEW_MATRIX,  model) ;
        glGetDoublev(GL_PROJECTION_MATRIX, proj) ;
        Vector3d worldPos;
        //gluUnProject( screenPos[0], vp[3] - 1 - screenPos[1], 1.0, model , proj, vp, &worldPos[0], &worldPos[1], &worldPos[2]);
        gluUnProject( screenPos[0], screenPos[1], 1.0, model , proj, vp, &worldPos[0], &worldPos[1], &worldPos[2]);
        return worldPos;
    }
};

class TextureObject;
class RenderbufferObject;
class FramebufferObject;

class TextureObject {
    friend class RenderbufferObject;
    friend class FramebufferObject;
private:
    GLuint id_;
    GLenum target_;
    GLint internalformat_;
    GLsizei width_, height_, depth_;
    GLenum format_;
public:
    TextureObject()
        : id_            (0)
        , target_        (0)
        , internalformat_(0)
        , width_         (0)
        , height_        (0)
        , depth_         (0)
        , format_        (0)
    {}
    ~TextureObject() { /*deinit();*/ }
    void init(
        GLenum target      = GL_TEXTURE_2D,
        GLenum paramWrap   = GL_REPEAT,
        GLenum paramFilter = GL_LINEAR,
        GLint envMode      = GL_REPLACE)
    {
        deinit();
        glGenTextures(1, &id_);
        target_ = target;
        setParamWrap(paramWrap);
        setParamFilter(paramFilter);
        setEnvMode(envMode);
        GLUtil::checkError("TextureObject::init");
    }
    void deinit() {
        if (id_)
            glDeleteTextures(1, &id_);
        id_ = target_ = internalformat_ = width_ = height_ = format_ = 0;
        GLUtil::checkError("TextureObject::deinit");
    }
    GLuint id() const { return id_; }
    void allocate(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type = GL_UNSIGNED_BYTE, const GLvoid* pixels = 0) {
        bind();
        internalformat_ = internalformat;
        width_          = width;
        height_         = height;
        depth_          = 0;
        format_         = format;
        glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, pixels);
        unbind();
        GLUtil::checkError("TextureObject::allocate");
    }
    void allocate3D(GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type = GL_UNSIGNED_BYTE, const GLvoid* pixels = 0) {
        bind();
        internalformat_ = internalformat;
        width_          = width;
        height_         = height;
        depth_          = depth;
        format_         = format;
        glTexImage3D(target_, 0, internalformat, width, height, depth, 0, format, type, pixels);
        unbind();
        GLUtil::checkError("TextureObject::allocate3D");
    }
    //void copy_cpu2gpu(FramebufferObject& framebuffer, const GLvoid* pixels);
    //void copy_gpu2cpu(FramebufferObject& framebuffer, GLvoid* pixels);
    void copy_cpu2gpu(GLenum type, const GLvoid* pixels);
    void copy_gpu2cpu(GLenum type, GLvoid* pixels);
    void copy_gpu2gpu(TextureObject& dst);
    void bind  () const {
        glBindTexture(target_, id_);
        GLUtil::checkError("TextureObject::bind");
    }
    void unbind() const {
        glBindTexture(target_, 0);
        GLUtil::checkError("TextureObject::unbind");
    }
    void enable () const { glEnable (target_); }
    void disable() const { glDisable(target_); }
    
    GLsizei width () const { return width_;  }
    GLsizei height() const { return height_; }
    void setParamWrap (GLenum param) const {
        bind();
        glTexParameteri(target_, GL_TEXTURE_WRAP_S, param);
        glTexParameteri(target_, GL_TEXTURE_WRAP_T, param);
        glTexParameteri(target_, GL_TEXTURE_WRAP_R, param);
        unbind();
        GLUtil::checkError("TextureObject::setParamWrap");
    }
    void setParamFilter(GLenum param) const {
        bind();
        glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, param);
        glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, param);
        unbind();
        GLUtil::checkError("TextureObject::setParamFilter");
    }
    void setParamBorderColor(GLfloat* borderColor) const {
        bind();
        glTexParameterfv(target_, GL_TEXTURE_BORDER_COLOR, borderColor);
        unbind();
        GLUtil::checkError("TextureObject::setParamBorderColor");
    }
    void setEnvMode(GLint param) const {
        bind();
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
        unbind();
        GLUtil::checkError("TextureObject::setEnvMode");
    }
    bool is_compatible(const TextureObject& rhs) const {
        return
            target_         == rhs.target_         &&
            internalformat_ == rhs.internalformat_ &&
            width_          == rhs.width_          &&
            height_         == rhs.height_         &&
            format_         == rhs.format_;
    }
};

class RenderbufferObject {
    friend class TextureObject;
    friend class FramebufferObject;
private:
    GLuint id_;
    GLsizei width_;
    GLsizei height_;
public:
    RenderbufferObject()
        : id_(0)
        , width_ (0)
        , height_(0)
    {}
    ~RenderbufferObject() { /*deinit();*/ }
    
    void init() {
        deinit();
        glGenRenderbuffersEXT(1, &id_);
        GLUtil::checkError("RenderbufferObject::init");
    }
    void deinit() {
        if (id_)
            glDeleteRenderbuffersEXT(1, &id_);
        id_ = 0;
        GLUtil::checkError("RenderbufferObject::deinit");
    }
    void bind() {
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id_);
        GLUtil::checkError("RenderbufferObject::bind");
    }
    static void unbind() {
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
        GLUtil::checkError("RenderbufferObject::unbind");
    }
    void allocate(GLenum internalformat, GLsizei width, GLsizei height) {
        width_  = width;
        height_ = height;
        bind();
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalformat, width, height);
        unbind();
        GLUtil::checkError("RenderbufferObject::allocate");
    }
    GLsizei width () const { return width_; }
    GLsizei height() const { return height_; }
};

class FramebufferObject {
    friend class TextureObject;
    friend class RenderbufferObject;
private:
    GLuint id_;
public:
    FramebufferObject()
        : id_(0)
    {}
    ~FramebufferObject() { /*deinit();*/ }
    
    void init() {
        deinit();
        glGenFramebuffersEXT(1, &id_);
        GLUtil::checkError("FramebufferObject::init");
    }
    void deinit() {
        if (id_)
            glDeleteFramebuffersEXT(1, &id_);
        id_ = 0;
        GLUtil::checkError("FramebufferObject::deinit");
    }
    bool is_bound() const {
        GLint id;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &id);
        GLUtil::checkError("FramebufferObject::is_bound");
        return id_ == id;
    }
    void bind() const {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_);
        GLUtil::checkError("FramebufferObject::bind");
    }
    static void unbind() {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        GLUtil::checkError("FramebufferObject::unbind");
    }
    static void attachRenderbuffer(GLenum attachment, const RenderbufferObject& renderbuffer) { glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, renderbuffer.id_); }
    static void detachRenderbuffer(GLenum attachment)                                         { glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, 0); }
    static void attachTexture(GLenum attachment, const TextureObject& texture) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, texture.target_, texture.id_, 0); }
    static void detachTexture(GLenum attachment, const TextureObject& texture) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, texture.target_, 0          , 0); }
    //static void detachTexture(GLenum attachment) { glFramebufferTexture2DEXT   (GL_FRAMEBUFFER_EXT, attachment, 0, 0, 0); }      // this should be OK according to API (weird...)
};


template <typename Scalar, int N>
struct TextureBuffer;

template <int N>
struct TextureBuffer<double, N> {
    TextureObject texture_;
    std::vector<KLIB::Vector<double, N> > cpuData_;
    void dump() {
        size_t sz = texture_.width() * texture_.height();
        if (cpuData_.size() != sz)
            cpuData_.resize(sz);
        texture_.copy_gpu2cpu(GL_DOUBLE, &cpuData_[0]);
    }
    void copy_gpu2cpu() { dump(); }
    void copy_cpu2gpu() {
        assert(cpuData_.size() == texture_.width() * texture_.height());
        texture_.copy_cpu2gpu(GL_DOUBLE, &cpuData_[0]);
    }
/*
    KLIB::Vector<double, N> getValue(const Vector2d& uv, bool wrap = false, bool lerp = false) const {
        double u = uv.x_;
        double v = uv.y_;
        if (!wrap && (u < 0 || v < 0 || 1 < u || 1 < v))
            return KLIB::Vector<Scalar, N>();
        if (u < 0 || 1 < u)
            u -= floor(u);
        if (v < 0 || 1 < v)
            v -= floor(v);
        int W = texture_.width () - 1;
        int H = texture_.height() - 1;
        int W1 = W + 1;
        int iu0 = static_cast<int>(u * W);
        int iv0 = static_cast<int>(v * H);
        int iu1 = iu0 == W ? iu0 : (iu0 + 1);
        int iv1 = iv0 == H ? iv0 : (iv0 + 1);
        double frac_u = u * W - iu0;
        double frac_v = v * H - iv0;
        if (lerp) {
            int index00 = iv0 * W1 + iu0;
            int index10 = iv0 * W1 + iu1;
            int index01 = iv1 * W1 + iu0;
            int index11 = iv1 * W1 + iu1;
            double wu0 = 1 - frac_u, wu1 = frac_u;
            double wv0 = 1 - frac_v, wv1 = frac_v;
            KLIB::Vector<double, N> result;
            result += wu0 * wv0 * cpuData_[index00];
            result += wu1 * wv0 * cpuData_[index10];
            result += wu0 * wv1 * cpuData_[index01];
            result += wu1 * wv1 * cpuData_[index11];
            return result;
        } else {
            int iu = frac_u < 0.5 ? iu0 : iu1;
            int iv = frac_v < 0.5 ? iv0 : iv1;
            int index = iv * W1 + iu;
            return cpuData_[index];
        }
    }
 */
    KLIB::Vector<double, N> getValue(const Vector2f& uv, bool wrap = false, bool lerp = false) const {
        return getValue(uv.convert<double>(), wrap, lerp);
    }
};
template <int N>
struct TextureBuffer<float, N> {
    TextureObject texture_;
    std::vector<KLIB::Vector<float, N> > cpuData_;
    void dump() {
        size_t sz = texture_.width() * texture_.height();
        if (cpuData_.size() != sz)
            cpuData_.resize(sz);
        texture_.copy_gpu2cpu(GL_FLOAT, &cpuData_[0]);
    }
    void copy_gpu2cpu() { dump(); }
    void copy_cpu2gpu() {
        assert(cpuData_.size() == texture_.width() * texture_.height());
        texture_.copy_cpu2gpu(GL_FLOAT, &cpuData_[0]);
    }
    KLIB::Vector<float, N> getValue(const Vector2f& uv, bool wrap = false, bool lerp = false) const {
        float u = uv.x_;
        float v = uv.y_;
        if (!wrap && (u < 0.0f || v < 0.0f || 1.0f < u || 1.0f < v))
            return KLIB::Vector<float, N>();
        if (u < 0.0f || 1.0f < u)
            u -= floor(u);
        if (v < 0.0f || 1.0f < v)
            v -= floor(v);
        int W = texture_.width () - 1;
        int H = texture_.height() - 1;
        int W1 = W + 1;
        int iu0 = static_cast<int>(u * W);
        int iv0 = static_cast<int>(v * H);
        int iu1 = iu0 == W ? iu0 : (iu0 + 1);
        int iv1 = iv0 == H ? iv0 : (iv0 + 1);
        float frac_u = u * W - iu0;
        float frac_v = v * H - iv0;
        if (lerp) {
            int index00 = iv0 * W1 + iu0;
            int index10 = iv0 * W1 + iu1;
            int index01 = iv1 * W1 + iu0;
            int index11 = iv1 * W1 + iu1;
            float wu0 = 1 - frac_u, wu1 = frac_u;
            float wv0 = 1 - frac_v, wv1 = frac_v;
            KLIB::Vector<float, N> result;
            result += wu0 * wv0 * cpuData_[index00];
            result += wu1 * wv0 * cpuData_[index10];
            result += wu0 * wv1 * cpuData_[index01];
            result += wu1 * wv1 * cpuData_[index11];
            return result;
        } else {
            int iu = frac_u < 0.5f ? iu0 : iu1;
            int iv = frac_v < 0.5f ? iv0 : iv1;
            int index = iv * W1 + iu;
            return cpuData_[index];
        }
    }
    KLIB::Vector<float, N> getValue(const Vector2d& uv, bool wrap = false, bool lerp = false) const {
        return getValue(uv.convert<float>(), wrap, lerp);
    }
};
template <int N>
struct TextureBuffer<unsigned char, N> {
    TextureObject texture_;
    std::vector<KLIB::Vector<unsigned char, N> > dump_;
    void dump() {
        size_t sz = texture_.width() * texture_.height();
        if (dump_.size() != sz)
            dump_.resize(sz);
        texture_.copy_gpu2cpu(GL_UNSIGNED_BYTE, &dump_[0]);
    }
    /*
    KLIB::Vector<unsigned char, N> getValue(const Vector2d& uv, bool wrap = false, bool lerp = false) const {
        double u = uv.x_;
        double v = uv.y_;
        if (!wrap && (u < 0 || v < 0 || 1 < u || 1 < v))
            return KLIB::Vector<Scalar, N>();
        if (u < 0 || 1 < u)
            u -= floor(u);
        if (v < 0 || 1 < v)
            v -= floor(v);
        int W = texture_.width () - 1;
        int H = texture_.height() - 1;
        int W1 = W + 1;
        int iu0 = static_cast<int>(u * W);
        int iv0 = static_cast<int>(v * H);
        int iu1 = iu0 == W ? iu0 : (iu0 + 1);
        int iv1 = iv0 == H ? iv0 : (iv0 + 1);
        double frac_u = u * W - iu0;
        double frac_v = v * H - iv0;
        if (lerp) {
            int index00 = iv0 * W1 + iu0;
            int index10 = iv0 * W1 + iu1;
            int index01 = iv1 * W1 + iu0;
            int index11 = iv1 * W1 + iu1;
            double wu0 = 1 - frac_u, wu1 = frac_u;
            double wv0 = 1 - frac_v, wv1 = frac_v;
            KLIB::Vector<double, N> result;
            result += wu0 * wv0 * dump_[index00].convert<double>();
            result += wu1 * wv0 * dump_[index10].convert<double>();
            result += wu0 * wv1 * dump_[index01].convert<double>();
            result += wu1 * wv1 * dump_[index11].convert<double>();
            return result.convert<unsigned char>();
        } else {
            int iu = frac_u < 0.5 ? iu0 : iu1;
            int iv = frac_v < 0.5 ? iv0 : iv1;
            int index = iv * W1 + iu;
            return dump_[index];
        }
    }
     */
    KLIB::Vector<unsigned char, N> getValue(const Vector2f& uv, bool wrap = false, bool lerp = false) const {
        return getValue(uv.convert<double>(), wrap, lerp);
    }
};
typedef TextureBuffer<float        , 1> TextureBuffer1f;
typedef TextureBuffer<double       , 1> TextureBuffer1d;
typedef TextureBuffer<unsigned char, 1> TextureBuffer1uc;
typedef TextureBuffer<float        , 3> TextureBuffer3f;
typedef TextureBuffer<double       , 3> TextureBuffer3d;
typedef TextureBuffer<unsigned char, 3> TextureBuffer3uc;
typedef TextureBuffer<float        , 4> TextureBuffer4f;
typedef TextureBuffer<double       , 4> TextureBuffer4d;
typedef TextureBuffer<unsigned char, 4> TextureBuffer4uc;

// display list
struct DisplayList {
private:
    GLuint id_;
    bool needUpdate_;
public:
    DisplayList()
        : id_(0)
        , needUpdate_(true)
    {}
    ~DisplayList() { /*deinit();*/ }
    void init() {
        deinit();
        id_ = glGenLists(1);
    }
    void deinit() {
        if (id_) {
            glDeleteLists(id_, 1);
            id_ = 0;
        }
    }
    bool needUpdate() const { return needUpdate_; }
    void requestUpdate() { needUpdate_ = true; }
    void newList(bool isCompileOnly = false) {
        if (!id_)
            init();
        glNewList(id_, isCompileOnly ? GL_COMPILE : GL_COMPILE_AND_EXECUTE);
    }
    void endList() {
        glEndList();
        needUpdate_ = false;
    }
    void callList() const {
        assert(id_);
        glCallList(id_);
    }
};

}
