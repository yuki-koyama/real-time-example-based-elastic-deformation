// Variable Length Vector
#pragma once
#pragma warning(push)
#pragma warning(disable: 4018)
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

namespace KLIB {

template <typename Scalar>
class VectorVL {
public:
    typedef Scalar value_type;
    typedef VectorVL<Scalar> vector_type;
private:
    std::vector<value_type> values_;
public:
    // constructors ===============
    VectorVL() {}
    explicit VectorVL(size_t size) : values_(size) {}
    VectorVL(size_t size, const value_type& s) : values_(size, s) {}
    VectorVL(const vector_type& src) : values_(src.values_) {}

    std::vector<value_type> std_vector () const { return values_; }
    
    // casting
    //template<typename OtherScalar>
    //explicit VectorVL(const VectorVL<OtherScalar>& src) { operator=(src); }        // this constructor may lead to bugs...
    explicit VectorVL(const std::vector<value_type>& vec) : values_(vec) {}
    VectorVL(size_t size, const value_type* ptr) : values_(size) { memcpy(&values_[0], ptr, size * sizeof(value_type)); }
    ~VectorVL() {}

    // size
    size_t size() const { return values_.size(); }
    void resize(size_t size) { values_.resize(size); }
    void resize(size_t size, const value_type& s) { values_.resize(size, s); }
    void clear() { values_.clear(); }
    
    // set
    void set(const std::vector<value_type>& vec) { values_ = vec; }
    void set(size_t size, const value_type* ptr) {
        values_.resize(size);
        memcpy(&values_[0], ptr, size * sizeof(value_type));
    }
    void set_va(size_t size, ...) {
        values_.clear();
        values_.reserve(size);
        va_list argptr;
        va_start(argptr, size);
        for (size_t i = 0; i < size; ++i) values_.push_back(va_arg(argptr, value_type));
        va_end(argptr);
    }
    
    // substitution operators ===================
    vector_type& operator=(const vector_type& src) {
        values_ = src.values_;
        return *this;
    }

    // type conversion ==========================
    template<typename OtherScalar>
    VectorVL<OtherScalar> convert() const {
        VectorVL<OtherScalar> result(values_.size());
        for (size_t i = 0; i < values_.size(); ++i) result[i] = static_cast<OtherScalar>(values_[i]);
        return result;
    }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const {
        if (values_.size() != rhs.values_.size()) throw "Incompatible size.";
        for (size_t i = 0; i < values_.size(); ++i) {
            Scalar d = values_[i] - rhs.values_[i];
            if (d <= -epsilon || epsilon <= d) return false;
        }
        return true;
    }
    
    // fill ====================
    void fill(size_t size, const value_type& s) {
        clear();
        resize(size, s);
    }
    
    // cast to raw pointer ====================
    value_type* ptr() { return &values_[0]; }
    const value_type* ptr() const { return &values_[0]; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    
    // arithmetic (member) =====================
    // with scalar
    vector_type& operator+=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] += rhs; return *this; }
    vector_type& operator-=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] -= rhs; return *this; }
    vector_type& operator*=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] *= rhs; return *this; }
    vector_type& operator/=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] /= rhs; return *this; }
    vector_type operator+(const value_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const value_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const value_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const value_type& rhs) const { return vector_type(*this) /= rhs; }
    // with vector
    vector_type& operator+=(const vector_type& rhs) { if (values_.size() != rhs.values_.size()) throw "Incompatible size."; for (size_t i = 0; i < values_.size(); ++i) values_[i] += rhs.values_[i]; return *this; }
    vector_type& operator-=(const vector_type& rhs) { if (values_.size() != rhs.values_.size()) throw "Incompatible size."; for (size_t i = 0; i < values_.size(); ++i) values_[i] -= rhs.values_[i]; return *this; }
    vector_type& operator*=(const vector_type& rhs) { if (values_.size() != rhs.values_.size()) throw "Incompatible size."; for (size_t i = 0; i < values_.size(); ++i) values_[i] *= rhs.values_[i]; return *this; }
    vector_type& operator/=(const vector_type& rhs) { if (values_.size() != rhs.values_.size()) throw "Incompatible size."; for (size_t i = 0; i < values_.size(); ++i) values_[i] /= rhs.values_[i]; return *this; }
    vector_type operator+(const vector_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const vector_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const vector_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const vector_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const {
        vector_type result(values_.size());
        for (size_t i = 0; i < values_.size(); ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // vector operation =========================
    // dot product
    value_type operator|(const vector_type& rhs) const {
        if (values_.size() != rhs.values_.size()) throw "Incompatible size.";
        value_type result = 0;
        for (size_t i = 0; i < values_.size(); ++i) result += values_[i] * rhs.values_[i];
        return result;
    }
    value_type dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    // normalize
    value_type norm() const { return static_cast<value_type>(sqrt(sqnorm())); }
    value_type sqnorm() const { return (*this) | (*this); }
    value_type length() const { return norm(); }
    value_type lengthSquared() const { return sqnorm(); }
    void normalize()  { (*this) /= norm(); }
    void normalize_cond() {
        value_type r = norm();
        (*this) /= (r == static_cast<value_type>(0)) ? 1 : r;
    }
    
    // max / min / sum / mean ==========================
    value_type elemMin() const {
        Scalar result = (std::numeric_limits<Scalar>::max());
        for (size_t i = 0; i < values_.size(); ++i) result = values_[i] < result ? values_[i] : result;
        return result;
    }
    value_type elemMax() const {
        Scalar result = -(std::numeric_limits<Scalar>::max());
        for (size_t i = 0; i < values_.size(); ++i) result = result < values_[i] ? values_[i] : result;
        return result;
    }
    value_type elemSum() const {
        Scalar result = 0;
        for (size_t i = 0; i < values_.size(); ++i) result += values_[i];
        return result;
    }
    value_type elemMean() const { return elemSum() / values_.size(); }
    vector_type pairwiseMax(const vector_type& rhs) const {
        if (values_.size() != rhs.values_.size()) throw "Incompatible size.";
        vector_type result(values_.size());
        for (size_t i = 0; i < values_.size(); ++i) result.values_[i] = values_[i] < rhs.values_[i] ? rhs.values_[i] : values_[i];
        return result;
    }
    vector_type pairwiseMin(const vector_type& rhs) const {
        if (values_.size() != rhs.values_.size()) throw "Incompatible size.";
        vector_type result(values_.size());
        for (size_t i = 0; i < values_.size(); ++i) result.values_[i] = values_[i] > rhs.values_[i] ? rhs.values_[i] : values_[i];
        return result;
    }
    
    // std::vector like functions ===============================
    value_type& front() { return values_.front(); }
    const value_type& front() const { return values_.front(); }
    value_type& back() { return values_.back(); }
    const value_type& back() const { return values_.back(); }
};

// arithmetic (global) ====================
template <typename Scalar> VectorVL<Scalar> operator+(const Scalar& lhs, const VectorVL<Scalar>& rhs) { return rhs + lhs; }
template <typename Scalar> VectorVL<Scalar> operator-(const Scalar& lhs, const VectorVL<Scalar>& rhs) { return rhs - lhs; }
template <typename Scalar> VectorVL<Scalar> operator*(const Scalar& lhs, const VectorVL<Scalar>& rhs) { return rhs * lhs; }
template <typename Scalar> VectorVL<Scalar> operator/(const Scalar& lhs, const VectorVL<Scalar>& rhs) { return rhs / lhs; }

// vector maker ==========================
template <typename Scalar>
VectorVL<Scalar> makeVectorVL(size_t size, ...) {
    VectorVL<Scalar> result(size);
    va_list argptr;
    va_start(argptr, size);
    for (size_t i = 0; i < size; ++i) result[i] = va_arg(argptr, Scalar);
    va_end(argptr);
    return result;
}

// input / output
template <typename Scalar>
std::ostream& operator<<(std::ostream& os, const VectorVL<Scalar>& rhs) {
    for (size_t i = 0; i < rhs.size() - 1; ++i) os << rhs[i] << '\t';
    os << rhs[rhs.size() - 1];
    return os;
}

// typedefs =========================================
typedef VectorVL<signed   char> VectorVLc;
typedef VectorVL<unsigned char> VectorVLuc;
typedef VectorVL<signed   int > VectorVLi;
typedef VectorVL<unsigned int > VectorVLui;
typedef VectorVL<signed   long> VectorVLl;
typedef VectorVL<unsigned long> VectorVLul;
typedef VectorVL<float        > VectorVLf;
typedef VectorVL<double       > VectorVLd;

}   // namespace KLIB

#pragma warning(pop)
