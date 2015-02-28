// Original source code by Kenshi Takayama

// Fixed-size Vector
#pragma once
#include <cstdarg>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>

namespace Y {

template <typename Scalar, int N>
class Vector {
public:
    typedef Scalar value_type;
    typedef Vector<Scalar, N> vector_type;
    value_type values_[N];
    
    static const size_t size_ = N;  // OpenMesh compatible
    
    // constructors ===============
    Vector() { fill(0); }
    Vector(const vector_type& src) { operator=(src); }
    //template<typename OtherScalar>
    //explicit Vector(const Vector<OtherScalar, N>& src) { operator=(src); }        // this constructor may lead to bugs...
    explicit Vector(value_type s) { fill(s); }
    explicit Vector(const value_type v[N]) { memcpy(values_, v, N * sizeof(value_type)); }
    template <int NHigh>
    explicit Vector(const Vector<Scalar, NHigh>& vecHigh) {
        assert(N < NHigh);
        memcpy(values_, vecHigh.ptr(), N * sizeof(value_type));
    }
    ~Vector() {}
    
    // dimension-wise constructors =================
    Vector(value_type s0, value_type s1) { set(s0, s1); }
    Vector(value_type s0, value_type s1, value_type s2) { set(s0, s1, s2); }
    Vector(value_type s0, value_type s1, value_type s2, value_type s3) { set(s0, s1, s2, s3); }
    Vector(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4) {set(s0, s1, s2, s3, s4); }
    Vector(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5) { set(s0, s1, s2, s3, s4, s5); }
    Vector(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5, value_type s6) { set(s0, s1, s2, s3, s4, s5, s6); }
    Vector(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5, value_type s6, value_type s7) { set(s0, s1, s2, s3, s4, s5, s6, s7); }
    //Vector(value_type s_head, const Vector<Scalar, N - 1>& v_tail) { set(s_head, v_tail); }
    template <int M>
    Vector(const Vector<Scalar, M>& v_head, const Vector<Scalar, N - M>& v_tail) { set<M>(v_head, v_tail); }
    
    // dimension-wise setters =================
    void set(value_type s0, value_type s1) {
        assert(N == 2);
        values_[0] = s0;
        values_[1] = s1;
    }
    void set(value_type s0, value_type s1, value_type s2) {
        assert(N == 3);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
    }
    void set(value_type s0, value_type s1, value_type s2, value_type s3) {
        assert(N == 4);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
        values_[3] = s3;
    }
    void set(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4) {
        assert(N == 5);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
        values_[3] = s3;
        values_[4] = s4;
    }
    void set(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5) {
        assert(N == 6);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
        values_[3] = s3;
        values_[4] = s4;
        values_[5] = s5;
    }
    void set(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5, value_type s6) {
        assert(N == 7);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
        values_[3] = s3;
        values_[4] = s4;
        values_[5] = s5;
        values_[6] = s6;
    }
    void set(value_type s0, value_type s1, value_type s2, value_type s3, value_type s4, value_type s5, value_type s6, value_type s7) {
        assert(N == 8);
        values_[0] = s0;
        values_[1] = s1;
        values_[2] = s2;
        values_[3] = s3;
        values_[4] = s4;
        values_[5] = s5;
        values_[6] = s6;
        values_[7] = s7;
    }
    //void set(value_type s_head, const Vector<Scalar, N - 1>& v_tail) {
    //    values_[0] = s_head;
    //    for (size_t i = 1; i < N; ++i)
    //        values_[i] = v_tail[i - 1];
    //}
    //void set(const Vector<Scalar, N - 1>& v_head, value_type s_tail) {
    //    for (size_t i = 0; i < N - 1; ++i)
    //        values_[i] = v_head[i];
    //    values_[N - 1] = s_tail;
    //}
    template <int M>
    void set(const Vector<Scalar, M>& v_head, const Vector<Scalar, N - M>& v_tail) {
        assert (M < N);
        for (size_t i = 0; i < M; ++i)
            values_[i] = v_head[i];
        for (size_t i = M; i < N; ++i)
            values_[i] = v_tail[i - M];
    }
    
    void set_va(void* argDummy = 0, ...) {
        va_list argptr;
        va_start(argptr, argDummy);
        for (size_t i = 0; i < N; ++i)
            values_[i] = va_arg(argptr, value_type);
        va_end (argptr);
    }
    
    // substitution operators ===================
    vector_type& operator=(const vector_type& src) {
        if (this != &src) memcpy(values_, src.values_, N * sizeof(value_type));
        return *this;
    }
    
    // type conversion ==========================
    template<typename OtherScalar>
    Vector<OtherScalar, N> convert() const {
        Vector<OtherScalar, N> result;
        for (size_t i = 0; i < N; ++i) result[i] = static_cast<OtherScalar>(values_[i]);
        return result;
    }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const {
        for (size_t i = 0; i < N; ++i) {
            Scalar d = values_[i] - rhs.values_[i];
            if (d <= -epsilon || epsilon <= d) return false;
        }
        return true;
    }
    // dictionary-order comparators
    bool operator<(const vector_type& rhs) const {
        for (size_t i = 0; i < N; ++i)
            if (values_[i] < rhs.values_[i])
                return true;
            else if (values_[i] > rhs.values_[i])
                return false;
        return false;
    }
    bool operator<=(const vector_type& rhs) const { return *this == rhs || *this < rhs; }
    bool operator>(const vector_type& rhs) const { return rhs < *this; }
    bool operator>=(const vector_type& rhs) const { return rhs <= *this; }
    
    
    // fill ====================
    void fill(const value_type& s) { std::fill(values_, values_ + N, s); }
    void vectorize(const value_type& s) { fill(s); }    // for OpenMesh compatibility
    
    // cast to raw pointer ====================
    value_type* ptr() { return values_; }
    const value_type* ptr() const { return values_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    
    // arithmetic (member) =====================
    // with scalar
    vector_type& operator+=(value_type rhs) { for (size_t i = 0; i < N; ++i) values_[i] += rhs; return *this; }
    vector_type& operator-=(value_type rhs) { for (size_t i = 0; i < N; ++i) values_[i] -= rhs; return *this; }
    vector_type& operator*=(value_type rhs) { for (size_t i = 0; i < N; ++i) values_[i] *= rhs; return *this; }
    vector_type& operator/=(value_type rhs) { for (size_t i = 0; i < N; ++i) values_[i] /= rhs; return *this; }
    vector_type operator+(value_type rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(value_type rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(value_type rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(value_type rhs) const { return vector_type(*this) /= rhs; }
    // with vector
    vector_type& operator+=(const vector_type& rhs) { for (size_t i = 0; i < N; ++i) values_[i] += rhs.values_[i]; return *this; }
    vector_type& operator-=(const vector_type& rhs) { for (size_t i = 0; i < N; ++i) values_[i] -= rhs.values_[i]; return *this; }
    vector_type& operator*=(const vector_type& rhs) { for (size_t i = 0; i < N; ++i) values_[i] *= rhs.values_[i]; return *this; }
    vector_type& operator/=(const vector_type& rhs) { for (size_t i = 0; i < N; ++i) values_[i] /= rhs.values_[i]; return *this; }
    vector_type operator+(const vector_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const vector_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const vector_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const vector_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const {
        vector_type result;
        for (size_t i = 0; i < N; ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // vector operation =========================
    // dot product
    value_type operator|(const vector_type& rhs) const {
        value_type result = 0;
        for (size_t i = 0; i < N; ++i) result += values_[i] * rhs.values_[i];
        return result;
    }
    value_type dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    // normalize
    value_type norm() const { return static_cast<value_type>(sqrt(sqnorm())); }
    value_type sqnorm() const { return (*this) | (*this); }
    value_type length() const { return norm(); }
    value_type lengthSquared() const { return sqnorm(); }
    vector_type normalized() const { return (*this) / norm(); }
    void normalize()  { (*this) /= norm(); }
    void normalize_cond() {
        value_type r = norm();
        (*this) /= (r == static_cast<value_type>(0)) ? 1 : r;
    }

    // max / min / sum / mean ==========================
    value_type elemMin() const {
        Scalar result = (std::numeric_limits<Scalar>::max());
        for (size_t i = 0; i < N; ++i) result = values_[i] < result ? values_[i] : result;
        return result;
    }
    value_type elemMax() const {
        Scalar result = -(std::numeric_limits<Scalar>::min());
        for (size_t i = 0; i < N; ++i) result = result < values_[i] ? values_[i] : result;
        return result;
    }
    value_type elemSum() const {
        Scalar result = 0;
        for (size_t i = 0; i < N; ++i) result += values_[i];
        return result;
    }
    value_type elemMean() const { return elemSum() / N; }
    vector_type pairwiseMax(const vector_type& rhs) const {
        vector_type result;
        for (size_t i = 0; i < N; ++i) result.values_[i] = values_[i] < rhs.values_[i] ? rhs.values_[i] : values_[i];
        return result;
    }
    vector_type pairwiseMin(const vector_type& rhs) const {
        vector_type result;
        for (size_t i = 0; i < N; ++i) result.values_[i] = values_[i] > rhs.values_[i] ? rhs.values_[i] : values_[i];
        return result;
    }
    
    // static maker ==========================
    static vector_type make(void* argDummy = 0, ...) {
        va_list argptr;
        va_start(argptr, argDummy);
        vector_type result;
        for (size_t i = 0; i < N; ++i)
            result[i] = va_arg(argptr, value_type);
        va_end (argptr);
        return result;
    }

    void set_random(value_type range_min, value_type range_max) {
        value_type range_size = range_max - range_min;
        for (size_t i = 0; i < N; ++i) {
            value_type val = static_cast<value_type>(rand() * range_size / RAND_MAX);
            values_[i] = range_min + val;
        }
    }
    
    // append ====================
    template <int M>
    Vector<Scalar, N + M> append(const Vector<Scalar, M>& tail) const {
        Vector<Scalar, N + M> result;
        for (size_t i = 0; i < N; ++i)
            result[i] = values_[i];
        for (size_t i = 0; i < M; ++i)
            result[N + i] = tail[i];
        return result;
    }
};

// arithmetic (global) ====================
template <typename Scalar, int N> Vector<Scalar, N> operator+(const Scalar& lhs, const Vector<Scalar, N>& rhs) { return rhs + lhs; }
template <typename Scalar, int N> Vector<Scalar, N> operator-(const Scalar& lhs, const Vector<Scalar, N>& rhs) { return rhs - lhs; }
template <typename Scalar, int N> Vector<Scalar, N> operator*(const Scalar& lhs, const Vector<Scalar, N>& rhs) { return rhs * lhs; }
template <typename Scalar, int N> Vector<Scalar, N> operator/(const Scalar& lhs, const Vector<Scalar, N>& rhs) { return rhs / lhs; }

// cross product==========================
template <typename Scalar>
Vector<Scalar, 3> operator%(const Vector<Scalar, 3>& lhs, const Vector<Scalar, 3>& rhs) {
    return Vector<Scalar, 3>(
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}
template <typename Scalar>
Vector<Scalar, 3> cross_product(const Vector<Scalar, 3>& lhs, const Vector<Scalar, 3>& rhs) { return lhs % rhs; }

// 90-dgree rotation of 2-dimensional vector ===============
template <typename Scalar>
Vector<Scalar, 2> rotate90(const Vector<Scalar, 2>& src) {
    return Vector<Scalar, 2>(-src[1], src[0]);
}

// abs =================================================
template <typename Scalar, int N>
Vector<Scalar, N> abs(const Vector<Scalar, N>& arg) {
    Vector<Scalar, N> result(arg);
    for (size_t i = 0; i < N; ++i) if (result[i] < 0) result[i] = -result[i];
    return result;
}

// stream I/O ===========================
template <typename Scalar, int N>
std::ostream& operator<<(std::ostream& os, const Vector<Scalar, N>& rhs) {
    for (size_t i = 0; i < N - 1; ++i) os << rhs[i] << '\t';
    os << rhs[N - 1];
    return os;
}
template <typename Scalar, int N>
std::istream& operator>>(std::istream& is, Vector<Scalar, N>& rhs) {
    for (size_t i = 0; i < N; ++i) is >> rhs[i];
    return is;
}

// interfaces for OpenMesh compatibility ====================
template <typename Scalar>
Vector<Scalar, 3> cross(const Vector<Scalar, 3>& lhs, const Vector<Scalar, 3>& rhs) { return lhs % rhs; }
template <typename Scalar, int N>
Vector<Scalar, N> vectorized(const Scalar& arg) { return Vector<Scalar, N>(arg); }

#include "Vector1.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

typedef Vector<signed   char, 1> Vector1c;
typedef Vector<unsigned char, 1> Vector1uc;
typedef Vector<signed   int , 1> Vector1i;
typedef Vector<unsigned int , 1> Vector1ui;
typedef Vector<signed   long, 1> Vector1l;
typedef Vector<unsigned long, 1> Vector1ul;
typedef Vector<float        , 1> Vector1f;
typedef Vector<double       , 1> Vector1d;

typedef Vector<signed   char, 2> Vector2c;
typedef Vector<unsigned char, 2> Vector2uc;
typedef Vector<signed   int , 2> Vector2i;
typedef Vector<unsigned int , 2> Vector2ui;
typedef Vector<signed   long, 2> Vector2l;
typedef Vector<unsigned long, 2> Vector2ul;
typedef Vector<float        , 2> Vector2f;
typedef Vector<double       , 2> Vector2d;

typedef Vector<signed   char, 3> Vector3c;
typedef Vector<unsigned char, 3> Vector3uc;
typedef Vector<signed   int , 3> Vector3i;
typedef Vector<unsigned int , 3> Vector3ui;
typedef Vector<signed   long, 3> Vector3l;
typedef Vector<unsigned long, 3> Vector3ul;
typedef Vector<float        , 3> Vector3f;
typedef Vector<double       , 3> Vector3d;

typedef Vector<signed   char, 4> Vector4c;
typedef Vector<unsigned char, 4> Vector4uc;
typedef Vector<signed   int , 4> Vector4i;
typedef Vector<unsigned int , 4> Vector4ui;
typedef Vector<signed   long, 4> Vector4l;
typedef Vector<unsigned long, 4> Vector4ul;
typedef Vector<float        , 4> Vector4f;
typedef Vector<double       , 4> Vector4d;

typedef Vector<signed   char, 5> Vector5c;
typedef Vector<unsigned char, 5> Vector5uc;
typedef Vector<signed   int , 5> Vector5i;
typedef Vector<unsigned int , 5> Vector5ui;
typedef Vector<signed   long, 5> Vector5l;
typedef Vector<unsigned long, 5> Vector5ul;
typedef Vector<float        , 5> Vector5f;
typedef Vector<double       , 5> Vector5d;

typedef Vector<signed   char, 6> Vector6c;
typedef Vector<unsigned char, 6> Vector6uc;
typedef Vector<signed   int , 6> Vector6i;
typedef Vector<unsigned int , 6> Vector6ui;
typedef Vector<signed   long, 6> Vector6l;
typedef Vector<unsigned long, 6> Vector6ul;
typedef Vector<float        , 6> Vector6f;
typedef Vector<double       , 6> Vector6d;

typedef Vector<signed   char, 7> Vector7c;
typedef Vector<unsigned char, 7> Vector7uc;
typedef Vector<signed   int , 7> Vector7i;
typedef Vector<unsigned int , 7> Vector7ui;
typedef Vector<signed   long, 7> Vector7l;
typedef Vector<unsigned long, 7> Vector7ul;
typedef Vector<float        , 7> Vector7f;
typedef Vector<double       , 7> Vector7d;

typedef Vector<signed   char, 8> Vector8c;
typedef Vector<unsigned char, 8> Vector8uc;
typedef Vector<signed   int , 8> Vector8i;
typedef Vector<unsigned int , 8> Vector8ui;
typedef Vector<signed   long, 8> Vector8l;
typedef Vector<unsigned long, 8> Vector8ul;
typedef Vector<float        , 8> Vector8f;
typedef Vector<double       , 8> Vector8d;

}
