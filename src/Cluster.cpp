#include "ShapeMatchingObject.h"
#include "Core.h"

using namespace std;

namespace {
    Core& core = Core::getInstance();
}

Cluster::Cluster()
: id(-1)
, Eigen(Y::Matrix3x3f(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0))
{}

Cluster::~Cluster() {
    
}

void Cluster::calculateSumOfMass(ShapeMatchingObject* object) {
    float tmp = 0.0f;
    for (vector<int>::iterator i = particle_ids.begin(); i != particle_ids.end(); ++i)
        tmp += object->particles[*i].m;
    sumOfMass = tmp;
}

void Cluster::calculateRestCenterPoint(ShapeMatchingObject* object) {
    Y::Vector3f tmp = Y::Vector3f(0.0, 0.0, 0.0);
    for (vector<int>::iterator i = particle_ids.begin(); i != particle_ids.end(); ++i) {
       tmp += object->particles[*i].x0 * object->particles[*i].m;
    }
    tmp /= sumOfMass;
    xc0 = tmp;
}

void Cluster::updateCenterPoint(ShapeMatchingObject* object) {
    Y::Vector3f tmp = Y::Vector3f(0.0, 0.0, 0.0);
    for (vector<int>::iterator i = particle_ids.begin(); i != particle_ids.end(); ++i) {
        tmp += object->particles[*i].x * object->particles[*i].m;
    }
    tmp /= sumOfMass;
    xc = tmp;
}

Y::Matrix3x3f Cluster::getMomentMatrix(ShapeMatchingObject* object) {
    Y::Matrix3x3f mcc0t = Y::Matrix3x3f(sumOfMass * xc.x_ * xc0.x_,
                                  sumOfMass * xc.x_ * xc0.y_,
                                  sumOfMass * xc.x_ * xc0.z_, 
                                  sumOfMass * xc.y_ * xc0.x_,
                                  sumOfMass * xc.y_ * xc0.y_,
                                  sumOfMass * xc.y_ * xc0.z_, 
                                  sumOfMass * xc.z_ * xc0.x_, 
                                  sumOfMass * xc.z_ * xc0.y_,
                                  sumOfMass * xc.z_ * xc0.z_);
    Y::Matrix3x3f sum = Y::Matrix3x3f(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    for (vector<int>::iterator i = particle_ids.begin(); i != particle_ids.end(); ++i)
        sum += object->particles[*i].mxx0t;
    Y::Matrix3x3f sumMinusMcc0t = sum - mcc0t;
    A = sumMinusMcc0t * Ustatic;
    return sumMinusMcc0t;
}

