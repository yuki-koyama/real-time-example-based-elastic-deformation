#include "Constraint.h"
#include "Util.h"

using namespace std;

void Constraint::translate(const Y::Vector3f& d)
{
    for (int i = 0; i < m_particles.size(); ++i)
        m_particles[i]->constraintPos += d;
    m_xc += d;
}
void Constraint::rotate(Y::Vector3f& axis, float angle)
{
    Y::Matrix3x3f rot = Y::Util::rotationFromAxisAnglef(axis, angle);
    for (int i = 0; i < m_particles.size(); ++i)
    {
        Particle*   p    = m_particles[i];
        Y::Vector3f dx   = p->constraintPos - m_xc;
        dx               = rot * dx;
        p->constraintPos = m_xc + dx;
    }
}
void Constraint::scale(float s)
{
    for (int i = 0; i < m_particles.size(); ++i)
    {
        Particle*   p  = m_particles[i];
        Y::Vector3f dx = p->constraintPos - m_xc;
        dx *= s;
        p->constraintPos = m_xc + dx;
    }
}
