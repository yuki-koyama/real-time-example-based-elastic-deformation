// Original source code by Kenshi Takayama

#pragma once
#include "Vector.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstring>

namespace Y
{

    template <typename Scalar, int NRow, int NCol> class Matrix
    {
    public:
        typedef Scalar                     value_type;
        typedef Matrix<Scalar, NRow, NCol> matrix_type;
        value_type                         values_[NRow * NCol];

        static const size_t size_ = NRow * NCol; // OpenMesh compatible

        // constructors ===============
        Matrix() { fill(0); }
        Matrix(const matrix_type& src) { operator=(src); }
        // template<typename OtherScalar>
        // explicit Matrix(const Matrix<OtherScalar, NRow, NCol>& src) { operator=(src); }        // this constructor
        // may lead to bugs...
        explicit Matrix(const value_type& s) { fill(s); }
        explicit Matrix(const value_type v[NRow * NCol]) { memcpy(values_, v, NRow * NCol * sizeof(value_type)); }
        template <int N> explicit Matrix(const Vector<Scalar, N>& diagonal) { setDiagonal(diagonal); }
        explicit Matrix(const Vector<Scalar, NCol>& vecRow, const Vector<Scalar, NRow>& vecCol)
        {
            setTensorProd(vecRow, vecCol);
        }
        ~Matrix() {}

        void set(void* argDummy = 0, ...)
        {
            va_list argptr;
            va_start(argptr, argDummy);
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] = va_arg(argptr, value_type);
            va_end(argptr);
        }
        template <int N> void setDiagonal(const Vector<Scalar, N>& diagonal)
        {
            assert((N == NRow && NRow <= NCol) || (N == NCol && NCol <= NRow));
            fill(0);
            for (size_t i = 0; i < N; ++i)
                (*this)(i, i) = diagonal[i];
        }
        void setTensorProd(const Vector<Scalar, NCol>& vecRow, const Vector<Scalar, NRow>& vecCol)
        {
            for (size_t i = 0; i < NRow; ++i)
                for (size_t j = 0; j < NCol; ++j)
                    values_[NCol * i + j] = vecRow[j] * vecCol[i];
        }
        void setTensorProd(const Vector<Scalar, NRow>& vec)
        {
            assert(NRow == NCol);
            const int N = NRow;
            for (size_t i = 0; i < N; ++i)
            {
                Scalar vec_i       = vec[i];
                values_[N * i + i] = vec_i * vec_i;
                for (size_t j = i + 1; j < N; ++j)
                    values_[N * i + j] = values_[N * j + i] = vec_i * vec[j];
            }
        }
        void setIdentity()
        {
            if (NRow < NCol)
            {
                setDiagonal(Vector<Scalar, NRow>(1.));
            }
            else
            {
                setDiagonal(Vector<Scalar, NCol>(1.));
            }
        }
        void setRow(size_t rowIndex, const Vector<Scalar, NCol>& rowValue)
        {
            memcpy(&values_[NCol * rowIndex], rowValue.ptr(), NCol * sizeof(value_type));
        }
        void setCol(size_t colIndex, const Vector<Scalar, NRow>& colValue)
        {
            for (size_t row = 0; row < NRow; ++row)
                (*this)(row, colIndex) = colValue[row];
        }
        Vector<Scalar, NCol> getRow(size_t rowIndex) const
        {
            Vector<Scalar, NCol> result;
            memcpy(result.ptr(), &values_[NCol * rowIndex], NCol * sizeof(value_type));
            return result;
        }
        Vector<Scalar, NRow> getCol(size_t colIndex) const
        {
            Vector<Scalar, NRow> result;
            for (size_t row = 0; row < NRow; ++row)
                result[row] = (*this)(row, colIndex);
            return result;
        }

        // substitution operators ===================
        matrix_type& operator=(const matrix_type& src)
        {
            if (this != &src) memcpy(values_, src.values_, NRow * NCol * sizeof(value_type));
            return *this;
        }

        // type conversion ==========================
        template <typename OtherScalar> Matrix<OtherScalar, NRow, NCol> convert() const
        {
            Matrix<OtherScalar, NRow, NCol> result;
            for (size_t i = 0; i < NRow * NCol; ++i)
                result[i] = static_cast<OtherScalar>(values_[i]);
            return result;
        }

        // fill ====================
        void fill(const value_type& s) { std::fill(values_, values_ + NRow * NCol, s); }

        // cast to raw pointer ====================
        value_type*       ptr() { return values_; }
        const value_type* ptr() const { return values_; }

        // indexing ==========================
        value_type&       operator[](size_t index) { return values_[index]; }
        const value_type& operator[](size_t index) const { return values_[index]; }
        value_type&       operator()(size_t rowIndex, size_t colIndex) { return values_[NCol * rowIndex + colIndex]; }
        const value_type& operator()(size_t rowIndex, size_t colIndex) const
        {
            return values_[NCol * rowIndex + colIndex];
        }

        // comparison ======================
        bool operator==(const matrix_type& rhs) const
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                if (values_[i] != rhs.values_[i]) return false;
            return true;
        }
        bool operator!=(const matrix_type& rhs) const { return !operator==(rhs); }

        // arithmetic (member) =====================
        // with scalar
        matrix_type& operator+=(const value_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] += rhs;
            return *this;
        }
        matrix_type& operator-=(const value_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] -= rhs;
            return *this;
        }
        matrix_type& operator*=(const value_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] *= rhs;
            return *this;
        }
        matrix_type& operator/=(const value_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] /= rhs;
            return *this;
        }
        matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
        matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
        matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
        matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
        // with same sized matrix
        matrix_type& operator+=(const matrix_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] += rhs.values_[i];
            return *this;
        }
        matrix_type& operator-=(const matrix_type& rhs)
        {
            for (size_t i = 0; i < NRow * NCol; ++i)
                values_[i] -= rhs.values_[i];
            return *this;
        }
        matrix_type operator+(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
        matrix_type operator-(const matrix_type& rhs) const { return matrix_type(*this) -= rhs; }
        // mult with vector
        Vector<Scalar, NRow> operator*(const Vector<Scalar, NCol>& rhs) const
        {
            Vector<Scalar, NRow> result;
            for (size_t row = 0; row < NRow; ++row)
                for (size_t col = 0; col < NCol; ++col)
                    result[row] += (*this)(row, col) * rhs[col];
            return result;
        }
        // mult with matrix
        template <int NColNew> Matrix<Scalar, NRow, NColNew> operator*(const Matrix<Scalar, NCol, NColNew>& rhs)
        {
            Matrix<Scalar, NRow, NColNew> result;
            for (size_t row = 0; row < NRow; ++row)
                for (size_t col = 0; col < NColNew; ++col)
                {
                    Scalar sum = 0;
                    for (size_t k = 0; k < NCol; ++k)
                        sum += (*this)(row, k) * rhs(k, col);
                    result(row, col) = sum;
                }
            return result;
        }
        // negate
        matrix_type operator-() const
        {
            matrix_type result;
            for (size_t i = 0; i < NRow * NCol; ++i)
                result.values_[i] = -values_[i];
            return result;
        }

        // matrix operations ==================
        // transpose
        Matrix<Scalar, NCol, NRow> transpose() const
        {
            Matrix<Scalar, NCol, NRow> result;
            for (size_t row = 0; row < NRow; ++row)
                for (size_t col = 0; col < NCol; ++col)
                    result(col, row) = operator()(row, col);
            return result;
        }
        // determinant / solve (not implemented) ====================
        value_type determinant() const;
        bool       solve(const Vector<Scalar, NRow>& b, Vector<Scalar, NCol>& x) const
        {
            assert(NCol < NRow);
            Matrix<Scalar, NCol, NRow> At  = this->transpose();
            Matrix<Scalar, NCol, NCol> AtA = At * (*this);
            Vector<Scalar, NCol>       Atb = At * b;
            return AtA.solve(Atb, x);
        }

        // static maker ==========================
        static matrix_type make(void* argDummy = 0, ...)
        {
            va_list argptr;
            va_start(argptr, argDummy);
            matrix_type result;
            for (size_t i = 0; i < NRow * NCol; ++i)
                result[i] = va_arg(argptr, value_type);
            va_end(argptr);
            return result;
        }
        static matrix_type identity()
        {
            matrix_type result;
            result.setIdentity();
            return result;
        }
    };

    // arithmetic (global) ====================
    template <typename Scalar, int NRow, int NCol>
    Matrix<Scalar, NRow, NCol> operator+(const Scalar& lhs, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        return rhs + lhs;
    }
    template <typename Scalar, int NRow, int NCol>
    Matrix<Scalar, NRow, NCol> operator-(const Scalar& lhs, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        return rhs - lhs;
    }
    template <typename Scalar, int NRow, int NCol>
    Matrix<Scalar, NRow, NCol> operator*(const Scalar& lhs, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        return rhs * lhs;
    }
    template <typename Scalar, int NRow, int NCol>
    Matrix<Scalar, NRow, NCol> operator/(const Scalar& lhs, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        return rhs / lhs;
    }

    // mult (vector x matrix)
    template <typename Scalar, int NRow, int NCol>
    Vector<Scalar, NCol> operator*(const Vector<Scalar, NRow>& lhs, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        Vector<Scalar, NCol> result;
        for (size_t col = 0; col < NCol; ++col)
            for (size_t row = 0; row < NRow; ++row)
                result[col] += lhs[row] * rhs(row, col);
        return result;
    }

    // col x row
    template <typename Scalar, int NRow, int NCol>
    Matrix<Scalar, NRow, NCol> mult_col_row(const Vector<Scalar, NCol>& col, const Vector<Scalar, NRow>& row)
    {
        Matrix<Scalar, NRow, NCol> result;
        for (size_t i = 0; i < NRow; ++i)
            for (size_t j = 0; j < NCol; ++j)
                result(i, j) = row[i] * col[j];
        return result;
    }

    // stream I/O ===========================
    template <typename Scalar, int NRow, int NCol>
    std::ostream& operator<<(std::ostream& os, const Matrix<Scalar, NRow, NCol>& rhs)
    {
        for (size_t i = 0; i < NRow; ++i)
        {
            for (size_t j = 0; j < NCol - 1; ++j)
                os << rhs(i, j) << '\t';
            os << rhs(i, NCol - 1) << '\n';
        }
        return os;
    }

#include "Matrix1x1.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

    // typedefs =========================================
    typedef Matrix<signed char, 2, 2>   Matrix2x2c;
    typedef Matrix<unsigned char, 2, 2> Matrix2x2uc;
    typedef Matrix<signed int, 2, 2>    Matrix2x2i;
    typedef Matrix<unsigned int, 2, 2>  Matrix2x2ui;
    typedef Matrix<signed long, 2, 2>   Matrix2x2l;
    typedef Matrix<unsigned long, 2, 2> Matrix2x2ul;
    typedef Matrix<float, 2, 2>         Matrix2x2f;
    typedef Matrix<double, 2, 2>        Matrix2x2d;

    typedef Matrix<signed char, 3, 3>   Matrix3x3c;
    typedef Matrix<unsigned char, 3, 3> Matrix3x3uc;
    typedef Matrix<signed int, 3, 3>    Matrix3x3i;
    typedef Matrix<unsigned int, 3, 3>  Matrix3x3ui;
    typedef Matrix<signed long, 3, 3>   Matrix3x3l;
    typedef Matrix<unsigned long, 3, 3> Matrix3x3ul;
    typedef Matrix<float, 3, 3>         Matrix3x3f;
    typedef Matrix<double, 3, 3>        Matrix3x3d;

    typedef Matrix<signed char, 4, 4>   Matrix4x4c;
    typedef Matrix<unsigned char, 4, 4> Matrix4x4uc;
    typedef Matrix<signed int, 4, 4>    Matrix4x4i;
    typedef Matrix<unsigned int, 4, 4>  Matrix4x4ui;
    typedef Matrix<signed long, 4, 4>   Matrix4x4l;
    typedef Matrix<unsigned long, 4, 4> Matrix4x4ul;
    typedef Matrix<float, 4, 4>         Matrix4x4f;
    typedef Matrix<double, 4, 4>        Matrix4x4d;

} // namespace Y
