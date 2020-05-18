#pragma once

#include "Matrix.h"
#include "MatrixVL.h"
#include <vector>

extern "C" {
#include "f2c.h"
#include "clapack.h"
}

namespace KLIB {

class LapackWrapper {
    LapackWrapper(void);
    ~LapackWrapper(void);
public:
    template <int N>
    static long factorize(Matrix<double, N, N>& inout_A, Vector<long, N>& out_pivot) {
        long n = static_cast<long>(N);
        long info;
        dgetrf_(&n, &n, inout_A.ptr(), &n, out_pivot.ptr(), &info);
        return info;
    }
    static long factorize(MatrixVLd& inout_A, VectorVLl& out_pivot) {
        assert (inout_A.sizeRow() == inout_A.sizeCol());
        long n = static_cast<long>(inout_A.sizeRow());
        out_pivot.resize(n);
        long info;
        dgetrf_(&n, &n, inout_A.ptr(), &n, out_pivot.ptr(), &info);
        return info;
    }
    template <int N>
    static long solve(Matrix<double, N, N>& in_LU, Vector<long, N>& in_pivot, Vector<double, N>& inout_b) {
        char trans = 'T';
        long n = static_cast<long>(N);
        long nrhs = 1;
        long info;
        dgetrs_(&trans, &n, &nrhs, in_LU.ptr(), &n, in_pivot.ptr(), inout_b.ptr(), &n, &info);
        return info;
    }
    static long solve(MatrixVLd& in_LU, VectorVLl& in_pivot, VectorVLd& inout_b) {
        assert (in_LU.sizeRow() == in_LU.sizeCol() && in_LU.sizeRow() == in_pivot.size() && in_LU.sizeRow() == inout_b.size());
        char trans = 'T';
        long n = static_cast<long>(in_LU.sizeRow());
        long nrhs = 1;
        long info;
        dgetrs_(&trans, &n, &nrhs, in_LU.ptr(), &n, in_pivot.ptr(), inout_b.ptr(), &n, &info);
        return info;
    }
    static long svd(MatrixVLd& in_A, MatrixVLd& out_U, VectorVLd& out_Z, MatrixVLd& out_VT);
    //{
    //    if (A.sizeRow() != A.sizeCol()) throw "Tried to perform SVD on a non-square matrix.";
    //    char jobu  = 'A';
    //    char jobvt = 'A';
    //    //A: m個のすべての列ベクトルをuに格納する.
    //    //N:列ベクトルを計算しない. 
    //    //O:min(m,n)個の列ベクトルでaを上書きする.
    //    //S:min(m,n)個の列ベクトルをuに格納する.
    //    long m = static_cast<long>(A.sizeRow());
    //    long n = static_cast<long>(A.sizeCol());
    //    vector<double> A_data(m * n);
    //    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) A_data[m * j + i] = A(i, j);
    //    long lda = m;
    //    long mn_min = min(m, n);
    //    vector<double> Z_data(mn_min);
    //    vector<double> U_data(m * m);
    //    long ldu = m;
    //    vector<double> VT_data(n * n);
    //    long ldvt = n;
    //    long lwork = max(3* mn_min + max(m, n), 5 * mn_min);
    //    vector<double> work(lwork);
    //    long info;
    //    dgesvd_(&jobu, &jobvt, &m, &n, &A_data[0], &lda, &Z_data[0], &U_data[0], &ldu, &VT_data[0], &ldvt, &work[0], &lwork, &info);
    //    Z.resize(mn_min);
    //    U.resize(m, m);
    //    VT.resize(n, n);
    //    for (int i = 0; i < mn_min; ++i) Z(i) = Z_data[i];
    //    for (int i = 0; i < m; ++i) for (int j = 0; j < m; ++j) U(i, j) = U_data[m * j + i];
    //    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) VT(i, j) = VT_data[n * i + j];
    //    return info;
    //}
    template <int N>
    static long eigen(Matrix<double, N, N>& inout_A, Vector<double, N> out_eigenvectors[N], double out_eigenvalues_real[N], double out_eigenvalues_img[N]) {
        // C-style array is row-major, while Fortran-style array is column-major, so here i compute left eigenvector to compensate for this
        char jobvl = 'V';
        char jobvr = 'N';
        long n = N;
        double vl[N * N]; 
        double vr;
	    long ldvr = 1;
	    long lwork = 4 * n;
        double work[4 * N]; 
        long info;
        dgeev_(&jobvl, &jobvr, &n, inout_A.ptr(), &n, out_eigenvalues_real, out_eigenvalues_img, vl, &n, &vr, &ldvr, work, &lwork, &info);
        for (long i = 0; i < n; ++i) {
            for (long j = 0; j < n; ++j)
                out_eigenvectors[i][j] = vl[n * i + j];
        }
        return info;
    }
    static long eigen(MatrixVLd& inout_A, std::vector<VectorVLd>& out_eigenvectors, std::vector<double>& out_eigenvalues_real, std::vector<double>& out_eigenvalues_img) {
        // C-style array is row-major, while Fortran-style array is column-major, so here i compute left eigenvector to compensate for this
        char jobvl = 'V';
        char jobvr = 'N';
        assert(inout_A.sizeRow() == inout_A.sizeCol());
        long n = static_cast<long>(inout_A.sizeRow());
        out_eigenvalues_real.resize(n);
        out_eigenvalues_img.resize(n);
        std::vector<double> vl(n * n); 
        double vr;
	    long ldvr = 1;
	    long lwork = 4 * n;
        std::vector<double> work(lwork); 
        long info;
        dgeev_(&jobvl, &jobvr, &n, inout_A.ptr(), &n, &out_eigenvalues_real[0], &out_eigenvalues_img[0], &vl[0], &n, &vr, &ldvr, &work[0], &lwork, &info);
        out_eigenvectors.resize(n);
        for (long i = 0; i < n; ++i) {
            out_eigenvectors[i].resize(n);
            for (long j = 0; j < n; ++j)
                out_eigenvectors[i][j] = vl[n * i + j];
        }
        return info;
    }
    static long invert(MatrixVLd& inout_A) {
        assert (inout_A.sizeRow() == inout_A.sizeCol());
        long n = static_cast<long>(inout_A.sizeRow());
        VectorVLl pivot(n);
        long info;
        dgetrf_(&n, &n, inout_A.ptr(), &n, pivot.ptr(), &info);
	    long lwork = 4 * n;
        std::vector<double> work(lwork); 
        dgetri_(&n, inout_A.ptr(), &n, pivot.ptr(), &work[0], &lwork, &info);
        return info;
    }
};

}
