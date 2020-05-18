// This code was written partially in reference to http://www.alecrivers.com/fastlsm/ .

#include "ShapeMatchingObject.h"
#include "Core.h"
#include "Embed.h"
#include "GLUtil.h"
#include "Matrix.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"
#include "invSqrt.h"
#include "jacobi.h"
#include "params.h"
#include <KLIB/DrawUtil.h>
#include <numeric>
#include <tetgen.h>

// If you want to use a tetrahedron as a cluster, put TETRA_CLUSTERS to 1.
// In this case, each cluster has exactly four particles.
// However, the simulation becomes unstable, because of the inherency of the shape matching dynamics.
#define TETRA_CLUSTERS 0

using namespace std;

namespace
{
    Core&   core   = Core::getInstance();
    Sphere& sphere = Sphere::getInstance();
} // namespace

// Translation rules between a symmetric 3x3 matrix and a 6-dimentional vector.
namespace
{
    static Y::Vector6f convert(Y::Matrix3x3f matrix)
    {
        return Y::Vector6f(
            matrix.m[0][0], matrix.m[1][1], matrix.m[2][2], matrix.m[0][1], matrix.m[0][2], matrix.m[1][2]);
    }
    static Y::Matrix3x3f convert(Y::Vector6f vector)
    {
        return Y::Matrix3x3f(
            vector[0], vector[3], vector[4], vector[3], vector[1], vector[5], vector[4], vector[5], vector[2]);
    }
} // namespace

void ShapeMatchingObject::Performance::init()
{
    *this = Performance();
}

void ShapeMatchingObject::Performance::addTick(clock_t tick)
{
    ++m_count;
    double t  = 1000 * tick / CLOCKS_PER_SEC;
    m_timeMax = max<double>(m_timeMax, t);
    m_timeMin = min<double>(m_timeMin, t);
    m_timeTotal += t;
    m_timeAvg = m_timeTotal / m_count;
}

ShapeMatchingObject::Performance::Performance()
    : m_count(0), m_timeMax(0), m_timeMin(DBL_MAX), m_timeTotal(0), m_timeAvg(0)
{
    m_clkData.setShowPercent(false);
    m_clkData.addEntry("misc");
    m_clkData.addEntry("sm");
    m_clkData.addEntry("proj");
    m_clkData.addEntry("damp");
    //    m_clkData.addEntry("embed");
}

ShapeMatchingObject::Config::Config()
    : m_damping(0.03), m_stiffness_alpha(1.0), m_stiffness_iter(1), m_useLocalExample(false), m_beta(0.95f)
{
}

bool ShapeMatchingObject::loadNode(const string& node_fname)
{
    tetgenio tio;
    if (!tio.load_node(const_cast<char*>(node_fname.c_str()))) return false;
    if (tetmesh.unused_indices_.empty())
    {
        if (tio.numberofpoints != particles.size()) return false;
    }
    else
    {
        if (tio.numberofpoints != tetmesh.unused_indices_.size()) return false;
    }
    if (tetmesh.unused_indices_.empty())
    {
        for (int i = 0; i < particles.size(); ++i)
        {
            Particle&   p = particles[i];
            Y::Vector3d pos(&tio.pointlist[3 * i]);
            p.x_example.push_back(pos.convert<float>());
        }
    }
    else
    { // mapping while taking unused vertex indices into account
        int n = static_cast<int>(tetmesh.unused_indices_.size());
        for (int i = 0; i < n; ++i)
        {
            int vid = tetmesh.unused_indices_[i];
            if (vid == -1) continue;
            Particle&   p = particles[vid];
            Y::Vector3d pos(&tio.pointlist[3 * i]);
            p.x_example.push_back(pos.convert<float>());
        }
    }
    precomputeExamples();
    return true;
}

bool ShapeMatchingObject::saveNode(const string& node_fname) const
{
    vector<Y::Vector3d> pointlist;
    for (vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
        pointlist.push_back(Y::Vector3f(p->x).convert<double>());
    tetgenio tio;
    tio.numberofpoints = (int) pointlist.size();
    tio.pointlist      = pointlist.front().ptr();
    tio.save_nodes(const_cast<char*>(node_fname.c_str()));
    return true;
}

bool ShapeMatchingObject::loadXML(const string& fname)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(fname.c_str())) return false;

    TiXmlElement* txe_object = doc.FirstChildElement("object");
    if (!loadXML(txe_object)) return false;

    return true;
}

bool ShapeMatchingObject::loadXML(const TiXmlElement* txe_object)
{
    if (!txe_object) return false;

    const TiXmlElement* txe_tetmesh = txe_object->FirstChildElement("tetmesh");
    if (!txe_tetmesh) return false;

    int n_tetrahedra = 0, n_vertices = 0, n_examples = 0;
    txe_tetmesh->QueryIntAttribute("n_tetrahedra", &n_tetrahedra);
    txe_tetmesh->QueryIntAttribute("n_vertices", &n_vertices);
    txe_tetmesh->QueryIntAttribute("n_examples", &n_examples);

    if (n_tetrahedra == 0 || n_vertices == 0 || n_examples == 0) return false;

    *this   = ShapeMatchingObject();
    m_scene = &core.m_scene;

    tetgenio in;

    // tetrahedronlist
    vector<Y::Vector4i> tetrahedronlist;
    const TiXmlElement* txe_tetlist = txe_tetmesh->FirstChildElement("tetlist");
    const TiXmlElement* txe_tet     = txe_tetlist->FirstChildElement("tet");
    for (int i = 0; i < n_tetrahedra; ++i, txe_tet = txe_tet->NextSiblingElement())
    {
        Y::Vector4i tet;
        txe_tet->QueryIntAttribute("v0", &tet.x_);
        txe_tet->QueryIntAttribute("v1", &tet.y_);
        txe_tet->QueryIntAttribute("v2", &tet.z_);
        txe_tet->QueryIntAttribute("v3", &tet.w_);
        tetrahedronlist.push_back(tet);
    }
    in.numberoftetrahedra = n_tetrahedra;
    in.tetrahedronlist    = tetrahedronlist.front().ptr();

    // pointlist
    vector<vector<Y::Vector3d>> pointlist_examples(n_examples);
    const TiXmlElement*         txe_example = txe_tetmesh->FirstChildElement("example");
    for (int i = 0; i < n_examples; ++i, txe_example = txe_example->NextSiblingElement())
    {
        const TiXmlElement* txe_pos = txe_example->FirstChildElement("pos");
        pointlist_examples[i].resize(n_vertices);
        for (int j = 0; j < n_vertices; ++j, txe_pos = txe_pos->NextSiblingElement())
        {
            Y::Vector3d& pos = pointlist_examples[i][j];
            txe_pos->QueryDoubleAttribute("x", &pos.x_);
            txe_pos->QueryDoubleAttribute("y", &pos.y_);
            txe_pos->QueryDoubleAttribute("z", &pos.z_);
        }
    }
    in.numberofpoints = n_vertices;
    in.pointlist      = pointlist_examples[0].front().ptr();
    tetmesh.init(in);
    in.initialize(); // to prevent double-deleting

    buildFromTetMesh();

    for (int i = 1; i < n_examples; ++i)
        for (int j = 0; j < n_vertices; ++j)
            particles[j].x_example.push_back(pointlist_examples[i][j].convert<float>());

    precomputeExamples();
    core.m_gui.m_author.m_example_use = true;

    const TiXmlElement* txe_trimesh = txe_object->FirstChildElement("trimesh");
    if (txe_trimesh)
    {
        int n_faces = 0, n_trivertices = 0;
        txe_trimesh->QueryIntAttribute("n_faces", &n_faces);
        txe_trimesh->QueryIntAttribute("n_vertices", &n_trivertices);

        trimesh.clear();

        // vertices
        const TiXmlElement* txe_pointlist = txe_trimesh->FirstChildElement("pointlist");
        const TiXmlElement* txe_pos       = txe_pointlist->FirstChildElement("pos");
        for (int i = 0; i < n_trivertices; ++i, txe_pos = txe_pos->NextSiblingElement())
        {
            Y::Vector3d p;
            txe_pos->QueryDoubleAttribute("x", &p.x_);
            txe_pos->QueryDoubleAttribute("y", &p.y_);
            txe_pos->QueryDoubleAttribute("z", &p.z_);
            TriMesh::VHandle v  = trimesh.add_vertex(p);
            trimesh.data(v).x0_ = p;
        }
        // faces
        const TiXmlElement* txe_facelist = txe_trimesh->FirstChildElement("facelist");
        const TiXmlElement* txe_face     = txe_facelist->FirstChildElement("face");
        for (int i = 0; i < n_faces; ++i, txe_face = txe_face->NextSiblingElement())
        {
            Y::Vector3i face;
            txe_face->QueryIntAttribute("v0", &face.x_);
            txe_face->QueryIntAttribute("v1", &face.y_);
            txe_face->QueryIntAttribute("v2", &face.z_);
            vector<TriMesh::VHandle> vhandles(3);
            for (int j = 0; j < 3; ++j)
                vhandles[j] = trimesh.vertex_handle(face[j]);
            trimesh.add_face(vhandles);
        }

        trimesh.request_face_normals();
        trimesh.request_vertex_normals();
        trimesh.setBufferObject();
        embedTriMesh();
    }

    const TiXmlElement* txe_config = txe_object->FirstChildElement("config");
    if (txe_config)
    {
        double damping, stiffness_alpha, beta;
        int    useLocalExample;
        txe_config->QueryDoubleAttribute("damping", &damping);
        txe_config->QueryDoubleAttribute("stiffness_alpha", &stiffness_alpha);
        txe_config->QueryIntAttribute("stiffness_iter", &m_config.m_stiffness_iter);
        txe_config->QueryIntAttribute("useLocalExample", &useLocalExample);
        txe_config->QueryDoubleAttribute("beta", &beta);
        m_config.m_damping         = damping;
        m_config.m_stiffness_alpha = stiffness_alpha;
        m_config.m_useLocalExample = useLocalExample != 0;
        m_config.m_beta            = beta;
    }

    const TiXmlElement* txe_initial = txe_object->FirstChildElement("initial");
    if (txe_initial)
    {
        const TiXmlElement* txe_offset_min = txe_initial->FirstChildElement("offset_min");
        if (txe_offset_min)
        {
            double x, y, z;
            txe_offset_min->QueryDoubleAttribute("x", &x);
            txe_offset_min->QueryDoubleAttribute("y", &y);
            txe_offset_min->QueryDoubleAttribute("z", &z);
            m_initial.m_offset = Y::Vector3f(x, y, z);
        }
        const TiXmlElement* txe_offset_max = txe_initial->FirstChildElement("offset_max");
        if (txe_offset_max)
        {
            double x, y, z;
            txe_offset_max->QueryDoubleAttribute("x", &x);
            txe_offset_max->QueryDoubleAttribute("y", &y);
            txe_offset_max->QueryDoubleAttribute("z", &z);
            m_initial.m_offset = Y::Vector3f(x, y, z);
        }
        const TiXmlElement* txe_velocity = txe_initial->FirstChildElement("velocity");
        if (txe_velocity)
        {
            double x, y, z;
            txe_velocity->QueryDoubleAttribute("x", &x);
            txe_velocity->QueryDoubleAttribute("y", &y);
            txe_velocity->QueryDoubleAttribute("z", &z);
            m_initial.m_velocity = Y::Vector3f(x, y, z);
        }
    }
    const TiXmlElement* txe_color = txe_object->FirstChildElement("color");
    if (txe_color)
    {
        Y::Vector3d color;
        txe_color->QueryDoubleAttribute("r", &color.x_);
        txe_color->QueryDoubleAttribute("g", &color.y_);
        txe_color->QueryDoubleAttribute("b", &color.z_);
        m_color = color.convert<float>();
    }
    reset();

    return true;
}

bool ShapeMatchingObject::saveXML(const string& fname) const
{
    TiXmlDocument doc;
    TiXmlElement* txe_object = new TiXmlElement("object");
    doc.LinkEndChild(txe_object);

    if (!saveXML(txe_object)) return false;

    if (!doc.SaveFile(fname.c_str())) return false;

    return true;
}

bool ShapeMatchingObject::saveXML(TiXmlElement* txe_object) const
{
    TiXmlElement* txe_tetmesh = new TiXmlElement("tetmesh");
    txe_object->LinkEndChild(txe_tetmesh);

    txe_tetmesh->SetAttribute("n_tetrahedra", (int) tetmesh.tetras_.size());
    txe_tetmesh->SetAttribute("n_vertices", (int) tetmesh.vertices_.size());
    txe_tetmesh->SetAttribute("n_examples", numberOfModes);

    // tetrahedronlist
    TiXmlElement* txe_tetlist = new TiXmlElement("tetlist");
    txe_tetmesh->LinkEndChild(txe_tetlist);
    for (vector<TetMesh::Tetra>::const_iterator tet = tetmesh.tetras_.begin(); tet != tetmesh.tetras_.end(); ++tet)
    {
        TiXmlElement* txe_tet = new TiXmlElement("tet");
        txe_tetlist->LinkEndChild(txe_tet);
        txe_tet->SetAttribute("v0", tet->vid_[0]);
        txe_tet->SetAttribute("v1", tet->vid_[1]);
        txe_tet->SetAttribute("v2", tet->vid_[2]);
        txe_tet->SetAttribute("v3", tet->vid_[3]);
    }

    // pointlist
    for (int i = 0; i < numberOfModes; ++i)
    {
        TiXmlElement* txe_example = new TiXmlElement("example");
        txe_tetmesh->LinkEndChild(txe_example);
        for (vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
        {
            TiXmlElement* txe_pos = new TiXmlElement("pos");
            txe_example->LinkEndChild(txe_pos);
            txe_pos->SetDoubleAttribute("x", p->x_example[i].x_);
            txe_pos->SetDoubleAttribute("y", p->x_example[i].y_);
            txe_pos->SetDoubleAttribute("z", p->x_example[i].z_);
        }
    }

    if (!trimesh.empty())
    {
        TiXmlElement* txe_trimesh = new TiXmlElement("trimesh");
        txe_object->LinkEndChild(txe_trimesh);

        txe_trimesh->SetAttribute("n_faces", trimesh.n_faces());
        txe_trimesh->SetAttribute("n_vertices", trimesh.n_vertices());

        // vertices
        TiXmlElement* txe_pointlist = new TiXmlElement("pointlist");
        txe_trimesh->LinkEndChild(txe_pointlist);
        for (TriMesh::CVIter v = trimesh.vertices_begin(); v != trimesh.vertices_end(); ++v)
        {
            Y::Vector3d   p       = trimesh.data(v).x0_;
            TiXmlElement* txe_pos = new TiXmlElement("pos");
            txe_pointlist->LinkEndChild(txe_pos);
            txe_pos->SetDoubleAttribute("x", p.x_);
            txe_pos->SetDoubleAttribute("y", p.y_);
            txe_pos->SetDoubleAttribute("z", p.z_);
        }

        // faces
        TiXmlElement* txe_facelist = new TiXmlElement("facelist");
        txe_trimesh->LinkEndChild(txe_facelist);
        for (TriMesh::CFIter f = trimesh.faces_begin(); f != trimesh.faces_end(); ++f)
        {
            TiXmlElement* txe_face = new TiXmlElement("face");
            txe_facelist->LinkEndChild(txe_face);
            TriMesh::CFVIter v = trimesh.cfv_iter(f);
            txe_face->SetAttribute("v0", v.handle().idx());
            ++v;
            txe_face->SetAttribute("v1", v.handle().idx());
            ++v;
            txe_face->SetAttribute("v2", v.handle().idx());
        }
    }

    TiXmlElement* txe_config = new TiXmlElement("config");
    txe_object->LinkEndChild(txe_config);

    txe_config->SetDoubleAttribute("damping", m_config.m_damping);
    txe_config->SetDoubleAttribute("stiffness_alpha", m_config.m_stiffness_alpha);
    txe_config->SetAttribute("stiffness_iter", m_config.m_stiffness_iter);
    txe_config->SetAttribute("useLocalExample", static_cast<int>(m_config.m_useLocalExample));
    txe_config->SetDoubleAttribute("beta", m_config.m_beta);

    TiXmlElement* txe_initial = new TiXmlElement("initial");
    txe_object->LinkEndChild(txe_initial);

    {
        TiXmlElement* txe_offset_min = new TiXmlElement("offset_min");
        txe_initial->LinkEndChild(txe_offset_min);

        txe_offset_min->SetDoubleAttribute("x", m_initial.m_offset.x_);
        txe_offset_min->SetDoubleAttribute("y", m_initial.m_offset.y_);
        txe_offset_min->SetDoubleAttribute("z", m_initial.m_offset.z_);

        TiXmlElement* txe_offset_max = new TiXmlElement("offset_max");
        txe_initial->LinkEndChild(txe_offset_max);

        txe_offset_max->SetDoubleAttribute("x", m_initial.m_offset.x_);
        txe_offset_max->SetDoubleAttribute("y", m_initial.m_offset.y_);
        txe_offset_max->SetDoubleAttribute("z", m_initial.m_offset.z_);

        TiXmlElement* txe_velocity = new TiXmlElement("velocity");
        txe_initial->LinkEndChild(txe_velocity);

        txe_velocity->SetDoubleAttribute("x", m_initial.m_velocity.x_);
        txe_velocity->SetDoubleAttribute("y", m_initial.m_velocity.y_);
        txe_velocity->SetDoubleAttribute("z", m_initial.m_velocity.z_);
    }

    TiXmlElement* txe_color = new TiXmlElement("color");
    txe_object->LinkEndChild(txe_color);
    txe_color->SetDoubleAttribute("r", m_color.x_);
    txe_color->SetDoubleAttribute("g", m_color.y_);
    txe_color->SetDoubleAttribute("b", m_color.z_);

    return true;
}

void ShapeMatchingObject::reset()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        Particle& p = particles[i];
        p.x         = p.x0 + m_initial.m_offset;
        p.v         = m_initial.m_velocity;
        p.f         = Y::Vector3f(0.0, 0.0, 0.0);
    }
    updateTriMesh();
    m_performance.init();
}

void ShapeMatchingObject::buildFromTetMesh()
{
    // add particles
    particles.clear();
    particles.resize(tetmesh.vertices_.size());
    for (int i = 0; i < tetmesh.vertices_.size(); i++)
    {
        TetMesh::Vertex& vertex = tetmesh.vertices_[i];
        Particle&        p      = particles[i];
        p.m                     = core.m_physParam.m_mass / (vertex.neighbor_vid_.size() + 1.0f);
        p.x                     = vertex.position_;
        p.x0                    = vertex.position_;
        p.id                    = i;
        p.x_example.push_back(vertex.position_);
        vertex.particle_id_ = i;
    }

    // make clusters
#if TETRA_CLUSTERS
    // a tetrahedron as a cluster
    clusters.clear();
    clusters.resize(tetmesh.tetras_.size());
    for (int i = 0; i < tetmesh.tetras_.size(); ++i)
    {
        TetMesh::Tetra& tetra   = tetmesh.tetras_[i];
        Cluster&        cluster = clusters[i];
        for (int j = 0; j < 4; ++j)
        {
            Particle& p = particles[tetra.vid_[j]];
            p.cluster_ids.push_back(i);
            p.main_cluster_id = i;
            cluster.particle_ids.push_back(tetra.vid_[j]);
        }
        cluster.id = i;
    }
#else
    // 1-ring neighborhood
    clusters.clear();
    clusters.resize(tetmesh.vertices_.size());
    for (int i = 0; i < tetmesh.vertices_.size(); ++i)
    {
        TetMesh::Vertex& vertex  = tetmesh.vertices_[i];
        Cluster&         cluster = clusters[i];
        particles[i].cluster_ids.push_back(i);
        particles[i].main_cluster_id = i;
        cluster.particle_ids.push_back(i);
        for (list<int>::iterator j = vertex.neighbor_vid_.begin(); j != vertex.neighbor_vid_.end(); ++j)
        {
            particles[*j].cluster_ids.push_back(i);
            cluster.particle_ids.push_back(*j);
        }
        cluster.id = i;
    }
#endif

    printf("#clusters: %d\n#particles: %d\n", (int) clusters.size(), (int) particles.size());

    calculateStaticVariables();
    precomputeUstatic();
    precomputeExamples();
    trimesh.clear();
}

void ShapeMatchingObject::embedTriMesh()
{
    for (TriMesh::VIter v = trimesh.vertices_begin(); v != trimesh.vertices_end(); ++v)
    {
        TriMesh::VertexData& vdata   = trimesh.data(v);
        Y::Vector3f          vpos    = vdata.x0_.convert<float>();
        double               distMin = DBL_MAX;
        for (int tetra_id = 0; tetra_id < tetmesh.tetras_.size(); ++tetra_id)
        {
            TetMesh::Tetra& tetra = tetmesh.tetras_[tetra_id];
            Y::Vector3f     tpos[4];
            for (int i = 0; i < 4; ++i)
            {
                tpos[i] = tetmesh.vertices_[tetra.vid_[i]].position_;
            }
            Y::Vector4f baryCoord = Y::calculteBaryCoordFromTetrahedron(vpos, tpos[0], tpos[1], tpos[2], tpos[3]);

            Y::Vector4d coord_dist;
            for (int i = 0; i < 4; ++i)
            {
                double coord  = baryCoord[i];
                coord_dist[i] = coord < 0 ? -coord : 1 < coord ? (coord - 1) : 0;
            }
            double dist = coord_dist.lengthSquared();
            if (dist < distMin)
            {
                distMin          = dist;
                vdata.tetra_id_  = tetra_id;
                vdata.baryCoord_ = Y::Vector4d(baryCoord[0], baryCoord[1], baryCoord[2], baryCoord[3]);
            }
            if (distMin == 0) break;
        }
    }
}

void ShapeMatchingObject::updateTriMesh()
{
    for (TriMesh::VIter v = trimesh.vertices_begin(); v != trimesh.vertices_end(); ++v)
    {
        TriMesh::VertexData& vdata = trimesh.data(v);
        Y::Vector3f          vpos;
        TetMesh::Tetra&      tetra = tetmesh.tetras_[vdata.tetra_id_];
        for (int i = 0; i < 4; ++i)
        {
            int particle_id = tetmesh.vertices_[tetra.vid_[i]].particle_id_;
            vpos += particles[particle_id].x * vdata.baryCoord_[i];
        }
        trimesh.set_point(v, Y::Vector3f(vpos).convert<double>());
    }
    trimesh.update_normals();
    trimesh.updateVBO();
}

void ShapeMatchingObject::simulateOneStep(float dt)
{
    if (particles.empty()) return;
    m_performance.m_clkStart = new Y::ClockStart(&m_performance.m_clkData);
    clock_t t0               = clock();
    calculateForce(dt);
    calculateVelocity(dt);
    applyVelocity(dt);
    m_lifetime = max<int>(-1, m_lifetime - 1);
    m_performance.addTick(clock() - t0);
    delete m_performance.m_clkStart;
    static int cnt = 0;
    cnt            = (cnt + 1) % 1000;
    if (!cnt)
    {
        m_performance.m_clkData.print();
        m_performance.m_clkData.resetTick();
    }
}

void ShapeMatchingObject::calculateForce(float dt)
{
    if (tetmesh.empty()) return;
    const float       k = core.m_physParam.m_airResistance;
    const Y::Vector3f g = Y::Vector3f(0.0f, -core.m_physParam.m_gravity, 0.0f);

    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];
        p.f         = Y::Vector3f(0.0, 0.0, 0.0);

        // apply gravity
        if (m_scene->m_useGravity) p.f += g * p.m;

        // apply air registance (very simple model) (for stability)
        p.f += -k * p.v;

        if (p.constraintID != -1) p.f += (p.constraintPos - p.x) * core.m_physParam.m_springCoef;

        if (&p == core.m_constraintTemp.m_particle)
        {
            p.f += (core.m_constraintTemp.m_constraintPos - p.x) * core.m_physParam.m_springCoef;
        }

        // we also pull the neighborhoods of the picked particle
        static const float power = 0.5f;
        for (vector<Particle*>::iterator it = core.m_constraintTemp.m_neighborParticles.begin();
             it != core.m_constraintTemp.m_neighborParticles.end();
             ++it)
        {
            if (&p == *it)
            {
                p.f += (core.m_constraintTemp.m_constraintPos - p.x) * core.m_physParam.m_springCoef * power;
            }
        }
    }
}

void ShapeMatchingObject::calculateVelocity(float dt)
{
    if (tetmesh.empty()) return;
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        p->x_tmp = p->x;
    }

    // do shape matching
    for (int k = 0; k < m_config.m_stiffness_iter; ++k)
    {
        m_performance.m_clkStart->setCurrentIndex(1); // measure t_sm
        updateParticles();
        updateClusters();
        shapeMatching();
        calculateGoalPositions();
        for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        {
            p->x = p->gx;
        }
    }

    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        p->x = p->x_tmp;

    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];

        p.v += (dt / p.m) * p.f;
        p.v += (m_config.m_stiffness_alpha / dt) * (p.gx - p.x);
    }

    m_performance.m_clkStart->setCurrentIndex(3); // measure t_damp
    // damping
    dampingGlobal(dt);
}

void ShapeMatchingObject::applyVelocity(float dt)
{
    m_performance.m_clkStart->setCurrentIndex(1); // measure t_misc
    if (tetmesh.empty()) return;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];

        p.x += dt * p.v;

        for (vector<Wall>::iterator wall = m_scene->m_walls.begin(); wall != m_scene->m_walls.end(); ++wall)
            wall->processCollision(p);
    }
}

/////////////////////////////////////////////////////////

void ShapeMatchingObject::render(bool shadowMode)
{
    // render particles
    if (core.m_gui.m_general.m_renderMode != Gui::General::RENDERMODE_TRIMESH)
    {
        renderParticles(shadowMode);
    }
    // render trimesh
    if (core.m_gui.m_general.m_renderMode != Gui::General::RENDERMODE_PARTICLE)
    {
        renderTrimesh(shadowMode);
    }

    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];
        if (&p == core.m_constraintTemp.m_particle)
        {
            static const float s = BALL_SIZE;
            glMatrixMode(GL_MODELVIEW);
            Y::Vector3f& c = core.m_constraintTemp.m_constraintPos;

            glColor3d(1, 0, 0);
            glPushMatrix();
            glTranslatef(p.x.x_, p.x.y_, p.x.z_);
            glScaled(s, s, s);
            sphere.renderObject();
            glPopMatrix();
            glColor3d(1, 0, 0);
            glPushMatrix();
            glTranslatef(c.x_, c.y_, c.z_);
            glScaled(s, s, s);
            sphere.renderObject();
            glPopMatrix();

            glColor3d(0.8f, 0.1f, 0.0f);
            glLineWidth(LINE_WIDTH);
            glBegin(GL_LINES);
            glVertex3fv(p.x.val);
            glVertex3fv(c.val);
            glEnd();
        }
    }
}

void ShapeMatchingObject::renderParticles(bool shadowMode)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        Particle& p = particles[i];
        glColor3f(m_color[0], m_color[1], m_color[2]);
        p.renderParticle(shadowMode);
        if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR && core.m_gui.m_author.m_example_show)
        {
            glColor3f(m_color[0], m_color[1], m_color[2]);
            for (int j = 1; j < numberOfModes; ++j)
            {
                glPushMatrix();
                Y::Vector3f& x = p.x_example[j];
                glTranslatef(x.x, x.y, x.z);
                sphere.renderObject();
                glPopMatrix();
            }
        }
    }
}

void ShapeMatchingObject::renderTrimesh(bool shadowMode)
{
    glColor3f(m_color[0], m_color[1], m_color[2]);
    trimesh.renderObject();

    if (core.m_gui.m_general.m_demoMode == Gui::General::DEMOMODE_AUTHOR && core.m_gui.m_author.m_example_show)
    {
        for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        {
            p->x_tmp = p->x;
        }
        for (int j = 1; j < numberOfModes; ++j)
        {
            for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
            {
                p->x = p->x_example[j];
            }
            updateTriMesh();
            trimesh.renderObject();
        }
        for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        {
            p->x = p->x_tmp;
        }
        updateTriMesh();
    }
}

/////////////////////////////////////////
// simulation methods
/////////////////////////////////////////

void ShapeMatchingObject::calculateStaticVariables()
{
    for (int i = 0; i < clusters.size(); i++)
    {
        Cluster& c = clusters[i];
        c.calculateSumOfMass(this);
        c.calculateRestCenterPoint(this);
    }
}

void ShapeMatchingObject::updateParticles()
{
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];
        p.updateMxx0t();
    }
}

void ShapeMatchingObject::updateClusters()
{
    for (int i = 0; i < clusters.size(); i++)
    {
        clusters[i].updateCenterPoint(this);
    }
}

void ShapeMatchingObject::shapeMatching()
{
    for (int i = 0; i < clusters.size(); i++)
    {
        Cluster* c = &clusters[i];

        // copied from precomputeExamples_Umode()   (need to be consistent with it!)
        Y::Matrix3x3f Fd    = c->getMomentMatrix(this);
        Y::Matrix3x3f F     = Fd * c->Ustatic;
        Y::Matrix3x3f Umode = F.transpose() * F;

        // Matrix3x3f Eigen = Y::Matrix3x3f(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
        Umode = c->Eigen.transpose() * Umode * c->Eigen; // "warm start" [Rivers07]

        float eigenValues[3];
        Y::jacobi(3, (float*) Umode.m, (float*) eigenValues, (float*) c->Eigen.m);
        for (int j = 0; j < 3; j++)
        {
            if (eigenValues[j] <= 0.0f)
            {
                eigenValues[j] = 0.001f;
            }
            eigenValues[j] = Y::invSqrt(eigenValues[j]);
        }
        Y::Matrix3x3f DPrime =
            Y::Matrix3x3f(eigenValues[0], 0.0f, 0.0f, 0.0f, eigenValues[1], 0.0f, 0.0f, 0.0f, eigenValues[2]);
        Umode = c->Eigen * DPrime * c->Eigen.transpose();

        Y::Matrix3x3f R = F * Umode;
        Umode.invert();
        if (R.determinant() < 0)
        {
            R *= -1;
            Umode *= -1;
        }
        c->R = R;
        c->U = convert(Umode);
    }
}

namespace
{
    void eliminateNegativeWeight(vector<double>& weight)
    {
        const int n = (int) weight.size();
        while (true)
        {
            double minValue = 0;
            int    minIndex = -1;
            for (int i = 0; i < n; ++i)
                if (weight[i] < minValue)
                {
                    minValue = weight[i];
                    minIndex = i;
                }
            if (minIndex == -1) break;
            weight[minIndex] = 0;
            double d         = -minValue / (n - 1);
            for (int i = 0; i < n; ++i)
                if (i != minIndex) weight[i] -= d;
        }
    }
    void applyBeta(vector<double>& weight, double beta)
    {
        double sum = 0.0;
        for (int i = 0; i < weight.size(); ++i)
        {
            sum += weight[i];
            weight[i] *= beta;
        }
        weight[0] += (1.0 - beta) * sum;
    }
} // namespace

void ShapeMatchingObject::calculateGoalPositions()
{
    const int n = numberOfModes;

    vector<double> weight_global(n, 1);
    m_performance.m_clkStart->setCurrentIndex(2); // measure t_proj
    if (1 < n && core.m_gui.m_author.m_example_use && !m_config.m_useLocalExample)
    {
        KLIB::VectorVLd weight_global_tmp(n - 1, 0.0);
        for (vector<Cluster>::iterator c = clusters.begin(); c != clusters.end(); ++c)
        {
            const Y::Vector6f& Umode_back = c->Umode.back();
            Y::Vector6f        tmp        = c->U - Umode_back;
            for (int i = 0; i < n - 1; ++i)
            {
                Y::Vector6f tmp2 = c->Umode[i] - Umode_back;
                weight_global_tmp[i] += (c->Umode[i] - Umode_back).dot_product(tmp);
            }
        }
        weight_global_tmp = AtA_inv * weight_global_tmp;
        for (int i = 0; i < n - 1; ++i)
        {
            weight_global[i] = weight_global_tmp[i];
            weight_global[n - 1] -= weight_global_tmp[i];
        }
        if (core.m_gui.m_tmp.m_weightClamp)
        {
            eliminateNegativeWeight(weight_global);
        }
        applyBeta(weight_global, m_config.m_beta);
    }
    if (core.m_gui.m_tmp.m_flag && n == 2)
    {
        weight_global.clear();
        weight_global.resize(n, 0);
        weight_global[0] = 1 - core.m_gui.m_tmp.m_real0;
        weight_global[1] = core.m_gui.m_tmp.m_real0;
    }

    m_performance.m_clkStart->setCurrentIndex(1); // measure t_sm

    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        p->gxs.clear();
    }

    int n_clusters = (int) clusters.size();

    for (int cluster_id = 0; cluster_id < n_clusters; ++cluster_id)
    {
        Cluster*                 c = &clusters[cluster_id];
        static const Y::Vector6f I(1, 1, 1, 0, 0, 0);
        Y::Vector6f              Udash;
        m_performance.m_clkStart->setCurrentIndex(2); // measure t_proj
        if (1 < n && core.m_gui.m_author.m_example_use)
        {
            if (m_config.m_useLocalExample)
            {
                vector<double>     weight_local(n, 1);
                KLIB::VectorVLd    weight_local_tmp(n - 1, 0.0);
                const Y::Vector6f& Umode_back = c->Umode.back();
                Y::Vector6f        tmp        = c->U - Umode_back;
                for (int i = 0; i < n - 1; ++i)
                {
                    weight_local_tmp[i] = (c->Umode[i] - Umode_back).dot_product(tmp);
                }
                weight_local_tmp = c->AtA_inv * weight_local_tmp;
                for (int i = 0; i < n - 1; ++i)
                {
                    weight_local[i] = weight_local_tmp[i];
                    weight_local[n - 1] -= weight_local_tmp[i];
                }
                if (core.m_gui.m_tmp.m_weightClamp)
                {
                    eliminateNegativeWeight(weight_local);
                }
                applyBeta(weight_local, m_config.m_beta);
                for (int i = 0; i < n; ++i)
                    Udash += c->Umode[i] * weight_local[i];
            }
            else
            {
                for (int i = 0; i < n; ++i)
                    Udash += c->Umode[i] * weight_global[i];
            }
        }
        else
        {
            Udash = I;
        }
        Y::Matrix3x3f Fdash = c->R * convert(Udash);
        m_performance.m_clkStart->setCurrentIndex(1); // measure t_sm
        for (vector<int>::iterator i = c->particle_ids.begin(); i != c->particle_ids.end(); ++i)
        {
            Particle& p = particles[*i];
            p.gxs.push_back(Fdash * (p.x0 - c->xc0) + c->xc);
        }
    }
    m_performance.m_clkStart->setCurrentIndex(0); // measure t_misc
    int n_particles = (int) particles.size();

    for (int particle_id = 0; particle_id < n_particles; ++particle_id)
    {
        Particle* p = &particles[particle_id];
        p->gx       = std::accumulate(p->gxs.begin(), p->gxs.end(), Y::Vector3f(0.0, 0.0, 0.0));
        p->gx /= p->gxs.size();
    }
}

// reference: Position Based Dynamics [Muller07]
void ShapeMatchingObject::dampingGlobal(float dt)
{
    // get xcm, vcm, and sum of mass.
    Y::Vector3f xcm = Y::Vector3f(0.0f, 0.0f, 0.0f);
    Y::Vector3f vcm = Y::Vector3f(0.0f, 0.0f, 0.0f);
    float       m   = 0.0f;
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        xcm += p->x * p->m;
        vcm += p->v * p->m;
        m += p->m;
    }
    xcm *= 1.0f / m;
    vcm *= 1.0f / m;

    // get L, I, Ri
    Y::Vector3f   L = Y::Vector3f(0.0f, 0.0f, 0.0f);
    Y::Matrix3x3f I = Y::Matrix3x3f(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        Y::Vector3f Ri  = p->x - xcm;
        Y::Vector3f tmp = Ri % (p->m * p->v);
        L += tmp;
        Y::Matrix3x3f RTilde = Y::Matrix3x3f(0.0f, -Ri.z, Ri.y, Ri.z, 0.0f, -Ri.x, -Ri.y, Ri.x, 0.0f);
        Y::Matrix3x3f tmpM   = RTilde * RTilde.transpose();
        I += tmpM * p->m;
    }

    // get w
    Y::Vector3f w = I.inverse() * L;

    // apply the damping
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
    {
        Y::Vector3f Ri  = p->x - xcm;
        Y::Vector3f DVi = vcm + (w % Ri) - p->v;
        p->v += m_config.m_damping * DVi;
    }
}

void ShapeMatchingObject::precomputeUstatic()
{
    for (int i = 0; i < clusters.size(); i++)
    {
        Cluster&      cluster = clusters[i];
        Y::Matrix3x3f Fs      = Y::Matrix3x3f(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        for (vector<int>::iterator j = cluster.particle_ids.begin(); j != cluster.particle_ids.end(); ++j)
        {
            Particle&     p   = particles[*j];
            Y::Vector3f   qi  = p.x0 - cluster.xc0;
            Y::Matrix3x3f tmp = Y::Matrix3x3f(qi.x * qi.x,
                                              qi.x * qi.y,
                                              qi.x * qi.z,
                                              qi.y * qi.x,
                                              qi.y * qi.y,
                                              qi.y * qi.z,
                                              qi.z * qi.x,
                                              qi.z * qi.y,
                                              qi.z * qi.z);
            Fs += (p.m * tmp);
        }
        Fs.invert();

        cluster.Ustatic = Fs;
    }
}

void ShapeMatchingObject::precomputeExamples()
{
    precomputeExamples_Umode();
    precomputeExamples_AtA();
    core.m_gui.m_author.m_example_use = true;
}

void ShapeMatchingObject::precomputeExamples_Umode()
{
    numberOfModes = static_cast<int>(particles.front().x_example.size());

    // store current configuration
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        p->x_tmp = p->x;

    // compute Umode for each cluster
    for (vector<Cluster>::iterator c = clusters.begin(); c != clusters.end(); ++c)
        c->Umode.resize(numberOfModes);
    for (int i = 0; i < numberOfModes; ++i)
    {
        for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
            p->x = p->x_example[i];
        updateParticles();
        updateClusters();

        for (vector<Cluster>::iterator c = clusters.begin(); c != clusters.end(); ++c)
        {
            // get Fdynamic
            Y::Matrix3x3f Fd    = c->getMomentMatrix(this);
            Y::Matrix3x3f F     = Fd * c->Ustatic;
            Y::Matrix3x3f Umode = F.transpose() * F;
            Y::Matrix3x3f Eigen = Y::Matrix3x3f(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
            float         eigenValues[3];
            Y::jacobi(3, (float*) Umode.m, (float*) eigenValues, (float*) Eigen.m);
            for (int j = 0; j < 3; j++)
            {
                if (eigenValues[j] <= 0.0f)
                {
                    eigenValues[j] = 0.0001f;
                }
                eigenValues[j] = Y::invSqrt(eigenValues[j]);
            }
            Y::Matrix3x3f DPrime =
                Y::Matrix3x3f(eigenValues[0], 0.0f, 0.0f, 0.0f, eigenValues[1], 0.0f, 0.0f, 0.0f, eigenValues[2]);
            Umode = Eigen * DPrime * Eigen.transpose();

            Y::Matrix3x3f R = F * Umode;
            if (!Umode.invert())
            {
                printf("error: fail to invert Umode\n");
            }
            if (R.determinant() < 0)
            {
                R *= -1;
                Umode *= -1;
            }
            c->Umode[i] = convert(Umode);
        }
    }

    // restore current configuration
    for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
        p->x = p->x_tmp;
}

void ShapeMatchingObject::precomputeExamples_AtA()
{
    int n = numberOfModes - 1;
    if (0 < n)
    {
        KLIB::MatrixVLd AtA_global(n, n);
        for (vector<Cluster>::iterator c = clusters.begin(); c != clusters.end(); ++c)
        {
            KLIB::MatrixVLd AtA_local(n, n);
            for (int i = 0; i < n; ++i)
            {
                Y::Vector6f Umode_i = c->Umode[i] - c->Umode.back();
                for (int j = i; j < n; ++j)
                {
                    Y::Vector6f Umode_j = c->Umode[j] - c->Umode.back();
                    float       d       = Umode_i.dot_product(Umode_j);
                    AtA_local(i, j)     = d;
                    if (i != j) AtA_local(j, i) = d;
                }
            }
            AtA_global += AtA_local;
            for (int i = 0; i < n; ++i)
                AtA_local(i, i) += 0.00001;
            c->AtA_inv = AtA_local.invert();
        }
        AtA_inv = AtA_global.invert();
    }
}

ShapeMatchingObject::ShapeMatchingObject() : numberOfModes(0), m_scene(0), m_lifetime(-1), m_color(0.2, 0.6, 1.0) {}
