#pragma once
#include "GLUtil.h"

namespace KLIB {

struct DrawUtil {
    DrawUtil();
    ~DrawUtil();
    static double PI() {
        static const double pi = 6 * asin(0.5);
        return pi;
    }
    static void drawDisk(double radius, int numDiv = 24) {
        glBegin(GL_TRIANGLE_FAN);
        double t = 0;
        double dt = 2 * PI() / numDiv;
        for (int i = 0; i <= numDiv; ++i, t += dt)
            ::glVertex2d(radius * cos(t), radius * sin(t));
        glEnd();
    }
    static void drawCircle(double radius, int numDiv = 24) {
        glBegin(GL_LINE_LOOP);
        double t = 0;
        double dt = 2 * PI() / numDiv;
        for (int i = 0; i <= numDiv; ++i, t += dt)
            ::glVertex2d(radius * cos(t), radius * sin(t));
        glEnd();
    }
};

}
