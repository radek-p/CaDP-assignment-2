//
// Created by Radosław Piórkowski on 06.06.2016.
//

#include "MatrixOperations.h"
#include <emmintrin.h>

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))
struct multiplicationKernel {
public:
    enum { BLOCK_SIZE = 4 }; // note that instead of 2, we now process 4 elements
    multiplicationKernel(double a) : multiplier(a) {}
    inline void operator()(double* y, const double* x) const {
        (*y) += multiplier * (*x);
    }
    inline void block(double* y, const double* x) const {
        // load 4 data elements at a time
        __m128d X1 = _mm_loadu_pd(x + 0);
        __m128d X2 = _mm_loadu_pd(x + 2);
        __m128d Y1 = _mm_loadu_pd(y + 0);
        __m128d Y2 = _mm_loadu_pd(y + 2);

        // do the computations
        __m128d result0 = _mm_add_pd(Y1, _mm_mul_pd(X1, _mm_set1_pd(multiplier)));
        __m128d result1 = _mm_add_pd(Y2, _mm_mul_pd(X2, _mm_set1_pd(multiplier)));

        // store the results
        _mm_storeu_pd(y + 0, result0);
        _mm_storeu_pd(y + 2, result1);
    }
public:
    double multiplier;

};

template<class OutputPointer, class InputPointer, class Operation>
void executeKernel(OutputPointer dst, InputPointer src, size_t length, Operation op) {
    size_t n;
    // execute op on array elements block-wise
    for(n = 0; n < ROUND_DOWN(length, op.BLOCK_SIZE); n += op.BLOCK_SIZE)
        op.block(dst + n, src + n);
    // execute the remaining array elements one by one
    for(; n < length; n++) op(dst + n, src + n);
}

void DeferredSparseDenseMultiplication::addTo(DenseMatrixFragment &C) {
    exec(C, false);
}

void DeferredSparseDenseMultiplication::writeTo(DenseMatrixFragment &C) {
    exec(C, true);
}

DeferredSparseDenseMultiplication operator*(std::shared_ptr<SparseMatrixFragment> A,
                                           std::shared_ptr<DenseMatrixFragment> B) {
    return DeferredSparseDenseMultiplication(A, B);
}

DeferredSparseDenseMultiplication::DeferredSparseDenseMultiplication(
        std::shared_ptr<SparseMatrixFragment> A,
        std::shared_ptr<DenseMatrixFragment> B) : A(A), B(B) { }

void DeferredSparseDenseMultiplication::exec(DenseMatrixFragment &C, bool override) {

    if (override)
        C = 0.0;

    int pRow = A->size().pRow();
    int kRow = A->size().kRow();
    int pCol = B->size().pCol();
    int kCol = B->size().kCol();

    assert(C.size().pRow() <= pRow);
    assert(C.size().pCol() <= pCol);
    assert(C.size().kRow() >= kRow);
    assert(C.size().kCol() >= kCol);

    multiplicationKernel kernel(0);

    for (int row = pRow; row < kRow; ++row) {
        for (int rowElementIdx = A->rowIntervals[row]; rowElementIdx < A->rowIntervals[row+1]; ++rowElementIdx) {
            kernel.multiplier = A->entries[rowElementIdx];
            double *cElem = &C.at(row, pCol);
            double *bElem = &B->at(A->columns[rowElementIdx], pCol);

            executeKernel(cElem, bElem, (size_t) (kCol - pCol), kernel);
//            for (int col = pCol; col < kCol; ++col) {
//                *cElem += multiplier * (*bElem);
//                ++cElem; ++bElem;
//            }
        }
    }
}



