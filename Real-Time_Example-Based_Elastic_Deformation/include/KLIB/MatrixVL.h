// Variable Length Matrix
#pragma once
#pragma warning(push)
#pragma warning(disable: 4018)

#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "VectorVL.h"


namespace KLIB {

template <typename Scalar>
class MatrixVL {
public:
    typedef Scalar value_type;
    typedef MatrixVL<Scalar> matrix_type;
    typedef VectorVL<Scalar> vector_type;
private:
    std::vector<value_type> values_;
    size_t sizeRow_;
    size_t sizeCol_;
public:
    // constructors ===============
    MatrixVL() {}
    MatrixVL(size_t sizeRow, size_t sizeCol) : values_(sizeRow * sizeCol), sizeRow_(sizeRow), sizeCol_(sizeCol) {}
    MatrixVL(size_t sizeRow, size_t sizeCol, const value_type& s) : values_(sizeRow * sizeCol, s) {}
    MatrixVL(const matrix_type& src) : values_(src.values_), sizeRow_(src.sizeRow_), sizeCol_(src.sizeCol_) {}
    
    // casting
    template<typename OtherScalar>
    explicit MatrixVL(const MatrixVL<OtherScalar>& src) { operator=(src); }
    MatrixVL(size_t sizeRow, size_t sizeCol, const std::vector<value_type>& vec) : values_(vec), sizeRow_(sizeRow), sizeCol_(sizeCol) {
        if (vec.size() != sizeRow * sizeCol) throw "Incompatible size.";
    }
    MatrixVL(size_t sizeRow, size_t sizeCol, const value_type* ptr) : values_(sizeRow * sizeCol), sizeRow_(sizeRow), sizeCol_(sizeCol) {
        memcpy(&values_[0], ptr, sizeRow * sizeCol * sizeof(value_type));
    }
    ~MatrixVL() {}
    
    // size
    size_t sizeRow() const { return sizeRow_; }
    size_t sizeCol() const { return sizeCol_; }
    void resize(size_t sizeRow, size_t sizeCol) {
        sizeRow_ = sizeRow;
        sizeCol_ = sizeCol;
        values_.resize(sizeRow * sizeCol);
    }
    void resize(size_t sizeRow, size_t sizeCol, const value_type& s) {
        sizeRow_ = sizeRow;
        sizeCol_ = sizeCol;
        values_.resize(sizeRow * sizeCol, s);
    }
    void clear() {
        values_.clear();
        sizeRow_ = sizeCol_ = 0;
    }

    // set
    void set(size_t sizeRow, size_t sizeCol, const std::vector<value_type>& vec) {
        if (vec.size() != sizeRow * sizeCol) throw "Incompatible size.";
        sizeRow_ = sizeRow;
        sizeCol_ = sizeCol;
        values_ = vec;
    }
    void set(size_t sizeRow, size_t sizeCol, const value_type* ptr) {
        sizeRow_ = sizeRow;
        sizeCol_ = sizeCol;
        values_.resize(sizeRow * sizeCol);
        memcpy(&values_[0], ptr, sizeRow * sizeCol * sizeof(value_type));
    }
    void set_va(size_t sizeRow, size_t sizeCol, ...) {
        sizeRow_ = sizeRow;
        sizeCol_ = sizeCol;
        values_.clear();
        values_.reserve(sizeRow * sizeCol);
        va_list argptr;
        va_start(argptr, sizeCol);
        for (size_t i = 0; i < sizeRow * sizeCol; ++i) values_.push_back(va_arg(argptr, value_type));
        va_end(argptr);
    }
    
    // substitution operators ===================
    matrix_type& operator=(const matrix_type& src) {
        values_ = src.values_;
        sizeRow_ = src.sizeRow_;
        sizeCol_ = src.sizeCol_;
        return *this;
    }
    template<typename OtherScalar>
    matrix_type& operator=(const VectorVL<OtherScalar>& src) {
        values_.clear();
        values_.reserve(src.size());
        sizeRow_ = src.sizeRow_;
        sizeCol_ = src.sizeCol_;
        for (size_t i = 0; i < sizeRow_ * sizeCol_; ++i) values_.push_back(static_cast<value_type>(src[i]));
        return *this;
    }
    
    // comparison ===============================
    bool operator==(const matrix_type& rhs) const { return equals(rhs); }
    bool operator!=(const matrix_type& rhs) const { return !equals(rhs); }
    bool equals(const matrix_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const {
        if (sizeRow_ != rhs.sizeRow_ || sizeCol_ != rhs.sizeCol_) throw "Incompatible size.";
        for (size_t i = 0; i < values_.size(); ++i) {
            Scalar d = values_[i] - rhs.values_[i];
            if (d <= -epsilon || epsilon <= d) return false;
        }
        return true;
    }
    
    // fill ====================
    void fill(size_t sizeRow, size_t sizeCol, const value_type& s) {
        clear();
        resize(sizeRow, sizeCol, s);
    }
    
    // cast to raw pointer ====================
    value_type* ptr() { return &values_[0]; }
    const value_type* ptr() const { return &values_[0]; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    value_type& operator()(size_t row, size_t col) { return values_[sizeCol_ * row + col]; }
    const value_type& operator()(size_t row, size_t col) const { return values_[sizeCol_ * row + col]; }

    // arithmetic (member) =====================
    // with scalar
    matrix_type& operator+=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] += rhs; return *this; }
    matrix_type& operator-=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] -= rhs; return *this; }
    matrix_type& operator*=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] *= rhs; return *this; }
    matrix_type& operator/=(const value_type& rhs) { for (size_t i = 0; i < values_.size(); ++i) values_[i] /= rhs; return *this; }
    matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
    matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
    matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
    // with same sized matrix
    matrix_type& operator+=(const matrix_type& rhs) {
        if (sizeRow_ != rhs.sizeRow_ || sizeCol_ != rhs.sizeCol_) throw "Incompatible size.";
        for (size_t i = 0; i < values_.size(); ++i) values_[i] += rhs.values_[i];
        return *this;
    }
    matrix_type& operator-=(const matrix_type& rhs) {
        if (sizeRow_ != rhs.sizeRow_ || sizeCol_ != rhs.sizeCol_) throw "Incompatible size.";
        for (size_t i = 0; i < values_.size(); ++i) values_[i] -= rhs.values_[i];
        return *this;
    }
    matrix_type operator+(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
    // multiplication with vector & matrix
    vector_type operator*(const vector_type& rhs) const {
        if (sizeCol_ != rhs.size()) throw "Incompatible size.";
        vector_type result(sizeRow_);
        for (size_t row = 0; row < sizeRow_; ++row) for (size_t col = 0; col < sizeCol_; ++col) result[row] += (*this)(row, col) * rhs[col];
        return result;
    }
    matrix_type operator*(const matrix_type& rhs) const {
        if (sizeCol_ != rhs.sizeRow_) throw "Incompatible size.";
        matrix_type result(sizeRow_, rhs.sizeCol_);
        for (size_t row = 0; row < sizeRow_; ++row) for (size_t col = 0; col < rhs.sizeCol_; ++col) for (size_t i = 0; i < sizeCol_; ++i) {
            result(row, col) += (*this)(row, i) * rhs(i, col);
        }
        return result;
    }
    // negate
    matrix_type operator-() const {
        matrix_type result(sizeRow_, sizeCol_);
        for (size_t i = 0; i < values_.size(); ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // matrix operations ==================
    // transpose
    matrix_type transpose() const {
        matrix_type result(sizeCol_, sizeRow_);
        for (size_t row = 0; row < sizeRow_; ++row) for (size_t col = 0; col < sizeCol_; ++col) result(col, row) = (*this)(row, col);
        return result;
    }
    value_type determinant() const;
    matrix_type invert() const {
        // http://www.page.sannet.ne.jp/d_takahashi/boost/ublas/index.html
        
        assert(sizeCol_ == sizeRow_);
        unsigned int n = sizeCol_;
        
        typedef boost::numeric::ublas::vector<double> dvector;
        typedef boost::numeric::ublas::matrix<double> dmatrix;
        
        dmatrix B = boost::numeric::ublas::identity_matrix<double>(n);

        dmatrix A(n, n);
        for (size_t row = 0; row < n; ++ row) {
            for (size_t col = 0; col < n; ++ col) {
                A(row, col) = (*this)(row, col);
            }
        }
        
        boost::numeric::ublas::permutation_matrix<> pm(A.size1());

        lu_factorize(A, pm);
        lu_substitute(A, pm, B);

        matrix_type result(n, n);
        for (size_t row = 0; row < n; ++ row) {
            for (size_t col = 0; col < n; ++ col) {
                result(row, col) = B(row, col);
            }
        }
        return result;
    };
    vector_type solve(const vector_type& b) const;
};

// arithmetic (global) ====================
template <typename Scalar> MatrixVL<Scalar> operator+(const Scalar& lhs, const MatrixVL<Scalar>& rhs) { return rhs + lhs; }
template <typename Scalar> MatrixVL<Scalar> operator-(const Scalar& lhs, const MatrixVL<Scalar>& rhs) { return rhs - lhs; }
template <typename Scalar> MatrixVL<Scalar> operator*(const Scalar& lhs, const MatrixVL<Scalar>& rhs) { return rhs * lhs; }
template <typename Scalar> MatrixVL<Scalar> operator/(const Scalar& lhs, const MatrixVL<Scalar>& rhs) { return rhs / lhs; }

// mult (vector x matrix)
template <typename Scalar>
VectorVL<Scalar> operator*(const VectorVL<Scalar>& lhs, const MatrixVL<Scalar>& rhs) {
    if (lhs.size() != rhs.sizeRow()) throw "Incompatible size.";
    VectorVL<Scalar> result(rhs.sizeCol(), 0);
    for (size_t col = 0; col < rhs.sizeCol(); ++col) for (size_t row = 0; row < rhs.sizeRow(); ++row) result[col] += lhs[row] * rhs(row, col);
    return result;
}

// matrix maker ==========================
template <typename Scalar>
MatrixVL<Scalar> makeMatrixVL(size_t sizeRow, size_t sizeCol, ...) {
    MatrixVL<Scalar> result(sizeRow, sizeCol);
    va_list argptr;
    va_start(argptr, sizeCol);
    for (size_t i = 0; i < sizeRow * sizeCol; ++i) result[i] = va_arg(argptr, Scalar);
    va_end(argptr);
    return result;
}

// input / output
template <typename Scalar>
std::ostream& operator<<(std::ostream& os, const MatrixVL<Scalar>& rhs) {
    for (size_t row = 0; row < rhs.sizeRow(); ++row) {
        for (size_t col = 0; col < rhs.sizeCol() - 1; ++col) os << rhs(row, col) << '\t';
        os << rhs(row, rhs.sizeCol() - 1) << std::endl;
    }
    return os;
}

// typedefs =========================================
typedef MatrixVL<signed   char> MatrixVLc;
typedef MatrixVL<unsigned char> MatrixVLuc;
typedef MatrixVL<signed   int > MatrixVLi;
typedef MatrixVL<unsigned int > MatrixVLui;
typedef MatrixVL<signed   long> MatrixVLl;
typedef MatrixVL<unsigned long> MatrixVLul;
typedef MatrixVL<float        > MatrixVLf;
typedef MatrixVL<double       > MatrixVLd;

}   // namespace KLIB

#pragma warning(pop)
