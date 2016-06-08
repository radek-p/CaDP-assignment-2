//
// Created by Radosław Piórkowski on 06.06.2016.
//

#include "MatrixOperations.h"
#include <emmintrin.h>

//struct saxpy_kernel
//{
//public:
//    enum { BLOCK_SIZE = 8 }; // note that instead of 4, we now process 8 elements
//    saxpy_kernel(double a) : a(a) {}
//    inline void operator()(double* y, const double* x) const {
//        (*y) += a * (*x);
//    }
//    inline void block(double* y, const double* x) const {
//        // load 8 data elements at a time
//        __m128d X1 = _mm_loadu_pd(x + 0); // _mm_loadu_ps(x + 0);
//        __m128d X2 = _mm_loadu_pd(x + 2);
//        __m128d Y1 = _mm_loadu_pd(y + 0);
//        __m128d Y2 = _mm_loadu_pd(y + 2);
//        // do the computations
//        __m128 result0 = _mm_add_ps(Y1, _mm_mul_ps(X1, _mm_set1_ps(a)));
//        __m128 result1 = _mm_add_ps(Y2, _mm_mul_ps(X2, _mm_set1_ps(a)));
//        // store the results
//        _mm_storeu_ps(y + 0, result1);
//        _mm_storeu_ps(y + 4, result2);
//    }
//protected:
//    double a;
//};

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

//    for (int row = pRow; row < kRow; ++row) {
//        for (int rowElementIdx = A->rowIntervals[row]; rowElementIdx < A->rowIntervals[row+1]; ++rowElementIdx) {
//            for (int col = pCol; col < kCol; ++col) {
//                C.at(row, col) += A->entries[rowElementIdx] * B->at(A->columns[rowElementIdx], col);
//            }
//        }
//    }


    for (int row = pRow; row < kRow; ++row) {
        for (int rowElementIdx = A->rowIntervals[row]; rowElementIdx < A->rowIntervals[row+1]; ++rowElementIdx) {
            const double multiplier = A->entries[rowElementIdx];
            double *cElem = &C.at(row, pCol);
            double *bElem = &B->at(A->columns[rowElementIdx], pCol);
            for (int col = pCol; col < kCol; ++col) {
                *cElem += multiplier * (*bElem);
                ++cElem; ++bElem;
            }
        }
    }
}



