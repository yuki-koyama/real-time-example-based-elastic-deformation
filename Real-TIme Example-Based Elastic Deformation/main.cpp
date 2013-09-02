#include <AntTweakBar.h>
#include <iostream>
#include <time.h>
#include "Core.h"
#include "Sphere.h"

#include <string>
#include <Y/Clock.h>

#include "params.h"

using namespace std;

extern void glut_display(void);
extern void glut_reshape(int width, int height);
extern void glut_idle(void);
extern void glut_mouse(int button, int state, int x, int y);
extern void glut_motion(int x, int y);
extern void glut_keyboard (unsigned char key, int x, int y);
extern void glut_special (int key, int x, int y);
extern void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString);
extern void initGLEW(void);
extern void initTw(void);

namespace {
    Core& core = Core::getInstance();
    Y::ClockData clkData;
    clock_t clk_start; // for FPS measure
    clock_t clk_perTimeStep;
    bool didSimulation;
}

void glut_display(void) {
    // clock management
    clk_perTimeStep = clock();
    static int cnt = 0;
    cnt = (cnt + 1) % 100;
    if (!cnt) {
        clock_t clk_finish = clk_perTimeStep;
        double fps = CLOCKS_PER_SEC * 100.0 / (clk_finish - clk_start);
        cout << "fps: " << fps << endl;
        clk_start = clk_finish;
    }

    // draw scene
    core.m_drawer.draw();

    // draw UI components
	TwDraw();

	glutSwapBuffers();
    
    didSimulation = false;
}

void glut_reshape(int width, int height) {
    core.m_drawer.reshape(width, height);
    TwWindowSize(width, height);
}

void glut_idle(void) {
    static const clock_t targetInterval = CLOCKS_PER_SEC / TARGET_FPS;
    if (!core.m_scene.m_isPause && !didSimulation) {
        for (int i = 0; i < core.m_physParam.m_stepPerFrame; i ++) {
            core.m_scene.simulateOneStep(core.m_physParam.m_dt);
        }
    }
    didSimulation = true;
    if (clock() - clk_perTimeStep > targetInterval) {
        glutPostRedisplay();
    }
}

void glut_mouse(int button, int state, int x, int y) { core.m_eventHandler.glut_mouse(button, state, x, y); }
void glut_motion(int x, int y) { core.m_eventHandler.glut_motion(x, y); }
void glut_keyboard (unsigned char key, int x, int y) { core.m_eventHandler.glut_keyboard(key, x, y); }
void glut_special (int key, int x, int y) { core.m_eventHandler.glut_special(key, x, y); }

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString) {
    destinationClientString = sourceLibraryString;
}

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(core.m_config.WINDOW_WIDTH, core.m_config.WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Real-Time Example-Based Elastic Deformation");
    
    glutDisplayFunc(glut_display);
    glutReshapeFunc(glut_reshape);
    glutIdleFunc(glut_idle);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutPassiveMotionFunc(glut_motion);
    glutKeyboardFunc(glut_keyboard);
    glutSpecialFunc(glut_special);
    TwGLUTModifiersFunc(glutGetModifiers);  // so important! see TwSimpleGLUT.c
    
    initGLEW();
    initTw();
    
    TwCopyStdStringToClientFunc(CopyStdStringToClient);
    core.m_gui.init();
    
    core.m_drawer.init();
    
    Sphere::getInstance().setBufferObject();
    core.m_scene.m_walls.push_back(Wall());

    clk_start = clock();
    clk_perTimeStep = clk_start;
            
    glutMainLoop();
    
    return 0;
}

// Initialize GLEW
void initGLEW(void) {
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed\n";
        exit(1);
    } else {
        std::cerr << "GLEW initialization done\n";
    }
}

// Initialize AntTweakBar
void initTw(void) {
    if (!TwInit(TW_OPENGL, NULL)) {
        std::cerr << "AntTeakBar initialization failed\n";
        exit(1);
    } else {
        std::cerr << "AntTeakBar initialization done\n";
    }
}
