//
// Created by Radosław Piórkowski on 06.06.2016.
//

#include "MatrixOperations.h"


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

    for (int row = pRow; row < kRow; ++row) {
        for (int rowElementIdx = A->rowIntervals[row]; rowElementIdx < A->rowIntervals[row+1]; ++rowElementIdx) {
            for (int col = pCol; col < kCol; ++col) {
                C.at(row, col) += A->entries[rowElementIdx] * B->at(A->columns[rowElementIdx], col);
            }
        }
    }
}



