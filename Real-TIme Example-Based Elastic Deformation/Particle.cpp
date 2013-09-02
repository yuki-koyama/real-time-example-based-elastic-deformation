#include "Core.h"
#include "Particle.h"
#include "Sphere.h"

namespace {
    Core& core = Core::getInstance();
    Sphere& sphere = Sphere::getInstance();
}

Particle::Particle(Y::Vector3f x, Y::Vector3f v, Y::Vector3f f, float m)
: id(-1)
, constraintID(-1)
{
    this->x = x;
    this->x0 = x;
    this->v = v;
    this->f = f;
    this->m = m;
}

void Particle::updateMxx0t() {
    Y::Vector3f t = x * m;
    mxx0t = Y::Matrix3x3f(t.x_ * x0.x_, t.x_ * x0.y_, t.x_ * x0.z_,
                          t.y_ * x0.x_, t.y_ * x0.y_, t.y_ * x0.z_,
                          t.z_ * x0.x_, t.z_ * x0.y_, t.z_ * x0.z_);
}

void Particle::renderParticle(bool shadowMode) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x.x_, x.y_, x.z_);
    sphere.renderObject();
    glPopMatrix();
    if (constraintID != -1) {
        glPushMatrix();
        glTranslatef(constraintPos.x_, constraintPos.y_, constraintPos.z_);
        if (!shadowMode) {
            if (constraintID == core.m_eventHandler.m_currentConstraintID)
                glColor3d(1, 0.8, 0.5);
            else
                glColor3d(0.8, 0.5, 0.2);
        }
        sphere.renderObject();
        glPopMatrix();
        if (!shadowMode) glColor3d(0, 0, 0);
        glBegin(GL_LINES);
        glVertex3fv(x.ptr());
        glVertex3fv(constraintPos.ptr());
        glEnd();
    }
}