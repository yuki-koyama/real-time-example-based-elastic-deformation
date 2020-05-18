#pragma once

#include "Matrix.h"
#include "Util.h"
#include "Vector.h"
#include "gl.h"
#include <iostream>
#include <string>
#include <vector>

namespace Y
{

    class GLUtil
    {
        GLUtil();
        ~GLUtil();

    public:
        static Vector3d project(const Vector3d& worldPos)
        {
            int    vp[4];
            double model[16], proj[16];
            glGetIntegerv(GL_VIEWPORT, vp);
            glGetDoublev(GL_MODELVIEW_MATRIX, model);
            glGetDoublev(GL_PROJECTION_MATRIX, proj);
            Vector3d screenPos;
            gluProject(
                worldPos[0], worldPos[1], worldPos[2], model, proj, vp, &screenPos[0], &screenPos[1], &screenPos[2]);
            // screenPos[1] = vp[3] - 1 - screenPos[1];
            return screenPos;
        }
        static Vector3d unproject(const Vector2d& screenPos)
        {
            int    vp[4];
            double model[16], proj[16];
            glGetIntegerv(GL_VIEWPORT, vp);
            glGetDoublev(GL_MODELVIEW_MATRIX, model);
            glGetDoublev(GL_PROJECTION_MATRIX, proj);
            Vector3d worldPos;
            // gluUnProject( screenPos[0], vp[3] - 1 - screenPos[1], 1.0, model , proj, vp, &worldPos[0], &worldPos[1],
            // &worldPos[2]);
            gluUnProject(screenPos[0], screenPos[1], 1.0, model, proj, vp, &worldPos[0], &worldPos[1], &worldPos[2]);
            return worldPos;
        }
    };
} // namespace Y
