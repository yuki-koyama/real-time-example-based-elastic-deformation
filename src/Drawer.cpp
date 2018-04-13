#include "Core.h"

#include "GLUtil.h"
#include <KLIB/DrawUtil.h>

#include "params.h"

using namespace std;

namespace {
    Core& core = Core::getInstance();
    static const unsigned int shadowTextureWidth =  SHADOW_TEXTURE_SIZE;
    static const unsigned int shadowTextureHeight = SHADOW_TEXTURE_SIZE;
    
    inline void glColor3(Y::Vector<double, 3> vec) { glColor3dv(vec.val); }
}

void Drawer::reshape(int width, int height) {
    m_width = width;
    m_height = height;
    m_trackball.setSize(width, height);
}

void Drawer::init() {
    showGrid = true;

    m_trackball.eyePoint.set(EYE_X, EYE_Y, EYE_Z);
    m_trackball.focusPoint.set(0.0f, FOCUS_Y, 0.0f);
    m_trackball.upDirection.set(0, 1, 0);
    
    offset.x_ = 0.0; 
    offset.y_ = 0.0; 
    offset.z_ = 0.0;
    
    // shadow mapping //
    glActiveTexture(GL_TEXTURE1);
    static const GLfloat white[] = { 2.0f, 1.0f, 1.0f, 1.0f };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowTextureWidth, shadowTextureHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, white);
    glActiveTexture(GL_TEXTURE0);
    ////////////////////
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glPolygonOffset(1, 1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    
    // initialize lights
	const GLfloat lightColor[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	
    // initialize material
    const GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	const GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    const GLfloat shininess[] = { 30.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void Drawer::scene(bool shadowMapping) {
    // Shape Matching Object
    if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR) {
        core.m_scene.m_smObject_author.updateTriMesh();
        core.m_scene.m_smObject_author.render(shadowMapping);
    } else if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_PLAY) {
        for (list<ShapeMatchingObject>::iterator smObject = core.m_scene.m_smObjects_play.begin(); smObject != core.m_scene.m_smObjects_play.end(); ++ smObject) {
            smObject->updateTriMesh();
            smObject->render(shadowMapping);
        }
    }
    
    // walls
    for (vector<Wall>::iterator wall = core.m_scene.m_walls.begin(); wall != core.m_scene.m_walls.end(); ++ wall) {
        wall->render(shadowMapping);
    }
}

void Drawer::draw() {    
    const Y::Vector3d& e = m_trackball.eyePoint;
    const Y::Vector3d& f = m_trackball.focusPoint;
    const Y::Vector3d& u = m_trackball.upDirection;
    Y::Vector4f lightPos(core.m_scene.m_lightPos, 1.0f);
    
    GLdouble modelview[16];

    // shadow mapping -- create the depth map
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowTextureWidth, shadowTextureHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(90.0, (GLdouble)shadowTextureWidth / (GLdouble)shadowTextureHeight, 1.0, 20.0);
    gluLookAt(lightPos[0], lightPos[1], lightPos[2], f.x_ + offset.x_, f.y_ + offset.y_, f.z_ + offset.z_, u.x_, u.y_, u.z_);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_LIGHTING);
    glCullFace(GL_FRONT);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    scene(true);
    glActiveTexture(GL_TEXTURE1);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowTextureWidth, shadowTextureHeight);
    glActiveTexture(GL_TEXTURE0);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_LIGHTING);
    glCullFace(GL_BACK);
    //////////////////////////////////////
   
    // Set OpenGL viewport and camera
    int& w = m_width;
    int& h = m_height;
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // background gradation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3(core.m_config.BGCOLOR_BOTTOM);    glVertex2d(0, 0);    glVertex2d(w, 0);
    glColor3(core.m_config.BGCOLOR_TOP   );    glVertex2d(w, h);    glVertex2d(0, h);
    glEnd();
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
        
    // camera setting
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fovy, (double)w/h, m_nearClip, m_farClip);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(e.x_, e.y_, e.z_, f.x_, f.y_, f.z_, u.x_, u.y_, u.z_);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos.ptr());
    
    // shadow mapping -- apply shadow mapping texture
    glActiveTexture(GL_TEXTURE1);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslated(0.5, 0.5, 0.5);
    glScaled(0.5, 0.5, 0.5);
    glMultMatrixd(modelview);
    // if not identity, multiply the current inverse matrix of the modelview matrix
    glActiveTexture(GL_TEXTURE0);
    scene(false);
    /////////////////////////////////////////////////

    if (core.m_eventHandler.m_dragMode == EventHandler::DRAGMODE_SELECT) {
        // 2D draw mode
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glColor4d(1, 1, 0, 0.5);
        glTranslated(core.m_eventHandler.m_select_pos[0].x_, core.m_eventHandler.m_select_pos[0].y_, 0);
        double radius = (core.m_eventHandler.m_select_pos[1] - core.m_eventHandler.m_select_pos[0]).length();
        KLIB::DrawUtil::drawDisk(radius);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    if (core.m_gui.m_general.m_showWorldAxis) {
        glDisable(GL_LIGHTING);
        glLineWidth(3.0);
        glBegin(GL_LINES);
        glColor3d(1, 0, 0);     glVertex3d(0, 0, 0);    glVertex3d(1, 0, 0);
        glColor3d(0, 1, 0);     glVertex3d(0, 0, 0);    glVertex3d(0, 1, 0);
        glColor3d(0, 0, 1);     glVertex3d(0, 0, 0);    glVertex3d(0, 0, 1);
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

Drawer::Drawer()
: m_fovy(40)
, m_nearClip(0.1)
, m_farClip(100000)
{
}

Drawer::~Drawer() {
}

