#include "Core.h"
#include <Y/GLUtil.h>
using namespace std;

namespace {
    Core& core = Core::getInstance();
    ShapeMatchingObject& smObject = core.m_scene.m_smObject_author;
}

EventHandler::EventHandler()
: m_dragMode(DRAGMODE_NONE)
, m_currentConstraintID(-1)
{}
void EventHandler::glut_mouse(int button, int state, int x, int y) {
    if (TwEventMouseButtonGLUT(button, state, x, y))
        return;
    int y2 = core.m_drawer.m_height - y;
    bool flagShift = (glutGetModifiers() & GLUT_ACTIVE_SHIFT) != 0;
    bool flagCtrl  = (glutGetModifiers() & GLUT_ACTIVE_CTRL ) != 0;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (flagShift) {
                m_dragMode = DRAGMODE_SELECT;
                m_select_pos[0] = m_select_pos[1] = Y::Vector2d(x, y2);
            } else if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR) {
                m_currentConstraintID = -1;
                m_manipulate_prevPos.set(x, y2);
                // find nearest constraint
                for (int i = 0 ; i < smObject.particles.size(); ++i) {
                    Particle& p = smObject.particles[i];
                    if (p.constraintID == -1)
                        continue;
                    Y::Vector3d x3 = static_cast<Y::Vector3f>(p.constraintPos).convert<double>();
                    Y::Vector2d x2 = Y::GLUtil::project(x3).xy();
                    if (10 < (x2 - m_manipulate_prevPos).length())
                        continue;
                    m_currentConstraintID = p.constraintID;
                    break;
                }
                if (m_currentConstraintID == -1)
                    return;
                m_dragMode = DRAGMODE_MANIPULATE;
            } else { // DEMOMODE_PLAY
                Y::Vector3d line_v0 = core.m_drawer.m_trackball.eyePoint;
                Y::Vector3d line_v1 = Y::GLUtil::unproject(Y::Vector2d(x, y2));
                Y::Vector3d line_dir = line_v1 - line_v0;
                Particle* picked_particle = 0;
                double picked_dist = DBL_MAX;
                
                // find adequate particle
                
                ////////////////////////////////////////
                // if you want to use 'temp' user interaction in author mode
                ////////////////////////////////////////
                if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR) {
                        
                        // make updated trimesh
                        TriMesh trimesh = smObject.trimesh;
                        TetMesh& tetmesh = smObject.tetmesh;
                        for (TriMesh::VIter v = trimesh.vertices_begin(); v != trimesh.vertices_end(); ++ v) {
                            TriMesh::VertexData& vdata = trimesh.data(v);
                            Y::Vector3f vpos;
                            TetMesh::Tetra& tetra = tetmesh.tetras_[vdata.tetra_id_];
                            for (int i = 0; i < 4; ++ i) {
                                int particle_id = tetmesh.vertices_[tetra.vid_[i]].particle_id_;
                                vpos += smObject.particles[particle_id].x * vdata.baryCoord_[i];
                            }
                            trimesh.set_point(v, Y::Vector3f(vpos).convert<double>());
                        }
                        trimesh.update_normals();
                        
                        // check each surface triangle
                        for (TriMesh::FIter f = trimesh.faces_begin(); f != trimesh.faces_end(); ++f) {
                            double dp = trimesh.normal(f) | line_dir;
                            if (dp >= 0)
                                continue;
                            
                            Y::Vector3d face_w[3];
                            TriMesh::FVIter v = trimesh.fv_iter(f);
                            for (int i = 0; i < 3; ++ i, ++ v) {
                                face_w[i] = trimesh.point(v);
                            }
                            Y::Vector2d baryCoordV;
                            Y::Vector3d baryCoordW;
                            if (!Y::Util::calcIntersectionLineTriangle(line_v0, line_v1, face_w[0], face_w[1], face_w[2], baryCoordV, baryCoordW))
                                continue;
                            if (baryCoordW[0] < 0 || baryCoordW[1] < 0 || baryCoordW[2] < 0 || 1 < baryCoordW[0] || 1 < baryCoordW[1] || 1 < baryCoordW[2])
                                continue;
                            double dist = baryCoordV[1];
                            if (dist < picked_dist) {
                                picked_dist = dist;
                                Y::Vector3d hitPosition = baryCoordV[0] * line_v0 + baryCoordV[1] * line_v1;
                                float dist2_min = FLT_MAX;
                                for (TriMesh::FVIter v = trimesh.fv_iter(f); v; ++ v) {
                                    TetMesh::Tetra& tet = tetmesh.tetras_[trimesh.data(v).tetra_id_];
                                    for (int i = 0; i < 4; ++ i) {
                                        Particle* particle = &smObject.particles[tet.vid_[i]];
                                        float dist2 = (particle->x - hitPosition.convert<float>()).length();
                                        if (dist2 < dist2_min) {
                                            dist2_min = dist2;
                                            picked_particle = particle;
                                        }
                                    }
                                }
                                
                                // also pick the neighborhoods of the particle.
                                core.m_constraintTemp.m_neighborParticles.clear();
                                for (std::vector<int>::iterator it = smObject.clusters[picked_particle->main_cluster_id].particle_ids.begin(); it != smObject.clusters[picked_particle->main_cluster_id].particle_ids.end(); ++ it) {
                                    core.m_constraintTemp.m_neighborParticles.push_back(&(smObject.particles[*it]));
                                }
                            }
                        }
                } else {
                    for (list<ShapeMatchingObject>::iterator smObject = core.m_scene.m_smObjects_play.begin();
                         smObject != core.m_scene.m_smObjects_play.end(); ++ smObject) {
                        
                        // make updated trimesh
                        TriMesh trimesh = smObject->trimesh;
                        TetMesh& tetmesh = smObject->tetmesh;
                        for (TriMesh::VIter v = trimesh.vertices_begin(); v != trimesh.vertices_end(); ++ v) {
                            TriMesh::VertexData& vdata = trimesh.data(v);
                            Y::Vector3f vpos;
                            TetMesh::Tetra& tetra = tetmesh.tetras_[vdata.tetra_id_];
                            for (int i = 0; i < 4; ++ i) {
                                int particle_id = tetmesh.vertices_[tetra.vid_[i]].particle_id_;
                                vpos += smObject->particles[particle_id].x * vdata.baryCoord_[i];
                            }
                            trimesh.set_point(v, Y::Vector3f(vpos).convert<double>());
                        }
                        trimesh.update_normals();
                        
                        // check each surface triangle
                        for (TriMesh::FIter f = trimesh.faces_begin(); f != trimesh.faces_end(); ++f) {
                            double dp = trimesh.normal(f) | line_dir;
                            if (dp >= 0)
                                continue;
                            
                            Y::Vector3d face_w[3];
                            TriMesh::FVIter v = trimesh.fv_iter(f);
                            for (int i = 0; i < 3; ++ i, ++ v) {
                                face_w[i] = trimesh.point(v);
                            }
                            Y::Vector2d baryCoordV;
                            Y::Vector3d baryCoordW;
                            if (!Y::Util::calcIntersectionLineTriangle(line_v0, line_v1, face_w[0], face_w[1], face_w[2], baryCoordV, baryCoordW))
                                continue;
                            if (baryCoordW[0] < 0 || baryCoordW[1] < 0 || baryCoordW[2] < 0 || 1 < baryCoordW[0] || 1 < baryCoordW[1] || 1 < baryCoordW[2])
                                continue;
                            double dist = baryCoordV[1];
                            if (dist < picked_dist) {
                                picked_dist = dist;
                                Y::Vector3d hitPosition = baryCoordV[0] * line_v0 + baryCoordV[1] * line_v1;
                                float dist2_min = FLT_MAX;
                                for (TriMesh::FVIter v = trimesh.fv_iter(f); v; ++ v) {
                                    TetMesh::Tetra& tet = tetmesh.tetras_[trimesh.data(v).tetra_id_];
                                    for (int i = 0; i < 4; ++ i) {
                                        Particle* particle = &smObject->particles[tet.vid_[i]];
                                        float dist2 = (particle->x - hitPosition.convert<float>()).length();
                                        if (dist2 < dist2_min) {
                                            dist2_min = dist2;
                                            picked_particle = particle;
                                        }
                                    }
                                }
                                
                                // also pick the neighborhoods of the particle.
                                core.m_constraintTemp.m_neighborParticles.clear();
                                for (std::vector<int>::iterator it = smObject->clusters[picked_particle->main_cluster_id].particle_ids.begin(); it != smObject->clusters[picked_particle->main_cluster_id].particle_ids.end(); ++ it) {
                                    core.m_constraintTemp.m_neighborParticles.push_back(&(smObject->particles[*it]));
                                }
                            }
                        }
                    }
                }
                if (!picked_particle)
                    return;
                core.m_constraintTemp.m_particle = picked_particle;
                core.m_constraintTemp.m_constraintPos = picked_particle->x;
                m_dragMode = DRAGMODE_TEMP;
            }
        } else {    // (state == GLUT_UP)
            if (m_dragMode == DRAGMODE_SELECT) {
                m_dragMode = DRAGMODE_NONE;
                m_select_pos[1].set(x, y2);
                double radius = (m_select_pos[1] - m_select_pos[0]).length();
                // selection procedure
                for (int i = 0; i < smObject.particles.size(); ++i) {
                    Particle& p = smObject.particles[i];
                    Y::Vector3d x3 = static_cast<Y::Vector3f>(p.x).convert<double>();
                    Y::Vector2d x2 = Y::GLUtil::project(x3).xy();
                    if (radius < (x2 - m_select_pos[0]).length())
                        continue;
                    if (p.constraintID != -1)
                        return;     // groups of constrained particles should not overlap
                }
                m_currentConstraintID = core.m_constraints.size();
                core.m_constraints.push_back(Constraint());
                Constraint& c = core.m_constraints.back();
                for (int i = 0; i < smObject.particles.size(); ++i) {
                    Particle& p = smObject.particles[i];
                    Y::Vector3d x3 = static_cast<Y::Vector3f>(p.x).convert<double>();
                    Y::Vector2d x2 = Y::GLUtil::project(x3).xy();
                    if (radius < (x2 - m_select_pos[0]).length())
                        continue;
                    p.constraintID = m_currentConstraintID;
                    p.constraintPos = p.x;
                    c.m_particles.push_back(&p);
                    c.m_xc += p.x;
                }
                c.m_xc *= 1.0f / c.m_particles.size();
            } else if (m_dragMode == DRAGMODE_MANIPULATE) {
                m_dragMode = DRAGMODE_NONE;
            } else if (m_dragMode == DRAGMODE_TEMP) {
                m_dragMode = DRAGMODE_NONE;
                core.m_constraintTemp.m_particle = 0;
                core.m_constraintTemp.m_neighborParticles.clear();
            }
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            core.m_drawer.m_trackball.mouseDown(x, y,
                                                flagShift ? Y::Trackball::DRAGMODE_ZOOM :
                                                flagCtrl  ? Y::Trackball::DRAGMODE_PAN  : Y::Trackball::DRAGMODE_ROTATE);
        } else {
            core.m_drawer.m_trackball.mouseUp();
        }
    }
}
void EventHandler::glut_motion(int x, int y) {
    if (TwEventMouseMotionGLUT(x, y))
        return;
    int y2 = core.m_drawer.m_height - y;
    core.m_drawer.m_trackball.mouseMove(x, y);
    if (m_dragMode == DRAGMODE_SELECT) {
        m_select_pos[1].set(x, y2);
    } else if (m_dragMode == DRAGMODE_MANIPULATE) {
        Y::Vector2d manipulate_pos(x, y2);
        Y::Vector2f d = (manipulate_pos - m_manipulate_prevPos).convert<float>() * 0.01f;
        Constraint& currentConstraint = core.m_constraints[core.m_eventHandler.m_currentConstraintID];
        if (core.m_gui.m_author.m_manipulateMode == Gui::Author::MANIPULATEMODE_TRANSLATE) {
            Y::Vector3d e0 = core.m_drawer.m_trackball.eyePoint;
            Y::Vector3d e1 = Y::GLUtil::unproject(manipulate_pos);
            Y::Vector3d n = (e0 - currentConstraint.m_xc.convert<double>()).normalized();
            Y::Vector3d p = currentConstraint.m_xc.convert<double>();
            // (t e0 + (1 - t) e1 - p) | n = 0, solve for t
            // t (e0 - e1) | n = (p - e1) | n
            double t = ((p - e1) | n) / ((e0 - e1) | n);
            Y::Vector3d x = t * e0 + (1 - t) * e1;
            Y::Vector3d d = x - p;
            currentConstraint.translate(d.convert<float>());
        } else if (core.m_gui.m_author.m_manipulateMode == Gui::Author::MANIPULATEMODE_ROTATE) {
            Y::Vector3f axis = core.m_drawer.m_trackball.eyePoint.convert<float>() - currentConstraint.m_xc;
            currentConstraint.rotate(axis, d.x_ * 0.5f);
        } else if (core.m_gui.m_author.m_manipulateMode == Gui::Author::MANIPULATEMODE_SCALE) {
            currentConstraint.scale(1.0f + d.x_ * 0.5f);
        }
        m_manipulate_prevPos = manipulate_pos;
    } else if (m_dragMode == DRAGMODE_TEMP) {
        Y::Vector3d line_v0 = core.m_drawer.m_trackball.eyePoint;
        Y::Vector3d line_v1 = Y::GLUtil::unproject(Y::Vector2d(x, y2));
        Y::Vector3d line_dir = line_v1 - line_v0;
        Y::Vector3d c = Y::Vector3f(core.m_constraintTemp.m_constraintPos).convert<double>();
        double t = (line_dir | (c - line_v0)) / line_dir.lengthSquared();
        c = (1 - t) * line_v0 + t * line_v1;
        core.m_constraintTemp.m_constraintPos = c.convert<float>();
    }
}
void EventHandler::glut_keyboard (unsigned char key, int x, int y) {
    if (TwEventKeyboardGLUT(key, x, y))
        return;
    switch (key) {
        case 'Q':
        case 'q':
            exit(0);
            break;
    }
}
void EventHandler::glut_special (int key, int x, int y) {
    if (TwEventSpecialGLUT(key, x, y))
        return;
}
