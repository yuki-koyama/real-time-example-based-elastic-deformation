#include "MatrixVL.h"
#include "LapackWrapper.h"

namespace KLIB {

MatrixVLd::vector_type MatrixVLd::solve(const MatrixVLd::vector_type& b) const {
    MatrixVLd LU(*this);
    MatrixVLd::vector_type x(b);
    VectorVLl pivot;
    LapackWrapper::factorize(LU, pivot);
    LapackWrapper::solve(LU, pivot, x);
    return x;
}

MatrixVLd::matrix_type MatrixVLd::invert() const {
    MatrixVLd result(*this);
    long info = LapackWrapper::invert(result);
    if (info != 0)
        assert(false);
    return result;
}



}   // namespace KLIB
