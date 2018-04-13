#pragma once

#include "Vector.h"

namespace Y {
    Vector4f calculteBaryCoordFromTetrahedron(Vector3f x, Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4);
}
