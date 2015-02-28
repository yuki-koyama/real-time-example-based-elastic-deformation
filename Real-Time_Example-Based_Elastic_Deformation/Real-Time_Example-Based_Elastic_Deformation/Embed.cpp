#include "Embed.h"
#include "Matrix.h"

using namespace Y;

Vector4f Y::calculteBaryCoordFromTetrahedron(Vector3f x, Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4) {
    Vector3f vpos = x;
    Vector3f tpos[4] = { p1, p2, p3, p4 };
    Vector3f b;
    Matrix3x3f A;
    for (int i = 0; i < 3; ++i) {
        b[i] = (vpos - tpos[3]).dot_product(tpos[i] - tpos[3]);
        for (int j = i; j < 3; ++j)
            A.m[i][j] = A.m[j][i] = (tpos[i] - tpos[3]).dot_product(tpos[j] - tpos[3]);
    }
    if (!A.invert()) {
        printf("Error (Embed.cpp): cannot invert the matrix.\n");
    }
    Matrix3x3f A_inv = A;
    Vector3f x_ = A_inv * b;
    Vector4f baryCoord(0.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 3; ++ i) {
        baryCoord[i] = x_[i];
        baryCoord[3] -= x_[i];
    }
    return baryCoord;
}
