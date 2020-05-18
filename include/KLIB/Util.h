#pragma once
#include "Matrix.h"
#include <cmath>

namespace KLIB {

class Util {
    Util();
    ~Util();
public:
    static void flip_bool(bool &b) { b = !b; }
    static Matrix<float, 3, 3> rotationFromAxisAnglef(Vector<float, 3>& axis, float angle) {
        Matrix<float, 3, 3> m;
		axis.normalize();
		float c = cos(angle);
		float s = sin(angle);
		float omc = 1.0 - c;
		m(0, 0) = c + axis[0] * axis[0] * omc;
		m(1, 1) = c + axis[1] * axis[1] * omc;
		m(2, 2) = c + axis[2] * axis[2] * omc;
		
		float tmp1, tmp2;
        
        tmp1 = axis[0] * axis[1] * omc;
		tmp2 = axis[2] * s;
		m(0, 1) = tmp1 - tmp2;
		m(1, 0) = tmp1 + tmp2;
		
		tmp1 = axis[2] * axis[0] * omc;
		tmp2 = axis[1] * s;
		m(2, 0) = tmp1 - tmp2;
		m(0, 2) = tmp1 + tmp2;
		
		tmp1 = axis[1] * axis[2] * omc;
		tmp2 = axis[0] * s;
		m(1, 2) = tmp1 - tmp2;
		m(2, 1) = tmp1 + tmp2;
        
        return m;
    }

    template <typename Scalar>
    static Matrix<Scalar, 3, 3> rotationFromAxisAngle(Vector<Scalar, 3>& axis, Scalar angle) {
        Matrix<Scalar, 3, 3> m;
		axis.normalize();
		Scalar c = cos(angle);
		Scalar s = sin(angle);
		Scalar omc = 1.0 - c;
		m(0, 0) = c + axis[0] * axis[0] * omc;
		m(1, 1) = c + axis[1] * axis[1] * omc;
		m(2, 2) = c + axis[2] * axis[2] * omc;
		
		Scalar tmp1, tmp2;
        
        tmp1 = axis[0] * axis[1] * omc;
		tmp2 = axis[2] * s;
		m(0, 1) = tmp1 - tmp2;
		m(1, 0) = tmp1 + tmp2;
		
		tmp1 = axis[2] * axis[0] * omc;
		tmp2 = axis[1] * s;
		m(2, 0) = tmp1 - tmp2;
		m(0, 2) = tmp1 + tmp2;
		
		tmp1 = axis[1] * axis[2] * omc;
		tmp2 = axis[0] * s;
		m(1, 2) = tmp1 - tmp2;
		m(2, 1) = tmp1 + tmp2;

        return m;
    }
    template <typename Scalar>
    static Matrix<Scalar, 3, 3> rotationFromTwoVectors(Vector<Scalar, 3>& v0, Vector<Scalar, 3>& v1) {
        Vector<Scalar, 3> axis = v0 % v1;
        Scalar angle = Util::calcAngle(v0, v1);
        return Util::rotationFromAxisAngle(axis, angle);
    }
    template <typename Scalar, int N>
    static Scalar distanceSegmentPoint(const Vector<Scalar, N>& segment0, const Vector<Scalar, N>& segment1, const Vector<Scalar, N>& pos) {
        Vector<Scalar, N> v[2] = { segment0, segment1 };
        Vector<Scalar, 2>& baryCoord = calcBarycentricCoord<Scalar, N, 2>(v, pos);
//        double sum = baryCoord.elemSum();
        if (baryCoord[0] <= 0) return (segment1 - pos).length();
        if (baryCoord[1] <= 0) return (segment0 - pos).length();
        return (baryCoord[0] * segment0 + baryCoord[1] * segment1 - pos).length();
    }
    static double calcVolume(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2, const Vector3d& v3) {
        return (std::abs)((v1 - v0) | ((v2 - v0) % (v3 - v0))) / 6;
    }
    template <typename Scalar>
    static Vector<Scalar, 3> calcNormal(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1, const Vector<Scalar, 3>& v2) {
        Vector<Scalar, 3> v01 = v1 - v0;
        Vector<Scalar, 3> v02 = v2 - v0;
        Vector<Scalar, 3> n = v01 % v02;
        n.normalize();
        return n;
    }
    template <typename Scalar>
    static Scalar calcArea(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1, const Vector<Scalar, 3>& v2) {
        Vector<Scalar, 3>& v01 = v1 - v0;
        Vector<Scalar, 3>& v02 = v2 - v0;
        return 0.5 * (v01 % v02).length();
    }
    template <typename Scalar>
    static Scalar calcArea(const Vector<Scalar, 2>& v0, const Vector<Scalar, 2>& v1, const Vector<Scalar, 2>& v2) {
        Vector<Scalar, 3> v01(v1[0] - v0[0], v1[1] - v0[1], 0.);
        Vector<Scalar, 3> v02(v2[0] - v0[0], v2[1] - v0[1], 0.);
        Vector<Scalar, 3>& vn = v01 % v02;
        return 0.5 * vn.length() * (vn[2] < 0 ? -1 : 1);
    }
    template <typename Scalar, int N>
    static Scalar calcAngle(Vector<Scalar, N> v0, Vector<Scalar, N> v1) {
        Scalar r0 = v0.length();
        Scalar r1 = v1.length();
        if (r0 == 0 || r1 == 0)
            throw "One or both of two vectors are zero.";
        v0 /= r0;
        v1 /= r1;
        Scalar cosine = v0 | v1;
        v1 -= cosine * v0;
        Scalar sine = v1.length();
        Scalar angle = atan2(sine, cosine);
        return angle;
    }
    template <typename Scalar>
    static bool calcIntersectionLineTriangle(
        const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1,
        const Vector<Scalar, 3>& w0, const Vector<Scalar, 3>& w1, const Vector<Scalar, 3>& w2, 
        Vector<Scalar, 2>& baryCoordV, Vector<Scalar, 3>& baryCoordW)
    {
        /*
        * tv0 * v0 + tv1 * v1 = tw0 * w0 + tw1 * w1 + tw2 * w2
        * tv0 + tv1 = 1
        * tw0 + tw1 + tw2 = 1
        * ---
        * tv0 * v0 + (1 - tv0) * v1 = tw0 * w0 + tw1 * w1 + (1 - tw0 - tw1) * w2
        * ---
        * (v0 - v1) * tv0 + (w2 - w0) * tw0 + (w2 - w1) * tw1 = -v1 + w2
        * ---
        *                                 | tv0 |
        * | v0 - v1, w2 - w0, w2 - w1 | * | tw0 | = -v1 + w2
        *                                 | tw1 |
        */
        Matrix<Scalar, 3, 3> A;
        Vector<Scalar, 3> b = -v1 + w2, x;
        A.setCols(v0 - v1, w2 - w0, w2 - w1);
        if (!A.solve(b, x)) return false;
        baryCoordV[0] = x[0];
        baryCoordV[1] = 1 - baryCoordV[0];
        baryCoordW[0] = x[1];
        baryCoordW[1] = x[2];
        baryCoordW[2] = 1 - baryCoordW[0] - baryCoordW[1];
        return true;
    }
    template <typename Scalar>
    static bool calcIntersectionLineLine(
        const Vector<Scalar, 2>& v0, const Vector<Scalar, 2>& v1,
        const Vector<Scalar, 2>& w0, const Vector<Scalar, 2>& w1,
        Vector<Scalar, 2>& baryCoordV, Vector<Scalar, 2>& baryCoordW)
    {
        /*
        * tv0 * v0 + tv1 * v1 = tw0 * w0 + tw1 * w1
        * tv0 + tv1 = 1
        * tw0 + tw1 = 1
        * ---
        * tv0 * v0 + (1 - tv0) * v1 = tw0 * w0 + (1 - tw0) * w1
        * ---
        * (v0 - v1) * tv0 + (w1 - w0) * tw0 = -v1 + w1
        * ---
        *                     | tv0 |
        * | v0-v1,  w1-w0 | * | tw0 | = -v1 + w1
        * 
        * 
        */
        Matrix<Scalar, 2, 2> A;
        A.setCol(0, v0 - v1);
        A.setCol(1, w1 - w0);
        Vector<Scalar, 2> b = -v1 + w1;
        Vector<Scalar, 2> x;
        A.solve(b, x);
        baryCoordV[0] = x[0];
        baryCoordV[1] = 1 - baryCoordV[0];
        baryCoordW[0] = x[1];
        baryCoordW[1] = 1 - baryCoordW[0];
        return true;
    }
    template <typename Scalar, int N>
    static bool calcIntersectionLineLine(
        const Vector<Scalar, N>& v0, const Vector<Scalar, N>& v1,
        const Vector<Scalar, N>& w0, const Vector<Scalar, N>& w1,
        Vector<Scalar, 2>& baryCoordV, Vector<Scalar, 2>& baryCoordW)
    {
        /*
        * tv0 * v0 + tv1 * v1 = tw0 * w0 + tw1 * w1
        * tv0 + tv1 = 1
        * tw0 + tw1 = 1
        * ---
        * tv0 * v0 + (1 - tv0) * v1 = tw0 * w0 + (1 - tw0) * w1
        * ---
        * (v0 - v1) * tv0 + (w1 - w0) * tw0 = -v1 + w1
        * ---
        *                     | tv0 |
        * | v0-v1,  w1-w0 | * | tw0 | = -v1 + w1
        * 
        * 
        */
        Matrix<Scalar, N, 2> A;
        A.setCol(0, v0 - v1);
        A.setCol(1, w1 - w0);
        Vector<Scalar, N> b = -v1 + w1;
        Vector<Scalar, 2> x;
        A.solve(b, x);
        baryCoordV[0] = x[0];
        baryCoordV[1] = 1 - baryCoordV[0];
        baryCoordW[0] = x[1];
        baryCoordW[1] = 1 - baryCoordW[0];
        return true;
    }
    template <typename Scalar, int NDim, int NSimplex>
    static Vector<Scalar, NDim> calcProjection(const Vector<Scalar, NDim> v[NSimplex], const Vector<Scalar, NDim>& w) {
        assert(NSimplex <= NDim);
        Vector<Scalar, NSimplex> baryCoords = calcBarycentricCoord<Scalar, NDim, NSimplex>(v, w, 1);
        Vector<Scalar, NDim> result;
        for (int i = 0; i < NSimplex; ++i) result += baryCoords[i] * v[i];
        return result;
    }
    template <typename Scalar, int NDim>
    static Vector<Scalar, NDim + 1> calcBarycentricCoord(const Vector<Scalar, NDim> v[NDim + 1], const Vector<Scalar, NDim>& w, Scalar coord_sum = 1) {
        return calcBarycentricCoord<Scalar, NDim, NDim + 1>(v, w, coord_sum);
    }
    template <typename Scalar, int NDim, int NSimplex>
    static Vector<Scalar, NSimplex> calcBarycentricCoord(const Vector<Scalar, NDim> v[NSimplex], const Vector<Scalar, NDim>& w, Scalar coord_sum = 1) {
        assert(NSimplex <= NDim + 1);
        /* 
        [input]
          v[NSimplex]: simplex vertices
          w: target vector
        [output]
          d[NSimplex]: barycentric coordinate for each simplex vertex
        [equation]----------------------------------------------
        d0 * v0 + ... + dNSimplex * vNSimplex =~ w           (desired target)
        d0      + ... + dNSimplex             = coord_sum       (hard constraint)
        [solution via Lagrangian multiplier]--------------------------
        AtA(ij) = dot(v[i], v[j])
        Atb(i) = dot(v[i], w)
        c = [1 ... 1]
        | AtA ct | = | Atb        |
        | c    0 |   |  coord_sum |
        */
        Matrix<Scalar, NSimplex + 1, NSimplex + 1> A(0.0);
        Vector<Scalar, NSimplex + 1> b, x;
        for (int i = 0; i < NSimplex; ++i) {
            for (int j = i + 1; j < NSimplex; ++j) A(i, j) = A(j, i) = v[i] | v[j];
            A(i, i) = v[i].lengthSquared();
            A(i, NSimplex) = A(NSimplex, i) = 1;
            b[i] = v[i] | w;
        }
        b[NSimplex] = coord_sum;
        A.solve(b, x);
        Vector<Scalar, NSimplex> result;
        memcpy(result.ptr(), x.ptr(), sizeof(Scalar) * NSimplex);
        return result;
    }
};  // class Util

}   // namespace KLIB
